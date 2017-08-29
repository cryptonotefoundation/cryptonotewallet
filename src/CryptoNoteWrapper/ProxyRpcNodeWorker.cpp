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

#include "ProxyRpcNodeWorker.h"
#include "WalletLogger/WalletLogger.h"
#include "BlockChainExplorerAdapter.h"
#include "WalletGreenAdapter.h"

#include "NodeRpcProxy/NodeRpcProxy.h"

namespace WalletGui {

ProxyRpcNodeWorker::ProxyRpcNodeWorker(const CryptoNote::Currency& _currency, Logging::ILogger& _loggerManager, Logging::ILogger& _walletLogger,
  const QString& _nodeHost, quint16 _nodePort, QObject* _parent) : QObject(_parent), m_currency(_currency),
  m_loggerManager(_loggerManager), m_walletLogger(_walletLogger), m_nodeHost(_nodeHost), m_nodePort(_nodePort) {
}

ProxyRpcNodeWorker::~ProxyRpcNodeWorker() {
  Q_ASSERT(m_node.isNull());
}

void ProxyRpcNodeWorker::init() {
  WalletLogger::debug(tr("[RPC node] Initializing..."));
  QMetaObject::invokeMethod(this, "initImpl", Qt::QueuedConnection);
}

void ProxyRpcNodeWorker::deinit() {
  WalletLogger::debug(tr("[RPC node] Deinitializing..."));
  for (auto it = m_observerConnections.begin(); it != m_observerConnections.end(); ++it) {
    removeObserver(it.key());
  }

  m_observerConnections.clear();
  QEventLoop waitLoop;
  connect(this, &ProxyRpcNodeWorker::deinitCompletedSignal, &waitLoop, &QEventLoop::quit, Qt::QueuedConnection);
  QMetaObject::invokeMethod(this, "deinitImpl", Qt::QueuedConnection);
  waitLoop.exec();
}

NodeType ProxyRpcNodeWorker::getNodeType() const {
  return NodeType::RPC;
}

quintptr ProxyRpcNodeWorker::getPeerCount() const {
  Q_ASSERT(!m_node.isNull());
  return m_node->getPeerCount();
}

quint32 ProxyRpcNodeWorker::getKnownBlockCount() const {
  Q_ASSERT(!m_node.isNull());
  return m_node->getKnownBlockCount();
}

quint32 ProxyRpcNodeWorker::getLocalBlockCount() const {
  Q_ASSERT(!m_node.isNull());
  return m_node->getLocalBlockCount();
}

CryptoNote::BlockHeaderInfo ProxyRpcNodeWorker::getLastLocalBlockInfo() const {
  Q_ASSERT(!m_node.isNull());
  return m_node->getLastLocalBlockHeaderInfo();
}

void ProxyRpcNodeWorker::addObserver(INodeAdapterObserver* _observer) {
  QObject* observer = dynamic_cast<QObject*>(_observer);
  Q_ASSERT(observer != nullptr);
  m_observerConnections[_observer] << connect(this, SIGNAL(initCompletedSignal(int)), observer, SLOT(initCompleted(int)), Qt::QueuedConnection);
  m_observerConnections[_observer] << connect(this, SIGNAL(deinitCompletedSignal()), observer, SLOT(deinitCompleted()), Qt::QueuedConnection);
  m_observerConnections[_observer] << connect(this, SIGNAL(peerCountUpdatedSignal(quintptr)), observer, SLOT(peerCountUpdated(quintptr)), Qt::QueuedConnection);
  m_observerConnections[_observer] << connect(this, SIGNAL(localBlockchainUpdatedSignal(CryptoNote::BlockHeaderInfo)), observer, SLOT(localBlockchainUpdated(CryptoNote::BlockHeaderInfo)), Qt::QueuedConnection);
  m_observerConnections[_observer] << connect(this, SIGNAL(lastKnownBlockHeightUpdatedSignal(quint32)), observer, SLOT(lastKnownBlockHeightUpdated(quint32)), Qt::QueuedConnection);
  m_observerConnections[_observer] << connect(this, SIGNAL(connectionStatusUpdatedSignal(bool)), observer, SLOT(connectionStatusUpdated(bool)), Qt::QueuedConnection);
  WalletLogger::info(tr("[Application] observer"));

}

void ProxyRpcNodeWorker::removeObserver(INodeAdapterObserver* _observer) {
  if (!m_observerConnections.contains(_observer) || m_observerConnections.value(_observer).isEmpty()) {
    return;
  }

  for (const auto& connection : m_observerConnections[_observer]) {
    disconnect(connection);
  }

  m_observerConnections[_observer].clear();
}

IBlockChainExplorerAdapter* ProxyRpcNodeWorker::getBlockChainExplorerAdapter() {
  Q_ASSERT(!m_node.isNull());
  return nullptr;
}

IWalletAdapter* ProxyRpcNodeWorker::getWalletAdapter() {
  Q_ASSERT(!m_node.isNull());
  IWalletAdapter* walletAdapter = new WalletGreenAdapter(m_currency, *m_node, m_walletLogger, nullptr);
  return walletAdapter;
}

void ProxyRpcNodeWorker::peerCountUpdated(size_t _count) {
  WalletLogger::info(tr("[RPC node] Event: Peer count updated: %1").arg(_count));
  Q_EMIT peerCountUpdatedSignal(_count);
}

void ProxyRpcNodeWorker::localBlockchainUpdated(uint32_t _height) {
  WalletLogger::debug(tr("[RPC node] Event: Local blockchain updated: %1").arg(_height));
  Q_EMIT localBlockchainUpdatedSignal(m_node->getLastLocalBlockHeaderInfo());
}

void ProxyRpcNodeWorker::lastKnownBlockHeightUpdated(uint32_t _height) {
  WalletLogger::debug(tr("[RPC node] Event: Known blockchain updated: %1").arg(_height));
  Q_EMIT lastKnownBlockHeightUpdatedSignal(_height);
}

void ProxyRpcNodeWorker::connectionStatusUpdated(bool _connected) {
  WalletLogger::info(tr("[RPC node] Connection state: %1").arg(_connected ? tr("Connected") : tr("Disconnected")));
  Q_EMIT connectionStatusUpdatedSignal(_connected);
}

void ProxyRpcNodeWorker::initImpl() {
  Q_ASSERT(m_node.isNull());
  m_node.reset(new CryptoNote::NodeRpcProxy(m_nodeHost.toStdString(), m_nodePort, m_loggerManager));
  m_node->addObserver(static_cast<CryptoNote::INodeObserver*>(this));
  m_node->addObserver(static_cast<CryptoNote::INodeRpcProxyObserver*>(this));
  WalletLogger::debug(tr("[RPC node] NodeRpcProxy initializing..."));
  m_node->init([this](std::error_code _errorCode) {
    Q_ASSERT(_errorCode.value() == 0);
    if (_errorCode.value() == 0) {
      Q_EMIT initCompletedSignal(INodeAdapter::INIT_SUCCESS);
    } else {
      WalletLogger::critical(tr("[RPC node] NodeRpcProxy init error: %1").arg(_errorCode.message().data()));
    }

    WalletLogger::debug(tr("[RPC node] NodeRpcProxy init result: %1").arg(_errorCode.value()));
  });
}

void ProxyRpcNodeWorker::deinitImpl() {
  Q_ASSERT(!m_node.isNull());
  WalletLogger::debug(tr("[RPC node] NodeRpcProxy shutdown..."));
  m_node->removeObserver(static_cast<CryptoNote::INodeObserver*>(this));
  m_node->removeObserver(static_cast<CryptoNote::INodeRpcProxyObserver*>(this));
  m_node->shutdown();
  m_node.reset();
  Q_EMIT deinitCompletedSignal();
}

}
