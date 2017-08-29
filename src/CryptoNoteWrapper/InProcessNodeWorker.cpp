// Copyright (c) 2015-2017, The Bytecoin developers
//
// This file is part of Bytecoin.
//
// Intensecoin is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Intensecoin is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Intensecoin.  If not, see <http://www.gnu.org/licenses/>.

#include <QCoreApplication>
#include <QDateTime>
#include <QEventLoop>
#include <QSemaphore>

#include <boost/program_options.hpp>
#include <boost/any.hpp>

#include <functional>

#include "InProcessNodeWorker.h"
#include "GuardExecutor.h"
#include "Settings/Settings.h"
#include "WalletLogger/WalletLogger.h"
#include "BlockChainExplorerAdapter.h"
#include "WalletGreenAdapter.h"

#include "CryptoNoteCore/Core.h"
#include "CryptoNoteCore/CryptoNoteTools.h"
#include "CryptoNoteCore/DatabaseBlockchainCache.h"
#include "CryptoNoteCore/DatabaseBlockchainCacheFactory.h"
#include "CryptoNoteCore/DataBaseErrors.h"
#include "CryptoNoteCore/MainChainStorage.h"
#include "CryptoNoteCore/RocksDBWrapper.h"
#include "CryptoNoteProtocol/CryptoNoteProtocolHandler.h"
#include "InProcessNode/InProcessNode.h"
#include "Logging/LoggerManager.h"
#include "P2p/NetNode.h"
#include "P2p/NetNodeConfig.h"
#include "System/Dispatcher.h"
#include "System/Event.h"

namespace WalletGui {

namespace {

std::vector<std::string> convertStringListToVector(const QStringList& list) {
  std::vector<std::string> result;
  for (const QString& item : list) {
    result.push_back(item.toStdString());
  }

  return result;
}

CryptoNote::NetNodeConfig makeNetNodeConfig() {
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
  config.init(options);
  config.setTestnet(Settings::instance().isTestnet());
  return config;
}

}

InProcessNodeWorker::InProcessNodeWorker(const CryptoNote::Currency& _currency, Logging::ILogger& _loggerManager,
  Logging::ILogger& _walletLogger, QObject* _parent) : QObject(_parent), m_currency(_currency), m_loggerManager(_loggerManager), m_walletLogger(_walletLogger),
  m_blockchainExplorerAdapter(nullptr) {
}

InProcessNodeWorker::~InProcessNodeWorker() {
}

void InProcessNodeWorker::init() {
  WalletLogger::debug(tr("[Embedded node] Initializing..."));
  QMetaObject::invokeMethod(this, "initImpl", Qt::QueuedConnection);
}

void InProcessNodeWorker::deinit() {
  WalletLogger::debug(tr("[Embedded node] Deinitializing..."));
  for (auto it = m_observerConnections.begin(); it != m_observerConnections.end(); ++it) {
    removeObserver(it.key());
  }

  m_observerConnections.clear();
  deinitImpl();
}

NodeType InProcessNodeWorker::getNodeType() const {
  return NodeType::IN_PROCESS;
}

quintptr InProcessNodeWorker::getPeerCount() const {
  Q_ASSERT(!m_node.isNull());
  try {
    return m_node->getPeerCount();
  } catch (const std::exception& _error) {
    WalletLogger::critical(tr("[Embedded node] Get peer count error: %1").arg(_error.what()));
  }

  return 0;
}

quint32 InProcessNodeWorker::getKnownBlockCount() const {
  Q_ASSERT(!m_node.isNull());
  try {
    return m_node->getKnownBlockCount();
  } catch (const std::exception& _error) {
    WalletLogger::critical(tr("[Embedded node] Get known block count error: %1").arg(_error.what()));
  }

  return 0;
}

quint32 InProcessNodeWorker::getLocalBlockCount() const {
  Q_ASSERT(!m_node.isNull());
  try {
    return m_node->getLocalBlockCount();
  } catch (const std::exception& _error) {
    WalletLogger::critical(tr("[Embedded node] Get local block count error: %1").arg(_error.what()));
  }

  return 0;
}

CryptoNote::BlockHeaderInfo InProcessNodeWorker::getLastLocalBlockInfo() const {
  Q_ASSERT(!m_node.isNull());
  try {
    return m_node->getLastLocalBlockHeaderInfo();
  } catch (const std::exception& _error) {
    WalletLogger::critical(tr("[Embedded node] Get last local block timestamp error: %1").arg(_error.what()));
  }

  return CryptoNote::BlockHeaderInfo();
}

void InProcessNodeWorker::addObserver(INodeAdapterObserver* _observer) {
  QObject* observer = dynamic_cast<QObject*>(_observer);
  Q_ASSERT(observer != nullptr);
  m_observerConnections[_observer] << connect(this, SIGNAL(initCompletedSignal(int)), observer, SLOT(initCompleted(int)), Qt::QueuedConnection);
  m_observerConnections[_observer] << connect(this, SIGNAL(deinitCompletedSignal()), observer, SLOT(deinitCompleted()), Qt::QueuedConnection);
  m_observerConnections[_observer] << connect(this, SIGNAL(peerCountUpdatedSignal(quintptr)), observer, SLOT(peerCountUpdated(quintptr)), Qt::QueuedConnection);
  m_observerConnections[_observer] << connect(this, SIGNAL(localBlockchainUpdatedSignal(CryptoNote::BlockHeaderInfo)), observer, SLOT(localBlockchainUpdated(CryptoNote::BlockHeaderInfo)), Qt::QueuedConnection);
  m_observerConnections[_observer] << connect(this, SIGNAL(lastKnownBlockHeightUpdatedSignal(quint32)), observer, SLOT(lastKnownBlockHeightUpdated(quint32)), Qt::QueuedConnection);
}

void InProcessNodeWorker::removeObserver(INodeAdapterObserver* _observer) {
  if (!m_observerConnections.contains(_observer) || m_observerConnections.value(_observer).isEmpty()) {
    return;
  }

  for (const auto& connection : m_observerConnections[_observer]) {
    disconnect(connection);
  }

  m_observerConnections[_observer].clear();
}

WalletGui::IBlockChainExplorerAdapter* InProcessNodeWorker::getBlockChainExplorerAdapter() {
  Q_ASSERT(!m_node.isNull());
  return m_blockchainExplorerAdapter;
}

IWalletAdapter* InProcessNodeWorker::getWalletAdapter() {
  Q_ASSERT(!m_node.isNull());
  IWalletAdapter* walletAdapter = new WalletGreenAdapter(m_currency, *m_node, m_walletLogger, nullptr);
  return walletAdapter;
}

void InProcessNodeWorker::peerCountUpdated(size_t _count) {
  WalletLogger::info(tr("[Embedded node] Event: Peer count updated: %1").arg(_count));
  Q_EMIT peerCountUpdatedSignal(_count);
}

void InProcessNodeWorker::localBlockchainUpdated(uint32_t _height) {
  WalletLogger::debug(tr("[Embedded node] Event: Local blockchain updated: %1").arg(_height));
  Q_EMIT localBlockchainUpdatedSignal(m_node->getLastLocalBlockHeaderInfo());
}

void InProcessNodeWorker::lastKnownBlockHeightUpdated(uint32_t _height) {
  WalletLogger::debug(tr("[Embedded node] Event: Known blockchain updated: %1").arg(_height));
  Q_EMIT lastKnownBlockHeightUpdatedSignal(_height);
}

void InProcessNodeWorker::initImpl() {
  GuardExecutor executor(nullptr, [this]() {
    m_node.reset();
    m_nodeServer.reset();
    m_protocolHandler.reset();
    m_core.reset();
    m_dispatcher.reset();
    m_database.reset();
  });

  InitStatus status = initCore();
  if (status != INIT_SUCCESS) {
    Q_EMIT initCompletedSignal(status);
    return;
  }

  status = initNodeServer();
  if (status != INIT_SUCCESS) {
    m_core->save();
    Q_EMIT initCompletedSignal(status);
    return;
  }

  status = initInProcessNode();
  if (status != INIT_SUCCESS) {
    m_nodeServer->deinit();
    m_core->save();
    Q_EMIT initCompletedSignal(status);
    return;
  }

  BlockChainExplorerAdapter* blockchainExplorerAdapter = new BlockChainExplorerAdapter(*m_node, m_loggerManager, nullptr);
  blockchainExplorerAdapter->moveToThread(qApp->thread());
  m_blockchainExplorerAdapter = blockchainExplorerAdapter;

  Q_EMIT initCompletedSignal(INIT_SUCCESS);
  m_nodeServer->run();
  try {
    m_node->removeObserver(this);
  } catch (const std::exception& _error) {
    WalletLogger::critical(tr("[Embedded node] Remove observer error: %1").arg(_error.what()));
  }

  m_dispatcher->yield();
  m_node->shutdown();
  m_nodeServer->deinit();
  m_core->save();
  m_database->shutdown();
  m_dispatcher->yield();
  Q_EMIT deinitCompletedSignal();
}

void InProcessNodeWorker::deinitImpl() {
  if (!m_nodeServer.isNull()) {
    m_nodeServer->sendStopSignal();
  }
}

INodeAdapter::InitStatus InProcessNodeWorker::initCore() {
  Q_ASSERT(m_core.isNull());
  WalletLogger::debug(tr("[Embedded node] Core initializing..."));
  try {
    CryptoNote::Checkpoints checkpoints(m_loggerManager);
    if (!Settings::instance().isTestnet()) {
      for (const CryptoNote::CheckpointData& checkpoint : CryptoNote::CHECKPOINTS) {
        checkpoints.addCheckpoint(checkpoint.index, checkpoint.blockId);
      }
    }

    CryptoNote::DataBaseConfig dbConfig;

    dbConfig.setDataDir(Settings::instance().getDataDir().absolutePath().toStdString());
    dbConfig.setReadCacheSize(128 * 1024 * 1024);
    dbConfig.setWriteBufferSize(256 * 1024 * 1024);
    dbConfig.setTestnet(Settings::instance().isTestnet());

    QString blocksFilePath = Settings::instance().getDataDir().absoluteFilePath(QString::fromStdString(m_currency.blocksFileName()));
    QString indexesFilePath = Settings::instance().getDataDir().absoluteFilePath(QString::fromStdString(m_currency.blockIndexesFileName()));
    std::unique_ptr<CryptoNote::IMainChainStorage> mainChainStorage(new CryptoNote::MainChainStorage(blocksFilePath.toStdString(), indexesFilePath.toStdString()));
    if (mainChainStorage->getBlockCount() == 0) {
      CryptoNote::RawBlock genesis;
      genesis.block = CryptoNote::toBinaryArray(m_currency.genesisBlock());
      mainChainStorage->pushBlock(genesis);
    }

    m_dispatcher.reset(new System::Dispatcher);
    m_database.reset(new CryptoNote::RocksDBWrapper(m_loggerManager));
    try {
      m_database->init(dbConfig);
      if (!CryptoNote::DatabaseBlockchainCache::checkDBSchemeVersion(*m_database, m_loggerManager))
      {
        m_database->shutdown();
        m_database->destoy(dbConfig);
        m_database->init(dbConfig);
      }
    } catch (const std::system_error& _error) {
      m_database.reset();
      m_dispatcher.reset();
      if (_error.code().value() == static_cast<int>(CryptoNote::error::DataBaseErrorCodes::IO_ERROR)) {
        return INIT_DB_IN_USAGE;
      }

      return INIT_DB_FAILED;
    } catch (const std::exception& _error) {
      m_database.reset();
      m_dispatcher.reset();
      return INIT_DB_FAILED;
    }


    m_core.reset(new CryptoNote::Core(m_currency, m_loggerManager, std::move(checkpoints), *m_dispatcher,
      std::unique_ptr<CryptoNote::IBlockchainCacheFactory>(new CryptoNote::DatabaseBlockchainCacheFactory(*m_database, m_loggerManager)),
      std::move(mainChainStorage)));
    m_core->load();
    m_protocolHandler.reset(new CryptoNote::CryptoNoteProtocolHandler(m_currency, *m_dispatcher, *m_core, nullptr, m_loggerManager));
    m_nodeServer.reset(new CryptoNote::NodeServer(*m_dispatcher, *m_protocolHandler, m_loggerManager));
    m_node.reset(new CryptoNote::InProcessNode(*m_core, *m_protocolHandler, *m_dispatcher));
    m_protocolHandler->set_p2p_endpoint(m_nodeServer.data());
  } catch (const std::runtime_error& _error) {
    WalletLogger::critical(tr("Core init error: %1").arg(_error.what()));
    return INIT_CORE_FAILED;
  }

  return INIT_SUCCESS;
}

INodeAdapter::InitStatus InProcessNodeWorker::initNodeServer() {
  try {
    WalletLogger::debug(tr("[Embedded node] Node server initializing..."));
    if (!m_nodeServer->init(makeNetNodeConfig())) {
      WalletLogger::critical(tr("[Embedded node] Node server init error"));
      return INIT_NET_NODE_FAILED;
    }
  } catch (const std::runtime_error& _error) {
    WalletLogger::critical(tr("[Embedded node] Node server init error: %1").arg(_error.what()));
    return INIT_P2P_BIND_FAILED;
  }

  return INIT_SUCCESS;
}

INodeAdapter::InitStatus InProcessNodeWorker::initInProcessNode() {
  QEventLoop waitLoop;
  connect(this, &InProcessNodeWorker::nodeInitCompletedSignal, &waitLoop, &QEventLoop::exit, Qt::QueuedConnection);
  WalletLogger::info(tr("[Embedded node] InProcessNode initializing..."));
  std::error_code initResult;
  m_node->init([this, &initResult](std::error_code _errorCode) {
    WalletLogger::info(tr("[Embedded node] InProcessNode init result: %1").arg(_errorCode.value()));
    initResult = _errorCode;
    Q_EMIT nodeInitCompletedSignal(_errorCode.value());
  });

  WalletLogger::info(tr("[Embedded node] Waiting..."));
  waitLoop.exec();
  if (initResult) {
    return INIT_NODE_FAILED;
  }

  try {
    m_node->addObserver(this);
  } catch (const std::exception& _error) {
    WalletLogger::critical(tr("[Embedded node] InProcessNode add observer error: %1").arg(_error.what()));
    m_node->shutdown();
    return INIT_NODE_FAILED;
  }

  return INIT_SUCCESS;
}

}
