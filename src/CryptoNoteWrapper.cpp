// Copyright (c) 2011-2015 The Cryptonote developers
// Copyright (c) 2016-2020 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <limits>
#include <future>
#include "CryptoNoteWrapper.h"
#include <CheckpointsData.h>
#include "Common/StringTools.h"
#include "CryptoNoteCore/CryptoNoteBasicImpl.h"
#include "CryptoNoteCore/CryptoNoteFormatUtils.h"
#include "CryptoNoteCore/Currency.h"
#include "NodeRpcProxy/NodeRpcProxy.h"
#include "CryptoNoteCore/CoreConfig.h"
#include "P2p/NetNodeConfig.h"
#include "CryptoNoteCore/Core.h"
#include "CryptoNoteCore/Miner.h"
#include "CryptoNoteCore/MinerConfig.h"
#include "CryptoNoteCore/TransactionExtra.h"
#include "Rpc/CoreRpcServerCommandsDefinitions.h"
#include "Rpc/HttpClient.h"
#include "CryptoNoteProtocol/CryptoNoteProtocolHandler.h"
#include "InProcessNode/InProcessNode.h"
#include "P2p/NetNode.h"
#include "WalletLegacy/WalletLegacy.h"
#include "Logging/LoggerManager.h"
#include "System/Dispatcher.h"
#include "CurrencyAdapter.h"
#include "Settings.h"
#include <QDebug>

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
  std::vector<uint8_t> extraVec;
  extraVec.reserve(extra.size());
  std::for_each(extra.begin(), extra.end(), [&extraVec](const char el) { extraVec.push_back(el); });

  Crypto::Hash paymentId;
  std::string res = (CryptoNote::getPaymentIdFromTxExtra(extraVec, paymentId) && paymentId != CryptoNote::NULL_HASH ? Common::podToHex(paymentId) : "");
  return res;
}

inline std::string interpret_rpc_response(bool ok, const std::string& status) {
  std::string err;
  if (ok) {
    if (status == CORE_RPC_STATUS_BUSY) {
      err = "daemon is busy. Please try later";
    } else if (status != CORE_RPC_STATUS_OK) {
      err = status;
    }
  } else {
    err = "possible lost connection to daemon";
  }
  return err;
}

}

Node::~Node() {
}

class RpcNode : CryptoNote::INodeObserver, public Node {
public:
  Logging::LoggerManager& m_logManager;
  RpcNode(const CryptoNote::Currency& currency, INodeCallback& callback, Logging::LoggerManager& logManager, const std::string& nodeHost, unsigned short nodePort) :
    m_callback(callback),
    m_currency(currency),
    m_dispatcher(),
    m_logManager(logManager),
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

  uint64_t getPeerCount() {
    return m_node.getPeerCount();
  }

  uint64_t getMinimalFee() {
    return m_node.getMinimalFee();
  }

  std::string feeAddress() const {
    return m_node.feeAddress();
  }

  uint64_t feeAmount() const {
    return m_node.feeAmount();
  }

  uint64_t getDifficulty() {
    return m_node.getNextDifficulty();
  }

  uint64_t getTxCount() {
    return m_node.getTransactionsCount();
  }

  uint64_t getTxPoolSize() {
    return m_node.getTransactionsPoolSize();
  }

  uint64_t getAltBlocksCount() {
    return m_node.getAltBlocksCount();
  }

  uint64_t getConnectionsCount() {
    return m_node.getOutConnectionsCount();
  }

  uint64_t getOutgoingConnectionsCount() {
    return m_node.getOutConnectionsCount();
  }

  uint64_t getIncomingConnectionsCount() {
    return m_node.getIncConnectionsCount();
  }

  uint64_t getWhitePeerlistSize() {
    return m_node.getWhitePeerlistSize();
  }

  uint64_t getGreyPeerlistSize() {
    return m_node.getGreyPeerlistSize();
  }

  CryptoNote::BlockHeaderInfo getLastLocalBlockHeaderInfo() {
    return m_node.getLastLocalBlockHeaderInfo();
  }

  uint8_t getCurrentBlockMajorVersion() {
    return getLastLocalBlockHeaderInfo().majorVersion;
  }

  uint64_t getAlreadyGeneratedCoins() {
    return m_node.getAlreadyGeneratedCoins();
  }

  std::vector<CryptoNote::p2pConnection> getConnections() {
    std::vector<CryptoNote::p2pConnection> connections;

    auto getConnectionsCompleted = std::promise<std::error_code>();
    auto getConnectionsWaitFuture = getConnectionsCompleted.get_future();

    m_node.getConnections(std::ref(connections),
      [&getConnectionsCompleted](std::error_code ec) {
      auto detachedPromise = std::move(getConnectionsCompleted);
      detachedPromise.set_value(ec);
    });

    std::error_code ec = getConnectionsWaitFuture.get();

    if (ec) {
      //qDebug() << "Failed to get connections: " << ec << ", " << ec.message();
    }

    return connections;
  }

  CryptoNote::IWalletLegacy* createWallet() override {
    return new CryptoNote::WalletLegacy(m_currency, m_node, m_logManager);
  }

private:
  INodeCallback& m_callback;
  const CryptoNote::Currency& m_currency;
  CryptoNote::NodeRpcProxy m_node;
  System::Dispatcher m_dispatcher;

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
  Logging::LoggerManager& m_logManager;
  InprocessNode(const CryptoNote::Currency& currency, Logging::LoggerManager& logManager, const CryptoNote::CoreConfig& coreConfig,
    const CryptoNote::NetNodeConfig& netNodeConfig, INodeCallback& callback) :
    m_currency(currency), m_dispatcher(),
    m_callback(callback),
    m_logManager(logManager),
    m_coreConfig(coreConfig),
    m_netNodeConfig(netNodeConfig),
    m_protocolHandler(currency, m_dispatcher, m_core, nullptr, logManager),
    m_core(currency, &m_protocolHandler, logManager, m_dispatcher, true),
    m_nodeServer(m_dispatcher, m_protocolHandler, logManager),
    m_node(m_core, m_protocolHandler) {

      CryptoNote::Checkpoints checkpoints(logManager);
      checkpoints.load_checkpoints_from_dns();
      for (const CryptoNote::CheckpointData& checkpoint : CryptoNote::CHECKPOINTS) {
        checkpoints.add_checkpoint(checkpoint.height, checkpoint.blockId);
      }
      if (!Settings::instance().isTestnet()) {
        m_core.set_checkpoints(std::move(checkpoints));
      }

      m_core.set_cryptonote_protocol(&m_protocolHandler);
      m_protocolHandler.set_p2p_endpoint(&m_nodeServer);

  }

  ~InprocessNode() override {

  }

  void init(const std::function<void(std::error_code)>& callback) override {
    try {
      if (!m_core.init(m_coreConfig, CryptoNote::MinerConfig(), true)) {
        callback(make_error_code(CryptoNote::error::NOT_INITIALIZED));
        return;
      }

      if(Settings::instance().getRollBack() != std::numeric_limits<uint32_t>::max()) {
        m_core.rollbackBlockchain(Settings::instance().getRollBack());
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
    m_core.deinit();
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

  uint64_t getPeerCount() {
    return m_nodeServer.get_connections_count();
  }

  uint64_t getDifficulty() {
    return m_core.getNextBlockDifficulty();
  }

  uint64_t getTxCount() {
    return m_core.getBlockchainTotalTransactions() - m_core.getCurrentBlockchainHeight();
  }

  uint64_t getTxPoolSize() {
    return m_core.getPoolTransactionsCount();
  }

  uint64_t getAltBlocksCount() {
    return m_core.getAlternativeBlocksCount();
  }

  uint64_t getConnectionsCount() {
    return m_nodeServer.get_connections_count();
  }

  uint64_t getOutgoingConnectionsCount() {
    return m_nodeServer.get_outgoing_connections_count();
  }

  uint64_t getIncomingConnectionsCount() {
    return m_nodeServer.get_connections_count() - m_nodeServer.get_outgoing_connections_count();
  }

  uint64_t getWhitePeerlistSize() {
    return m_nodeServer.getPeerlistManager().get_white_peers_count();
  }

  uint64_t getGreyPeerlistSize() {
    return m_nodeServer.getPeerlistManager().get_gray_peers_count();
  }

  uint64_t getMinimalFee() {
    return m_core.getMinimalFee();
  }

  std::string feeAddress() const {
    return m_node.feeAddress();
  }

  uint64_t feeAmount() const {
    return m_node.feeAmount();
  }

  CryptoNote::BlockHeaderInfo getLastLocalBlockHeaderInfo() {
    return m_node.getLastLocalBlockHeaderInfo();
  }

  uint8_t getCurrentBlockMajorVersion() {
    return getLastLocalBlockHeaderInfo().majorVersion;
  }

  uint64_t getAlreadyGeneratedCoins() {
    return m_node.getAlreadyGeneratedCoins();
  }

  std::vector<CryptoNote::p2pConnection> getConnections() {
    std::vector<CryptoNote::p2pConnection> connections;

    auto getConnectionsCompleted = std::promise<std::error_code>();
    auto getConnectionsWaitFuture = getConnectionsCompleted.get_future();

    m_node.getConnections(std::ref(connections),
      [&getConnectionsCompleted](std::error_code ec) {
      auto detachedPromise = std::move(getConnectionsCompleted);
      detachedPromise.set_value(ec);
    });

    std::error_code ec = getConnectionsWaitFuture.get();

    if (ec) {
      //qDebug() << "Failed to get connections: " << ec << ", " << ec.message();
    }

    return connections;
  }

  CryptoNote::IWalletLegacy* createWallet() override {
    return new CryptoNote::WalletLegacy(m_currency, m_node, m_logManager);
  }

private:
  INodeCallback& m_callback;
  const CryptoNote::Currency& m_currency;
  System::Dispatcher m_dispatcher;
  CryptoNote::CoreConfig m_coreConfig;
  CryptoNote::NetNodeConfig m_netNodeConfig;
  CryptoNote::Core m_core;
  CryptoNote::CryptoNoteProtocolHandler m_protocolHandler;
  CryptoNote::NodeServer m_nodeServer;
  CryptoNote::InProcessNode m_node;
  std::future<bool> m_nodeServerFuture;

  void peerCountUpdated(size_t count) {
    //m_callback.peerCountUpdated(*this, count);
    m_callback.peerCountUpdated(*this, m_nodeServer.get_connections_count() - 1);
  }

  void localBlockchainUpdated(uint64_t height) {
    m_callback.localBlockchainUpdated(*this, height);
  }

  void lastKnownBlockHeightUpdated(uint64_t height) {
    m_callback.lastKnownBlockHeightUpdated(*this, height);
  }
};

Node* createRpcNode(const CryptoNote::Currency& currency, INodeCallback& callback, Logging::LoggerManager& logManager,  const std::string& nodeHost, unsigned short nodePort) {
  return new RpcNode(currency, callback, logManager, nodeHost, nodePort);
}

Node* createInprocessNode(const CryptoNote::Currency& currency, Logging::LoggerManager& logManager,
  const CryptoNote::CoreConfig& coreConfig, const CryptoNote::NetNodeConfig& netNodeConfig, INodeCallback& callback) {
  return new InprocessNode(currency, logManager, coreConfig, netNodeConfig, callback);
}

}
