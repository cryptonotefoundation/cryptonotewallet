// Copyright (c) 2011-2015 The Cryptonote developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "CryptoNote.h"
#include "cryptonote_core/cryptonote_basic_impl.h"
#include "cryptonote_core/cryptonote_format_utils.h"
#include "cryptonote_core/Currency.h"
#include "node_rpc_proxy/NodeRpcProxy.h"
#include "cryptonote_core/CoreConfig.h"
#include "cryptonote_core/cryptonote_core.h"
#include "cryptonote_protocol/cryptonote_protocol_handler.h"
#include "inprocess_node/InProcessNode.h"
#include "p2p/net_node.h"
#include "wallet/Wallet.h"

namespace WalletGui {

namespace {

bool parsePaymentId(const std::string& payment_id_str, crypto::hash& payment_id) {
  cryptonote::blobdata payment_id_data;
  if (!epee::string_tools::parse_hexstr_to_binbuff(payment_id_str, payment_id_data)) {
    return false;
  }

  if (sizeof(crypto::hash) != payment_id_data.size()) {
    return false;
  }

  payment_id = *reinterpret_cast<const crypto::hash*>(payment_id_data.data());
  return true;
}

std::string convertPaymentId(const std::string& paymentIdString) {
  if (paymentIdString.empty()) {
    return "";
  }

  crypto::hash paymentId;
  if (!parsePaymentId(paymentIdString, paymentId)) {
    std::stringstream errorStr;
    errorStr << "Payment id has invalid format: \"" + paymentIdString + "\", expected 64-character string";
    throw std::runtime_error(errorStr.str());
  }

  std::vector<uint8_t> extra;
  std::string extra_nonce;
  cryptonote::set_payment_id_to_tx_extra_nonce(extra_nonce, paymentId);
  if (!cryptonote::add_extra_nonce_to_tx_extra(extra, extra_nonce)) {
    std::stringstream errorStr;
    errorStr << "Something went wrong with payment_id. Please check its format: \"" + paymentIdString + "\", expected 64-character string";
    throw std::runtime_error(errorStr.str());
  }

  return std::string(extra.begin(), extra.end());
}

std::string extractPaymentId(const std::string& extra) {
  std::vector<cryptonote::tx_extra_field> extraFields;
  std::vector<uint8_t> extraVector;
  std::copy(extra.begin(), extra.end(), std::back_inserter(extraVector));

  if (!cryptonote::parse_tx_extra(extraVector, extraFields)) {
    throw std::runtime_error("Can't parse extra");
  }

  std::string result;
  cryptonote::tx_extra_nonce extra_nonce;
  if (cryptonote::find_tx_extra_field_by_type(extraFields, extra_nonce)) {
    crypto::hash paymentIdHash;
    if (cryptonote::get_payment_id_from_tx_extra_nonce(extra_nonce.nonce, paymentIdHash)) {
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
  RpcNode(const cryptonote::Currency& currency, INodeCallback& callback, const std::string& nodeHost, unsigned short nodePort) :
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

  CryptoNote::IWallet* createWallet() override {
    return new CryptoNote::Wallet(m_currency, m_node);
  }

private:
  INodeCallback& m_callback;
  const cryptonote::Currency& m_currency;
  cryptonote::NodeRpcProxy m_node;

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
  InprocessNode(const cryptonote::Currency& currency, INodeCallback& callback, const cryptonote::CoreConfig& coreConfig) :
    m_callback(callback),
    m_currency(currency),
    m_coreConfig(coreConfig),
    m_core(m_currency, nullptr),
    m_protocolHandler(m_core, nullptr),
    m_nodeServer(m_protocolHandler),
    m_node(m_core, m_protocolHandler) {

    m_core.set_cryptonote_protocol(&m_protocolHandler);
    m_protocolHandler.set_p2p_endpoint(&m_nodeServer);
    cryptonote::checkpoints checkpoints;
    for (const cryptonote::CheckpointData& checkpoint : cryptonote::CHECKPOINTS) {
      checkpoints.add_checkpoint(checkpoint.height, checkpoint.blockId);
    }

    m_core.set_checkpoints(std::move(checkpoints));
  }

  ~InprocessNode() override {
     
  }

  void init(const std::function<void(std::error_code)>& callback) override {
    if (!m_core.init(m_coreConfig, cryptonote::MinerConfig(), true)) {
        callback(make_error_code(cryptonote::error::NOT_INITIALIZED));
        return;
    }
    if (!m_nodeServer.init(nodetool::NetNodeConfig(), false)) {
        m_core.deinit();
        callback(make_error_code(cryptonote::error::NOT_INITIALIZED));
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
    m_nodeServer.send_stop_signal();
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

  CryptoNote::IWallet* createWallet() override {
    return new CryptoNote::Wallet(m_currency, m_node);
  }

private:
  INodeCallback& m_callback;
  const cryptonote::Currency& m_currency;
  cryptonote::CoreConfig m_coreConfig;
  cryptonote::core m_core;
  cryptonote::t_cryptonote_protocol_handler<cryptonote::core> m_protocolHandler;
  nodetool::node_server<cryptonote::t_cryptonote_protocol_handler<cryptonote::core>> m_nodeServer;
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

Node* createRpcNode(const cryptonote::Currency& currency, INodeCallback& callback, const std::string& nodeHost, unsigned short nodePort) {
  return new RpcNode(currency, callback, nodeHost, nodePort);
}

Node* createInprocessNode(const cryptonote::Currency& currency, INodeCallback& callback, const cryptonote::CoreConfig& coreConfig) {
  return new InprocessNode(currency, callback, coreConfig);
}

}
