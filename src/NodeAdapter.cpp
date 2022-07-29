// Copyright (c) 2011-2015 The Cryptonote developers
// Copyright (c) 2016-2022 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QTimer>
#include <QUrl>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include "CryptoNoteCore/CoreConfig.h"
#include "CurrencyAdapter.h"
#include "LoggerAdapter.h"
#include "NodeAdapter.h"
#include "P2p/NetNodeConfig.h"
#include "Settings.h"
#include "Wallet/WalletErrors.h"

namespace WalletGui {

namespace {

std::vector<std::string> convertStringListToVector(const QStringList& list) {
  std::vector<std::string> result;
  Q_FOREACH (const QString& item, list) {
    result.push_back(item.toStdString());
  }

  return result;
}

}

class InProcessNodeInitializer : public QObject {
  Q_OBJECT
  Q_DISABLE_COPY(InProcessNodeInitializer)

Q_SIGNALS:
  void nodeInitCompletedSignal();
  void nodeInitFailedSignal(int _errorCode);
  void nodeDeinitCompletedSignal();

public:
  InProcessNodeInitializer(QObject* _parent = nullptr) {
  }

  ~InProcessNodeInitializer() {
  }

  void start(Node** _node, const CryptoNote::Currency* currency,  INodeCallback* _callback, Logging::LoggerManager* _loggerManager,
    const CryptoNote::CoreConfig& _coreConfig, const CryptoNote::NetNodeConfig& _netNodeConfig, const CryptoNote::RpcServerConfig& _rpcServerConfig) {
    (*_node) = createInprocessNode(*currency, *_loggerManager, _coreConfig, _netNodeConfig, _rpcServerConfig, *_callback);
    try {
      (*_node)->init([this](std::error_code _err) {
          if (_err) {
            Q_EMIT nodeInitFailedSignal(_err.value());
            QCoreApplication::processEvents();
            return;
          }

          Q_EMIT nodeInitCompletedSignal();
          QCoreApplication::processEvents();
        });
    } catch (std::runtime_error& err) {
      Q_EMIT nodeInitFailedSignal(CryptoNote::error::INTERNAL_WALLET_ERROR);
      QCoreApplication::processEvents();
      return;
    }

    delete *_node;
    *_node = nullptr;
    Q_EMIT nodeDeinitCompletedSignal();
  }

  void stop(Node** _node) {
    Q_CHECK_PTR(*_node);
    (*_node)->deinit();
  }
};

NodeAdapter& NodeAdapter::instance() {
  static NodeAdapter inst;
  return inst;
}

NodeAdapter::NodeAdapter() : QObject(), m_node(nullptr), m_nodeInitializerThread(), m_nodeInitializer(new InProcessNodeInitializer) {
  m_nodeInitializer->moveToThread(&m_nodeInitializerThread);

  qRegisterMetaType<CryptoNote::CoreConfig>("CryptoNote::CoreConfig");
  qRegisterMetaType<CryptoNote::NetNodeConfig>("CryptoNote::NetNodeConfig");
  qRegisterMetaType<CryptoNote::RpcServerConfig>("CryptoNote::RpcServerConfig");

  connect(m_nodeInitializer, &InProcessNodeInitializer::nodeInitCompletedSignal, this, &NodeAdapter::nodeInitCompletedSignal, Qt::QueuedConnection);
  connect(this, &NodeAdapter::initNodeSignal, m_nodeInitializer, &InProcessNodeInitializer::start, Qt::QueuedConnection);
  connect(this, &NodeAdapter::deinitNodeSignal, m_nodeInitializer, &InProcessNodeInitializer::stop, Qt::QueuedConnection);
}

NodeAdapter::~NodeAdapter() {
}

quintptr NodeAdapter::getPeerCount() {
  Q_ASSERT(m_node != nullptr);
  return m_node->getPeerCount();
}

std::string NodeAdapter::convertPaymentId(const QString& _paymentIdString) const {
  Q_CHECK_PTR(m_node);
  try {
    return m_node->convertPaymentId(_paymentIdString.toStdString());
  } catch (std::runtime_error& err) {
  }
  return std::string();
}

QString NodeAdapter::extractPaymentId(const std::string& _extra) const {
  Q_CHECK_PTR(m_node);
  return QString::fromStdString(m_node->extractPaymentId(_extra));
}

CryptoNote::IWalletLegacy* NodeAdapter::createWallet() const {
  Q_CHECK_PTR(m_node);
  return m_node->createWallet();
}

NodeType NodeAdapter::getNodeType() const {
  return m_node == nullptr ? NodeType::UNKNOWN : m_node->getNodeType();
}

bool NodeAdapter::init() {
  Q_ASSERT(m_node == nullptr);

  QString connection = Settings::instance().getConnection();

  if(connection.compare("embedded") == 0) {

    LoggerAdapter::instance().log("Initializing embedded node...");
    m_node = nullptr;
    return initInProcessNode();

  } else if(connection.compare("local") == 0) {

    LoggerAdapter::instance().log("Initializing with local node...");
    QUrl localNodeUrl = QUrl::fromUserInput(QString("127.0.0.1:%1").arg(Settings::instance().getCurrentLocalDaemonPort()));
    m_node = createRpcNode(CurrencyAdapter::instance().getCurrency(), *this, LoggerAdapter::instance().getLoggerManager(), localNodeUrl.host().toStdString(), localNodeUrl.port(), false);
    QTimer initTimer;
    initTimer.setInterval(3000);
    initTimer.setSingleShot(true);
    initTimer.start();
    m_node->init([this](std::error_code _err) {
      Q_UNUSED(_err);
    });
    QEventLoop waitLoop;
    connect(&initTimer, &QTimer::timeout, &waitLoop, &QEventLoop::quit);
    connect(this, &NodeAdapter::peerCountUpdatedSignal, &waitLoop, &QEventLoop::quit);
    connect(this, &NodeAdapter::localBlockchainUpdatedSignal, &waitLoop, &QEventLoop::quit);
    waitLoop.exec();
    if (initTimer.isActive()) {
      initTimer.stop();
      Q_EMIT nodeInitCompletedSignal();
      return true;
    }

  } else if(connection.compare("remote") == 0) {

    LoggerAdapter::instance().log("Initializing with remote node...");
    const NodeSetting nodeSetting = Settings::instance().getCurrentRemoteNode();
    m_node = createRpcNode(CurrencyAdapter::instance().getCurrency(), *this, LoggerAdapter::instance().getLoggerManager(), nodeSetting.host.toStdString(), nodeSetting.port, nodeSetting.ssl);
    QTimer initTimer;
    initTimer.setInterval(3000);
    initTimer.setSingleShot(true);
    initTimer.start();
    m_node->init([this](std::error_code _err) {
      Q_UNUSED(_err);
    });
    QEventLoop waitLoop;
    connect(&initTimer, &QTimer::timeout, &waitLoop, &QEventLoop::quit);
    connect(this, &NodeAdapter::peerCountUpdatedSignal, &waitLoop, &QEventLoop::quit);
    connect(this, &NodeAdapter::localBlockchainUpdatedSignal, &waitLoop, &QEventLoop::quit);
    waitLoop.exec();
    if (initTimer.isActive()) {
      initTimer.stop();
      Q_EMIT nodeInitCompletedSignal();
      return true;
    }

  } else {

    LoggerAdapter::instance().log("Trying to connect to local daemon...");
    QUrl localNodeUrl = QUrl::fromUserInput(QString("127.0.0.1:%1").arg(CryptoNote::RPC_DEFAULT_PORT));
    m_node = createRpcNode(CurrencyAdapter::instance().getCurrency(), *this, LoggerAdapter::instance().getLoggerManager(), localNodeUrl.host().toStdString(), localNodeUrl.port(), false);
    QTimer initTimer;
    initTimer.setInterval(3000);
    initTimer.setSingleShot(true);
    initTimer.start();
    m_node->init([this](std::error_code _err) {
      Q_UNUSED(_err);
    });
    QEventLoop waitLoop;
    connect(&initTimer, &QTimer::timeout, &waitLoop, &QEventLoop::quit);
    connect(this, &NodeAdapter::peerCountUpdatedSignal, &waitLoop, &QEventLoop::quit);
    connect(this, &NodeAdapter::localBlockchainUpdatedSignal, &waitLoop, &QEventLoop::quit);
    waitLoop.exec();
    if (initTimer.isActive()) {
      initTimer.stop();
      Q_EMIT nodeInitCompletedSignal();
      return true;
    }
    delete m_node;
    m_node = nullptr;
    LoggerAdapter::instance().log("Attempt connecting to local daemon timed out, launching builtin node...");
    return initInProcessNode();

  }

  return true;
}

quint64 NodeAdapter::getLastKnownBlockHeight() const {
  Q_CHECK_PTR(m_node);
  return m_node->getLastKnownBlockHeight();
}

quint64 NodeAdapter::getLastLocalBlockHeight() const {
  Q_CHECK_PTR(m_node);
  return m_node->getLastLocalBlockHeight();
}

QDateTime NodeAdapter::getLastLocalBlockTimestamp() const {
  Q_CHECK_PTR(m_node);
  return QDateTime::fromTime_t(m_node->getLastLocalBlockTimestamp(), Qt::UTC);
}

quint64 NodeAdapter::getDifficulty() {
  Q_CHECK_PTR(m_node);
  return m_node->getDifficulty();
}

quint64 NodeAdapter::getNextReward() {
  Q_CHECK_PTR(m_node);
  return m_node->getNextReward();
}

quint64 NodeAdapter::getTxCount() {
  Q_CHECK_PTR(m_node);
  return m_node->getTxCount();
}

quint64 NodeAdapter::getTxPoolSize() {
  Q_CHECK_PTR(m_node);
  return m_node->getTxPoolSize();
}

quint64 NodeAdapter::getAltBlocksCount() {
  Q_CHECK_PTR(m_node);
  return m_node->getAltBlocksCount();
}

quint64 NodeAdapter::getConnectionsCount() {
  Q_CHECK_PTR(m_node);
  return m_node->getConnectionsCount();
}

quint64 NodeAdapter::getOutgoingConnectionsCount() {
  Q_CHECK_PTR(m_node);
  return m_node->getOutgoingConnectionsCount();
}

quint64 NodeAdapter::getIncomingConnectionsCount() {
  Q_CHECK_PTR(m_node);
  return m_node->getIncomingConnectionsCount();
}

quint64 NodeAdapter::getWhitePeerlistSize() {
  Q_CHECK_PTR(m_node);
  return m_node->getWhitePeerlistSize();
}

quint64 NodeAdapter::getGreyPeerlistSize() {
  Q_CHECK_PTR(m_node);
  return m_node->getGreyPeerlistSize();
}

quint64 NodeAdapter::getMinimalFee() const {
  Q_CHECK_PTR(m_node);
  return m_node->getMinimalFee();
}

quint64 NodeAdapter::getNodeFeeAmount() const {
  Q_CHECK_PTR(m_node);
  return m_node->feeAmount();
}

QString NodeAdapter::getNodeFeeAddress() const {
  Q_CHECK_PTR(m_node);
  return QString::fromStdString(m_node->feeAddress());
}

CryptoNote::BlockHeaderInfo NodeAdapter::getLastLocalBlockHeaderInfo() {
  Q_CHECK_PTR(m_node);
  return m_node->getLastLocalBlockHeaderInfo();
}

uint8_t NodeAdapter::getCurrentBlockMajorVersion() {
  Q_CHECK_PTR(m_node);
  return m_node->getCurrentBlockMajorVersion();
}

quint64 NodeAdapter::getAlreadyGeneratedCoins() {
  Q_CHECK_PTR(m_node);
  return m_node->getAlreadyGeneratedCoins();
}

std::vector<CryptoNote::p2pConnection> NodeAdapter::getConnections() {
  Q_CHECK_PTR(m_node);
  return m_node->getConnections();
}

bool NodeAdapter::getBlockTemplate(CryptoNote::Block& b, const CryptoNote::AccountKeys& acc, const CryptoNote::BinaryArray& extraNonce, CryptoNote::difficulty_type& difficulty, uint32_t& height) {
  Q_CHECK_PTR(m_node);
  return m_node->getBlockTemplate(b, acc, extraNonce, difficulty, height);
}

bool NodeAdapter::handleBlockFound(CryptoNote::Block& b) {
  Q_CHECK_PTR(m_node);
  return m_node->handleBlockFound(b);
}

bool NodeAdapter::getBlockLongHash(Crypto::cn_context &context, const CryptoNote::Block& block, Crypto::Hash& res) {
  Q_CHECK_PTR(m_node);
  return m_node->getBlockLongHash(context, block, res);
}


void NodeAdapter::peerCountUpdated(Node& _node, size_t _count) {
  Q_UNUSED(_node);
  Q_EMIT peerCountUpdatedSignal(_count);
}

void NodeAdapter::localBlockchainUpdated(Node& _node, uint64_t _height) {
  Q_UNUSED(_node);
  Q_EMIT localBlockchainUpdatedSignal(_height);
}

void NodeAdapter::lastKnownBlockHeightUpdated(Node& _node, uint64_t _height) {
  Q_UNUSED(_node);
  Q_EMIT lastKnownBlockHeightUpdatedSignal(_height);
}

void NodeAdapter::connectionStatusUpdated(bool _connected) {
  Q_EMIT connectionStatusUpdatedSignal(_connected);
}


bool NodeAdapter::initInProcessNode() {
  Q_ASSERT(m_node == nullptr);
  m_nodeInitializerThread.start();
  CryptoNote::CoreConfig coreConfig = makeCoreConfig();
  CryptoNote::NetNodeConfig netNodeConfig = makeNetNodeConfig();
  CryptoNote::RpcServerConfig rpcServerConfig = makeRpcServerConfig();
  Q_EMIT initNodeSignal(&m_node, &CurrencyAdapter::instance().getCurrency(), this, &LoggerAdapter::instance().getLoggerManager(), coreConfig, netNodeConfig, rpcServerConfig);
  QEventLoop waitLoop;
  connect(m_nodeInitializer, &InProcessNodeInitializer::nodeInitCompletedSignal, &waitLoop, &QEventLoop::quit);
  connect(m_nodeInitializer, &InProcessNodeInitializer::nodeInitFailedSignal, &waitLoop, &QEventLoop::exit);
  if (waitLoop.exec() != 0) {
    return false;
  }

  Q_EMIT localBlockchainUpdatedSignal(getLastLocalBlockHeight());
  Q_EMIT lastKnownBlockHeightUpdatedSignal(getLastKnownBlockHeight());
  return true;
}

void NodeAdapter::deinit() {
  if (m_node != nullptr) {
    if (m_nodeInitializerThread.isRunning()) {
      m_nodeInitializer->stop(&m_node);
      QEventLoop waitLoop;
      connect(m_nodeInitializer, &InProcessNodeInitializer::nodeDeinitCompletedSignal, &waitLoop, &QEventLoop::quit, Qt::QueuedConnection);
      waitLoop.exec();
      m_nodeInitializerThread.quit();
      m_nodeInitializerThread.wait();
    } else {
      delete m_node;
      m_node = nullptr;
    }
  }
}

CryptoNote::CoreConfig NodeAdapter::makeCoreConfig() const {
  CryptoNote::CoreConfig config;
  boost::program_options::variables_map options;
  boost::any dataDir = std::string(Settings::instance().getDataDir().absolutePath().toLocal8Bit().data());
  options.insert(std::make_pair("data-dir", boost::program_options::variable_value(dataDir, false)));
  config.init(options);
  return config;
}

CryptoNote::NetNodeConfig NodeAdapter::makeNetNodeConfig() const {
  CryptoNote::NetNodeConfig config;
  boost::program_options::variables_map options;
  boost::any p2pBindIp = Settings::instance().getP2pBindIp().toStdString();
  boost::any p2pBindPort = static_cast<uint16_t>(Settings::instance().getP2pBindPort());
  boost::any p2pExternalPort = static_cast<uint16_t>(Settings::instance().getP2pExternalPort());
  boost::any p2pAllowLocalIp = Settings::instance().hasAllowLocalIpOption();
  boost::any dataDir = std::string(Settings::instance().getDataDir().absolutePath().toLocal8Bit().data());
  boost::any hideMyPort = Settings::instance().hasHideMyPortOption();
  boost::any connectionsCount = static_cast<uint32_t>(Settings::instance().getConnectionsCount());
  options.insert(std::make_pair("p2p-bind-ip", boost::program_options::variable_value(p2pBindIp, false)));
  options.insert(std::make_pair("p2p-bind-port", boost::program_options::variable_value(p2pBindPort, false)));
  options.insert(std::make_pair("p2p-external-port", boost::program_options::variable_value(p2pExternalPort, false)));
  options.insert(std::make_pair("allow-local-ip", boost::program_options::variable_value(p2pAllowLocalIp, false)));
  options.insert(std::make_pair("connections", boost::program_options::variable_value(connectionsCount, false)));
  std::vector<std::string> peerList = convertStringListToVector(Settings::instance().getPeers());
  if (!peerList.empty()) {
    options.insert(std::make_pair("add-peer", boost::program_options::variable_value(peerList, false)));
  }

  std::vector<std::string> priorityNodeList = convertStringListToVector(Settings::instance().getPriorityNodes());
  if (!priorityNodeList.empty()) {
    options.insert(std::make_pair("add-priority-node", boost::program_options::variable_value(priorityNodeList, false)));
  }

  std::vector<std::string> exclusiveNodeList = convertStringListToVector(Settings::instance().getExclusiveNodes());
  if (!exclusiveNodeList.empty()) {
    options.insert(std::make_pair("add-exclusive-node", boost::program_options::variable_value(exclusiveNodeList, false)));
  }

  std::vector<std::string> seedNodeList = convertStringListToVector(Settings::instance().getSeedNodes());
  if (!seedNodeList.empty()) {
    options.insert(std::make_pair("seed-node", boost::program_options::variable_value(seedNodeList, false)));
  }

  QString connection = Settings::instance().getConnection();

  if(connection.compare("remote") == 0) {
      options.insert(std::make_pair("hide-my-port", boost::program_options::variable_value(hideMyPort, true)));
  } else {
      options.insert(std::make_pair("hide-my-port", boost::program_options::variable_value(hideMyPort, false)));
  }

  options.insert(std::make_pair("data-dir", boost::program_options::variable_value(dataDir, false)));
  config.init(options);
  config.setTestnet(Settings::instance().isTestnet());
  return config;
}

CryptoNote::RpcServerConfig NodeAdapter::makeRpcServerConfig() const {
  CryptoNote::RpcServerConfig config;
  boost::filesystem::path dataDir = std::string(Settings::instance().getDataDir().absolutePath().toLocal8Bit().data());
  config.setDataDir(dataDir.string());
  boost::program_options::variables_map options;
  boost::any rpcBindIp = Settings::instance().getRpcBindIp().toStdString();
  boost::any rpcBindPort = static_cast<uint16_t>(Settings::instance().getRpcBindPort());

  options.insert(std::make_pair("rpc-bind-ip", boost::program_options::variable_value(rpcBindIp, false)));
  options.insert(std::make_pair("rpc-bind-port", boost::program_options::variable_value(rpcBindPort, false)));

  // dummy defaults
  std::string dummy = "", cors = "*";
  uint16_t sslport = CryptoNote::RPC_DEFAULT_SSL_PORT;
  bool no = false;
  options.insert(std::make_pair("rpc-bind-ssl-port", boost::program_options::variable_value(sslport, false)));
  options.insert(std::make_pair("rpc-bind-ssl-enable", boost::program_options::variable_value(no, false)));
  options.insert(std::make_pair("rpc-chain-file", boost::program_options::variable_value(dummy, false)));
  options.insert(std::make_pair("rpc-key-file", boost::program_options::variable_value(dummy, false)));
  options.insert(std::make_pair("rpc-bind-ssl-enable", boost::program_options::variable_value(dummy, false)));
  options.insert(std::make_pair("enable-cors", boost::program_options::variable_value(cors, false)));
  options.insert(std::make_pair("contact", boost::program_options::variable_value(dummy, false)));
  options.insert(std::make_pair("fee-address", boost::program_options::variable_value(dummy, false)));
  options.insert(std::make_pair("fee-amount", boost::program_options::variable_value(dummy, false)));
  options.insert(std::make_pair("view-key", boost::program_options::variable_value(dummy, false)));

  options.insert(std::make_pair("restricted-rpc", boost::program_options::variable_value(Settings::instance().hasRestrictedRpc(), false)));

  config.init(options);

  return config;
}

bool NodeAdapter::isOffline() {
  return getConnectionsCount() == 0;
}

CryptoNote::INode* NodeAdapter::getNode() {
  Q_CHECK_PTR(m_node);
  return m_node->getNode();
}

System::Dispatcher &NodeAdapter::getDispatcher() {
  return m_node->getDispatcher();
}

}

#include "NodeAdapter.moc"
