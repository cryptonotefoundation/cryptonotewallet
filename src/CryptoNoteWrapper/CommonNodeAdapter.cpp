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
#include <QThread>

#include "CommonNodeAdapter.h"
#include "BlockChainExplorerAdapter.h"
#include "InProcessNodeWorker.h"
#include "WalletGreenAdapter.h"

namespace WalletGui {

CommonNodeAdapter::CommonNodeAdapter(QObject* _parent) : QObject(_parent), m_workerThread(new QThread(this)),
  m_worker(nullptr), m_blockChainExplorerAdapter(nullptr), m_walletAdapter(nullptr) {
  qRegisterMetaType<CryptoNote::BlockHeaderInfo>("CryptoNote::BlockHeaderInfo");
}

CommonNodeAdapter::~CommonNodeAdapter() {
}

void CommonNodeAdapter::init() {
  Q_ASSERT(m_worker == nullptr);
  m_worker = createWorker();
  QObject* worker = dynamic_cast<QObject*>(m_worker);
  Q_ASSERT(worker != nullptr);
  worker->moveToThread(m_workerThread);
  m_workerThread->start();
  m_worker->addObserver(this);
  m_worker->init();
}

void CommonNodeAdapter::deinit() {
  Q_ASSERT(m_worker != nullptr);
  m_worker->deinit();
  m_worker->removeObserver(this);
  m_workerThread->quit();
  while (m_workerThread->isRunning()) {
    m_workerThread->wait(100);
    qApp->processEvents();
  }

  dynamic_cast<QObject*>(m_worker)->deleteLater();
  m_worker = nullptr;
  if (m_walletAdapter != nullptr) {
    dynamic_cast<QObject*>(m_walletAdapter)->deleteLater();
    m_walletAdapter = nullptr;
  }

  if (m_blockChainExplorerAdapter != nullptr) {
    dynamic_cast<QObject*>(m_blockChainExplorerAdapter)->deleteLater();
    m_blockChainExplorerAdapter = nullptr;
  }
}

NodeType CommonNodeAdapter::getNodeType() const {
  Q_ASSERT(m_worker != nullptr);
  return m_worker->getNodeType();
}

quintptr CommonNodeAdapter::getPeerCount() const {
  Q_ASSERT(m_worker != nullptr);
  return m_worker->getPeerCount();
}

quint32 CommonNodeAdapter::getKnownBlockCount() const {
  Q_ASSERT(m_worker != nullptr);
  return m_worker->getKnownBlockCount();
}

quint32 CommonNodeAdapter::getLocalBlockCount() const {
  Q_ASSERT(m_worker != nullptr);
  return m_worker->getLocalBlockCount();
}

CryptoNote::BlockHeaderInfo CommonNodeAdapter::getLastLocalBlockInfo() const {
  Q_ASSERT(m_worker != nullptr);
  return m_worker->getLastLocalBlockInfo();
}

void CommonNodeAdapter::addObserver(INodeAdapterObserver* _observer) {
  QObject* observer = dynamic_cast<QObject*>(_observer);
  Q_ASSERT(observer != nullptr);
  m_observerConnections[_observer] << connect(this, SIGNAL(initCompletedSignal(int)), observer, SLOT(initCompleted(int)));
  m_observerConnections[_observer] << connect(this, SIGNAL(deinitCompletedSignal()), observer, SLOT(deinitCompleted()));
  m_observerConnections[_observer] << connect(this, SIGNAL(peerCountUpdatedSignal(quintptr)), observer, SLOT(peerCountUpdated(quintptr)));
  m_observerConnections[_observer] << connect(this, SIGNAL(localBlockchainUpdatedSignal(CryptoNote::BlockHeaderInfo)), observer, SLOT(localBlockchainUpdated(CryptoNote::BlockHeaderInfo)));
  m_observerConnections[_observer] << connect(this, SIGNAL(lastKnownBlockHeightUpdatedSignal(quint32)), observer, SLOT(lastKnownBlockHeightUpdated(quint32)));
  m_observerConnections[_observer] << connect(this, SIGNAL(connectionStatusUpdatedSignal(bool)), observer, SLOT(connectionStatusUpdated(bool)));
}

void CommonNodeAdapter::removeObserver(INodeAdapterObserver* _observer) {
  if (!m_observerConnections.contains(_observer) || m_observerConnections.value(_observer).isEmpty()) {
    return;
  }

  for (const auto& connection : m_observerConnections[_observer]) {
    disconnect(connection);
  }
}

void CommonNodeAdapter::initCompleted(int _status) {
  Q_EMIT initCompletedSignal(_status);
}

void CommonNodeAdapter::deinitCompleted() {
  Q_EMIT deinitCompletedSignal();
}

void CommonNodeAdapter::peerCountUpdated(quintptr _count) {
  Q_EMIT peerCountUpdatedSignal(_count);
}

void CommonNodeAdapter::localBlockchainUpdated(CryptoNote::BlockHeaderInfo _lastLocalBlockinfo) {
  Q_EMIT localBlockchainUpdatedSignal(_lastLocalBlockinfo);
}

void CommonNodeAdapter::lastKnownBlockHeightUpdated(quint32 _height) {
  Q_EMIT lastKnownBlockHeightUpdatedSignal(_height);
}

void CommonNodeAdapter::connectionStatusUpdated(bool _connected) {
  Q_EMIT connectionStatusUpdatedSignal(_connected);
}

IBlockChainExplorerAdapter* CommonNodeAdapter::getBlockChainExplorerAdapter() {
  Q_ASSERT(m_worker != nullptr);
  if (m_blockChainExplorerAdapter == nullptr) {
    m_blockChainExplorerAdapter = m_worker->getBlockChainExplorerAdapter();
    if (m_blockChainExplorerAdapter == nullptr) {
      return nullptr;
    }

    QObject* blockChainExplorerAdapterObject = dynamic_cast<QObject*>(m_blockChainExplorerAdapter);
    blockChainExplorerAdapterObject->setParent(this);
  }

  return m_blockChainExplorerAdapter;
}

IWalletAdapter* WalletGui::CommonNodeAdapter::getWalletAdapter() {
  Q_ASSERT(m_worker != nullptr);
  if (m_walletAdapter == nullptr) {
    m_walletAdapter = m_worker->getWalletAdapter();
    QObject* walletAdapterObject = dynamic_cast<QObject*>(m_walletAdapter);
    walletAdapterObject->setParent(this);
  }

  return m_walletAdapter;
}

}
