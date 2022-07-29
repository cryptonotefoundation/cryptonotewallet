// Copyright (c) 2011-2015 The Cryptonote developers
// Copyright (c) 2016-2022 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <limits>
#include <future>
#include "CryptoNoteWrapper.h"
#include "Checkpoints/Checkpoints.h"
#include "Checkpoints/CheckpointsData.h"
#include "Common/StringTools.h"
#include "CryptoNoteCore/CryptoNoteBasicImpl.h"
#include "CryptoNoteCore/CryptoNoteFormatUtils.h"
#include "CryptoNoteCore/CryptoNoteTools.h"
#include "CryptoNoteCore/Currency.h"
#include "NodeRpcProxy/NodeRpcProxy.h"
#include "CryptoNoteCore/CoreConfig.h"
#include "P2p/NetNodeConfig.h"
#include "CryptoNoteCore/Core.h"
#include "CryptoNoteCore/Miner.h"
#include "CryptoNoteCore/MinerConfig.h"
#include "CryptoNoteCore/TransactionExtra.h"
#include "Rpc/CoreRpcServerCommandsDefinitions.h"
#include "Rpc/RpcServer.h"
#include "Rpc/JsonRpc.h"
#include "CryptoNoteProtocol/CryptoNoteProtocolHandler.h"
#include "InProcessNode/InProcessNode.h"
#include "P2p/NetNode.h"
#include "WalletLegacy/WalletLegacy.h"
#include "Logging/LoggerManager.h"
#include "LoggerAdapter.h"
#include "CurrencyAdapter.h"
#include "Settings.h"

#ifndef AUTO_VAL_INIT
#define AUTO_VAL_INIT(n) boost::value_initialized<decltype(n)>()
#endif

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

class RpcNode : public CryptoNote::INodeObserver, public CryptoNote::INodeRpcProxyObserver, public Node {
public:
  Logging::LoggerManager& m_logManager;
  RpcNode(const CryptoNote::Currency& currency, INodeCallback& callback, Logging::LoggerManager& logManager,
          const std::string& nodeHost, unsigned short nodePort, bool &enableSSL) :
    m_callback(callback),
    m_currency(currency),
    m_dispatcher(),
    m_logManager(logManager),
    m_logger(m_logManager, "RpcNode"),
    m_node(nodeHost, nodePort, "/", enableSSL)
  {
    m_node.addObserver(dynamic_cast<INodeObserver*>(this));
    m_node.addObserver(dynamic_cast<INodeRpcProxyObserver*>(this));
  }

  ~RpcNode() override {
  }

  void init(const std::function<void(std::error_code)>& callback) override {
    m_logger(Logging::DEBUGGING) << "Initializing RpcNode...";
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

  uint64_t getPeerCount() override {
    return m_node.getPeerCount();
  }

  uint64_t getMinimalFee() override {
    return m_node.getMinimalFee();
  }

  std::string feeAddress() const override {
    return m_node.feeAddress();
  }

  uint64_t feeAmount() const override {
    return m_node.feeAmount();
  }

  uint64_t getDifficulty() override {
    return m_node.getNextDifficulty();
  }

  uint64_t getTxCount() override {
    return m_node.getTransactionsCount();
  }

  uint64_t getTxPoolSize() override {
    return m_node.getTransactionsPoolSize();
  }

  uint64_t getAltBlocksCount() override {
    return m_node.getAltBlocksCount();
  }

  uint64_t getConnectionsCount() override {
    return m_node.getOutConnectionsCount();
  }

  uint64_t getOutgoingConnectionsCount() override {
    return m_node.getOutConnectionsCount();
  }

  uint64_t getIncomingConnectionsCount() override {
    return m_node.getIncConnectionsCount();
  }

  uint64_t getWhitePeerlistSize() override {
    return m_node.getWhitePeerlistSize();
  }

  uint64_t getGreyPeerlistSize() override {
    return m_node.getGreyPeerlistSize();
  }

  CryptoNote::BlockHeaderInfo getLastLocalBlockHeaderInfo() override {
    return m_node.getLastLocalBlockHeaderInfo();
  }

  uint8_t getCurrentBlockMajorVersion() override {
    return getLastLocalBlockHeaderInfo().majorVersion;
  }

  uint64_t getNextReward() override {
    return m_node.getNextReward();
  }

  bool getBlockTemplate(CryptoNote::Block& b, const CryptoNote::AccountKeys& acc, const CryptoNote::BinaryArray& ex_nonce, CryptoNote::difficulty_type& diffic, uint32_t& height) override {
    // not implemented
    return false;
  }

  bool handleBlockFound(CryptoNote::Block& b) override {
    // not implemented
    return false;
  }
  
  bool getBlockLongHash(Crypto::cn_context &context, const CryptoNote::Block& block, Crypto::Hash& res) override {
    // unsupported
    return false;
  }

  uint64_t getAlreadyGeneratedCoins() override {
    return m_node.getAlreadyGeneratedCoins();
  }

  std::vector<CryptoNote::p2pConnection> getConnections() override {
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
     m_logger(Logging::INFO) << "Failed to get connections: " << ec << ", " << ec.message();
    }

    return connections;
  }

  NodeType getNodeType() const override {
    return NodeType::RPC;
  }

  CryptoNote::IWalletLegacy* createWallet() override {
    return new CryptoNote::WalletLegacy(m_currency, m_node, m_logManager);
  }

  System::Dispatcher& getDispatcher() override {
     return m_dispatcher;
  }

  CryptoNote::INode* getNode() override {
    return &m_node;
  }

private:
  INodeCallback& m_callback;
  const CryptoNote::Currency& m_currency;
  CryptoNote::NodeRpcProxy m_node;
  System::Dispatcher m_dispatcher;
  Logging::LoggerRef m_logger;

  void peerCountUpdated(size_t count) override {
    m_callback.peerCountUpdated(*this, count);
  }

  void localBlockchainUpdated(uint32_t height) override {
    m_callback.localBlockchainUpdated(*this, height);
  }

  void lastKnownBlockHeightUpdated(uint32_t height) override {
    m_callback.lastKnownBlockHeightUpdated(*this, height);
  }

  // INodeRpcProxyObserver
  void connectionStatusUpdated(bool _connected) override {
    m_callback.connectionStatusUpdated(_connected);
  }
};

class InprocessNode : public CryptoNote::INodeObserver, public Node {
public:
  Logging::LoggerManager& m_logManager;
  InprocessNode(const CryptoNote::Currency& currency, Logging::LoggerManager& logManager, const CryptoNote::CoreConfig& coreConfig,
    const CryptoNote::NetNodeConfig& netNodeConfig, const CryptoNote::RpcServerConfig& rpcServerConfig, INodeCallback& callback) :
    m_currency(currency), m_dispatcher(),
    m_callback(callback),
    m_logManager(logManager),
    m_logger(m_logManager, "InprocessNode"),
    m_coreConfig(coreConfig),
    m_netNodeConfig(netNodeConfig),
    m_rpcServerConfig(rpcServerConfig),
    m_rpcServer(nullptr),
    m_protocolHandler(currency, m_dispatcher, m_core, nullptr, logManager),
    m_core(currency, &m_protocolHandler, logManager, m_dispatcher, true, false, false),
    m_nodeServer(m_dispatcher, m_protocolHandler, logManager),
    m_node(m_core, m_protocolHandler)
  {

      if (Settings::instance().withoutCheckpoints()) {
        m_logger(Logging::INFO) << "Loading without checkpoints";
      } else if (Settings::instance().isTestnet()) {
        m_logger(Logging::INFO) << "Running in Testnet mode";
      } else {
        bool allowReorg = Settings::instance().alowReorg();
        if (allowReorg) {
          m_logger(Logging::WARNING) << "Deep reorganization is allowed!";
        }
        CryptoNote::Checkpoints checkpoints(logManager, allowReorg);
        checkpoints.load_checkpoints_from_dns();
        for (const CryptoNote::CheckpointData& checkpoint : CryptoNote::CHECKPOINTS) {
          checkpoints.add_checkpoint(checkpoint.height, checkpoint.blockId);
        }
        m_core.set_checkpoints(std::move(checkpoints));
      }

      m_core.set_cryptonote_protocol(&m_protocolHandler);
      m_protocolHandler.set_p2p_endpoint(&m_nodeServer);
  }

  ~InprocessNode() override {

  }

  void init(const std::function<void(std::error_code)>& callback) override {

    m_logger(Logging::INFO) << "Initializing InprocessNode...";

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

      if (Settings::instance().hasRunRpc()) {
        m_logger(Logging::INFO) << "Starting core rpc server...";
        m_rpcServer = new CryptoNote::RpcServer(m_rpcServerConfig, m_dispatcher, m_logManager, m_core, m_nodeServer, m_protocolHandler);
        m_rpcServer->start();
        m_logger(Logging::INFO) << "Core rpc server started ok";
      }

    } catch (std::runtime_error& _err) {
      callback(make_error_code(CryptoNote::error::NOT_INITIALIZED));
      return;
    }

    m_node.init([this, callback](std::error_code ec) {
      m_node.addObserver(dynamic_cast<INodeObserver*>(this));
      callback(ec);
    });

    m_nodeServer.run();
    m_nodeServer.deinit();
    if (Settings::instance().hasRunRpc() && m_rpcServer != nullptr) {
      m_rpcServer->stop();
    }
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

  uint64_t getPeerCount() override {
    return m_nodeServer.get_connections_count();
  }

  uint64_t getDifficulty() override {
    return m_core.getNextBlockDifficulty();
  }

  uint64_t getTxCount() override {
    return m_core.getBlockchainTotalTransactions() - m_core.getCurrentBlockchainHeight();
  }

  uint64_t getTxPoolSize() override {
    return m_core.getPoolTransactionsCount();
  }

  uint64_t getAltBlocksCount() override {
    return m_core.getAlternativeBlocksCount();
  }

  uint64_t getConnectionsCount() override {
    return m_nodeServer.get_connections_count();
  }

  uint64_t getOutgoingConnectionsCount() override {
    return m_nodeServer.get_outgoing_connections_count();
  }

  uint64_t getIncomingConnectionsCount() override {
    return m_nodeServer.get_connections_count() - m_nodeServer.get_outgoing_connections_count();
  }

  uint64_t getWhitePeerlistSize() override {
    return m_nodeServer.getPeerlistManager().get_white_peers_count();
  }

  uint64_t getGreyPeerlistSize() override {
    return m_nodeServer.getPeerlistManager().get_gray_peers_count();
  }

  uint64_t getMinimalFee() override {
    return m_core.getMinimalFee();
  }

  std::string feeAddress() const override {
    return m_node.feeAddress();
  }

  uint64_t feeAmount() const override {
    return m_node.feeAmount();
  }

  CryptoNote::BlockHeaderInfo getLastLocalBlockHeaderInfo() override {
    return m_node.getLastLocalBlockHeaderInfo();
  }

  uint8_t getCurrentBlockMajorVersion() override {
    return getLastLocalBlockHeaderInfo().majorVersion;
  }

  uint64_t getNextReward() override {
    return m_node.getNextReward();
  }

  bool getBlockTemplate(CryptoNote::Block& b, const CryptoNote::AccountKeys& acc, const CryptoNote::BinaryArray& ex_nonce, CryptoNote::difficulty_type& diffic, uint32_t& height) override {
    return m_core.get_block_template(b, acc, diffic, height, ex_nonce);
  }

  bool handleBlockFound(CryptoNote::Block& b) override {
    return m_core.handle_block_found(b);
  }
  
  bool getBlockLongHash(Crypto::cn_context &context, const CryptoNote::Block& block, Crypto::Hash& res) override {
    return m_core.getBlockLongHash(context, block, res);
  }

  uint64_t getAlreadyGeneratedCoins() override {
    return m_node.getAlreadyGeneratedCoins();
  }

  std::vector<CryptoNote::p2pConnection> getConnections() override {
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
      m_logger(Logging::INFO) << "Failed to get connections: " << ec << ", " << ec.message();
    }

    return connections;
  }

  NodeType getNodeType() const override {
    return NodeType::IN_PROCESS;
  }

  CryptoNote::IWalletLegacy* createWallet() override {
    return new CryptoNote::WalletLegacy(m_currency, m_node, m_logManager);
  }

  System::Dispatcher& getDispatcher() override {
     return m_dispatcher;
  }

  CryptoNote::INode* getNode() override {
    return &m_node;
  }

private:
  INodeCallback& m_callback;
  const CryptoNote::Currency& m_currency;
  System::Dispatcher m_dispatcher;
  CryptoNote::CoreConfig m_coreConfig;
  CryptoNote::NetNodeConfig m_netNodeConfig;
  CryptoNote::RpcServerConfig m_rpcServerConfig;
  CryptoNote::Core m_core;
  CryptoNote::CryptoNoteProtocolHandler m_protocolHandler;
  CryptoNote::NodeServer m_nodeServer;
  CryptoNote::InProcessNode m_node;
  std::future<bool> m_nodeServerFuture;
  Logging::LoggerRef m_logger;
  CryptoNote::RpcServer* m_rpcServer;

  void peerCountUpdated(size_t count) override {
    m_callback.peerCountUpdated(*this, count);
  }

  void localBlockchainUpdated(uint32_t height) override {
    m_callback.localBlockchainUpdated(*this, height);
  }

  void lastKnownBlockHeightUpdated(uint32_t height) override {
    m_callback.lastKnownBlockHeightUpdated(*this, height);
  }

  // dummy, used only for INodeRpcProxyObserver
  void connectionStatusUpdated(bool _connected) {
    m_callback.connectionStatusUpdated(_connected);
  }
};

Node* createRpcNode(const CryptoNote::Currency& currency, INodeCallback& callback, Logging::LoggerManager& logManager,  const std::string& nodeHost, unsigned short nodePort, bool enableSSL) {
  return new RpcNode(currency, callback, logManager, nodeHost, nodePort, enableSSL);
}

Node* createInprocessNode(const CryptoNote::Currency& currency, Logging::LoggerManager& logManager,
  const CryptoNote::CoreConfig& coreConfig, const CryptoNote::NetNodeConfig& netNodeConfig, const CryptoNote::RpcServerConfig& rpcServerConfig, INodeCallback& callback) {
  return new InprocessNode(currency, logManager, coreConfig, netNodeConfig, rpcServerConfig, callback);
}

}
