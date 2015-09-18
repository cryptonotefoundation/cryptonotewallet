// Copyright (c) 2011-2015 The Cryptonote developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "CryptoNoteWrapper.h"
#include "CryptoNoteCore/CryptoNoteBasicImpl.h"
#include "CryptoNoteCore/CryptoNoteFormatUtils.h"
#include "CryptoNoteCore/Currency.h"
#include "NodeRpcProxy/NodeRpcProxy.h"
#include "CryptoNoteCore/CoreConfig.h"
#include "P2p/NetNodeConfig.h"
#include "CryptoNoteCore/Core.h"
#include "CryptoNoteProtocol/CryptoNoteProtocolHandler.h"
#include "InProcessNode/InProcessNode.h"
#include "P2p/NetNode.h"
#include "WalletLegacy/WalletLegacy.h"
#include "Logging/LoggerManager.h"
#include "System/Dispatcher.h"

namespace WalletGui {

namespace {

bool parsePaymentId(const std::string& payment_id_str, Crypto::Hash& payment_id) {
  return CryptoNote::parsePaymentId(payment_id_str, payment_id);
}

std::string convertPaymentId(const std::string& paymentIdString) {
  if (paymentIdString.empty()) {
    return "";
  }

  Crypto::Hash paymentId;
  if (!parsePaymentId(paymentIdString, paymentId)) {
    std::stringstream errorStr;
    errorStr << "Payment id has invalid format: \"" + paymentIdString + "\", expected 64-character string";
    throw std::runtime_error(errorStr.str());
  }

  std::vector<uint8_t> extra;
  CryptoNote::BinaryArray extraNonce;
  CryptoNote::setPaymentIdToTransactionExtraNonce(extraNonce, paymentId);
  if (!CryptoNote::addExtraNonceToTransactionExtra(extra, extraNonce)) {
    std::stringstream errorStr;
    errorStr << "Something went wrong with payment_id. Please check its format: \"" + paymentIdString + "\", expected 64-character string";
    throw std::runtime_error(errorStr.str());
  }

  return std::string(extra.begin(), extra.end());
}

std::string extractPaymentId(const std::string& extra) {
  std::vector<CryptoNote::TransactionExtraField> extraFields;
  std::vector<uint8_t> extraVector;
  std::copy(extra.begin(), extra.end(), std::back_inserter(extraVector));

  if (!CryptoNote::parseTransactionExtra(extraVector, extraFields)) {
    throw std::runtime_error("Can't parse extra");
  }

  std::string result;
  CryptoNote::TransactionExtraNonce extraNonce;
  if (CryptoNote::findTransactionExtraFieldByType(extraFields, extraNonce)) {
    Crypto::Hash paymentIdHash;
    if (CryptoNote::getPaymentIdFromTransactionExtraNonce(extraNonce.nonce, paymentIdHash)) {
      unsigned char* buff = reinterpret_cast<unsigned char *>(&paymentIdHash);
      for (size_t i = 0; i < sizeof(paymentIdHash); ++i) {
        result.push_back("0123456789ABCDEF"[buff[i] >> 4]);
        result.push_back("0123456789ABCDEF"[buff[i] & 15]);
      }
    }
  }

  return result;
}


}

Node::~Node() {
}

class RpcNode : CryptoNote::INodeObserver, public Node {
public:
  RpcNode(const CryptoNote::Currency& currency, INodeCallback& callback, const std::string& nodeHost, unsigned short nodePort) :
    m_callback(callback),
    m_currency(currency),
    m_node(nodeHost, nodePort) {
    m_node.addObserver(this);
  }

  ~RpcNode() override {
  }

  void init(const std::function<void(std::error_code)>& callback) override {
    m_node.init(callback);
  }

  void deinit() override {
  }

  std::string convertPaymentId(const std::string& paymentIdString) override {
    return WalletGui::convertPaymentId(paymentIdString);
  }

  std::string extractPaymentId(const std::string& extra) override {
    return WalletGui::extractPaymentId(extra);
  }

  uint64_t getLastKnownBlockHeight() const override {
    return m_node.getLastKnownBlockHeight();
  }

  uint64_t getLastLocalBlockHeight() const override {
    return m_node.getLastLocalBlockHeight();
  }

  uint64_t getLastLocalBlockTimestamp() const override {
    return m_node.getLastLocalBlockTimestamp();
  }

  uint64_t getPeerCount() const override {
    return m_node.getPeerCount();
  }

  CryptoNote::IWalletLegacy* createWallet() override {
    return new CryptoNote::WalletLegacy(m_currency, m_node);
  }

private:
  INodeCallback& m_callback;
  const CryptoNote::Currency& m_currency;
  CryptoNote::NodeRpcProxy m_node;

  void peerCountUpdated(size_t count) {
    m_callback.peerCountUpdated(*this, count);
  }

  void localBlockchainUpdated(uint64_t height) {
    m_callback.localBlockchainUpdated(*this, height);
  }

  void lastKnownBlockHeightUpdated(uint64_t height) {
    m_callback.lastKnownBlockHeightUpdated(*this, height);
  }
};

class InprocessNode : CryptoNote::INodeObserver, public Node {
public:
  InprocessNode(const CryptoNote::Currency& currency, Logging::LoggerManager& logManager, const CryptoNote::CoreConfig& coreConfig,
    const CryptoNote::NetNodeConfig& netNodeConfig, INodeCallback& callback) :
    m_currency(currency), m_dispatcher(),
    m_callback(callback),
    m_coreConfig(coreConfig),
    m_netNodeConfig(netNodeConfig),
    m_protocolHandler(currency, m_dispatcher, m_core, nullptr, logManager),
    m_core(currency, &m_protocolHandler, logManager),
    m_nodeServer(m_dispatcher, m_protocolHandler, logManager),
    m_node(m_core, m_protocolHandler) {

    m_core.set_cryptonote_protocol(&m_protocolHandler);
    m_protocolHandler.set_p2p_endpoint(&m_nodeServer);
    CryptoNote::Checkpoints checkpoints(logManager);
    for (const CryptoNote::CheckpointData& checkpoint : CryptoNote::CHECKPOINTS) {
      checkpoints.add_checkpoint(checkpoint.height, checkpoint.blockId);
    }
  }

  ~InprocessNode() override {

  }

  void init(const std::function<void(std::error_code)>& callback) override {
    try {
      if (!m_core.init(m_coreConfig, CryptoNote::MinerConfig(), true)) {
        callback(make_error_code(CryptoNote::error::NOT_INITIALIZED));
        return;
      }

      if (!m_nodeServer.init(m_netNodeConfig)) {
        callback(make_error_code(CryptoNote::error::NOT_INITIALIZED));
        return;
      }
    } catch (std::runtime_error& _err) {
      callback(make_error_code(CryptoNote::error::NOT_INITIALIZED));
      return;
    }

    m_node.init([this, callback](std::error_code ec) {
      m_node.addObserver(this);
      callback(ec);
    });

    m_nodeServer.run();
    m_nodeServer.deinit();
    m_node.shutdown();
  }

  void deinit() override {
    m_nodeServer.sendStopSignal();
  }

  std::string convertPaymentId(const std::string& paymentIdString) override {
    return WalletGui::convertPaymentId(paymentIdString);
  }

  std::string extractPaymentId(const std::string& extra) override {
    return WalletGui::extractPaymentId(extra);
  }

  uint64_t getLastKnownBlockHeight() const override {
    return m_node.getLastKnownBlockHeight();
  }

  uint64_t getLastLocalBlockHeight() const override {
    return m_node.getLastLocalBlockHeight();
  }

  uint64_t getLastLocalBlockTimestamp() const override {
    return m_node.getLastLocalBlockTimestamp();
  }

  uint64_t getPeerCount() const override {
    return m_node.getPeerCount();
  }

  CryptoNote::IWalletLegacy* createWallet() override {
    return new CryptoNote::WalletLegacy(m_currency, m_node);
  }

private:
  INodeCallback& m_callback;
  const CryptoNote::Currency& m_currency;
  System::Dispatcher m_dispatcher;
  CryptoNote::CoreConfig m_coreConfig;
  CryptoNote::NetNodeConfig m_netNodeConfig;
  CryptoNote::core m_core;
  CryptoNote::CryptoNoteProtocolHandler m_protocolHandler;
  CryptoNote::NodeServer m_nodeServer;
  CryptoNote::InProcessNode m_node;
  std::future<bool> m_nodeServerFuture;

  void peerCountUpdated(size_t count) {
    m_callback.peerCountUpdated(*this, count);
  }

  void localBlockchainUpdated(uint64_t height) {
    m_callback.localBlockchainUpdated(*this, height);
  }

  void lastKnownBlockHeightUpdated(uint64_t height) {
    m_callback.lastKnownBlockHeightUpdated(*this, height);
  }
};

Node* createRpcNode(const CryptoNote::Currency& currency, INodeCallback& callback, const std::string& nodeHost, unsigned short nodePort) {
  return new RpcNode(currency, callback, nodeHost, nodePort);
}

Node* createInprocessNode(const CryptoNote::Currency& currency, Logging::LoggerManager& logManager,
  const CryptoNote::CoreConfig& coreConfig, const CryptoNote::NetNodeConfig& netNodeConfig, INodeCallback& callback) {
  return new InprocessNode(currency, logManager, coreConfig, netNodeConfig, callback);
}

}
