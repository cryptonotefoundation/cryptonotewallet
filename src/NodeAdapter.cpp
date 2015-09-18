// Copyright (c) 2011-2015 The Cryptonote developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QTimer>
#include <QUrl>

#include <CryptoNoteCore/CoreConfig.h>
#include <P2p/NetNodeConfig.h>
#include <Wallet/WalletErrors.h>

#include "CurrencyAdapter.h"
#include "LoggerAdapter.h"
#include "NodeAdapter.h"
#include "Settings.h"

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
    const CryptoNote::CoreConfig& _coreConfig, const CryptoNote::NetNodeConfig& _netNodeConfig) {
    (*_node) = createInprocessNode(*currency, *_loggerManager, _coreConfig, _netNodeConfig, *_callback);
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

  connect(m_nodeInitializer, &InProcessNodeInitializer::nodeInitCompletedSignal, this, &NodeAdapter::nodeInitCompletedSignal, Qt::QueuedConnection);
  connect(this, &NodeAdapter::initNodeSignal, m_nodeInitializer, &InProcessNodeInitializer::start, Qt::QueuedConnection);
  connect(this, &NodeAdapter::deinitNodeSignal, m_nodeInitializer, &InProcessNodeInitializer::stop, Qt::QueuedConnection);
}

NodeAdapter::~NodeAdapter() {
}

quintptr NodeAdapter::getPeerCount() const {
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

bool NodeAdapter::init() {
  Q_ASSERT(m_node == nullptr);
  QUrl localNodeUrl = QUrl::fromUserInput(QString("127.0.0.1:%1").arg(CryptoNote::RPC_DEFAULT_PORT));

  m_node = createRpcNode(CurrencyAdapter::instance().getCurrency(), *this, localNodeUrl.host().toStdString(), localNodeUrl.port());

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
  return initInProcessNode();
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

bool NodeAdapter::initInProcessNode() {
  Q_ASSERT(m_node == nullptr);
  m_nodeInitializerThread.start();
  CryptoNote::CoreConfig coreConfig = makeCoreConfig();
  CryptoNote::NetNodeConfig netNodeConfig = makeNetNodeConfig();
  Q_EMIT initNodeSignal(&m_node, &CurrencyAdapter::instance().getCurrency(), this, &LoggerAdapter::instance().getLoggerManager(), coreConfig, netNodeConfig);
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
  boost::any dataDir = Settings::instance().getDataDir().absolutePath().toStdString();
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
  boost::any dataDir = Settings::instance().getDataDir().absolutePath().toStdString();
  boost::any hideMyPort = Settings::instance().hasHideMyPortOption();
  options.insert(std::make_pair("p2p-bind-ip", boost::program_options::variable_value(p2pBindIp, false)));
  options.insert(std::make_pair("p2p-bind-port", boost::program_options::variable_value(p2pBindPort, false)));
  options.insert(std::make_pair("p2p-external-port", boost::program_options::variable_value(p2pExternalPort, false)));
  options.insert(std::make_pair("allow-local-ip", boost::program_options::variable_value(p2pAllowLocalIp, false)));
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

  options.insert(std::make_pair("hide-my-port", boost::program_options::variable_value(hideMyPort, false)));
  options.insert(std::make_pair("data-dir", boost::program_options::variable_value(dataDir, false)));
  int size = options.size();
  config.init(options);
  config.setTestnet(Settings::instance().isTestnet());
  return config;
}

}

#include "NodeAdapter.moc"
