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

#include <QSharedPointer>
#include <QThread>

#include "BlockChainExplorerAdapter.h"
#include "BlockChainExplorerWorker.h"

namespace WalletGui {

BlockChainExplorerAdapter::BlockChainExplorerAdapter(CryptoNote::INode& _node, Logging::ILogger& _loggerManager, QObject* _parent) : QObject(_parent), m_workerThread(new QThread(this)),
  m_worker(new BlockChainExplorerWorker(_node, _loggerManager, nullptr)) {
  QObject* worker = dynamic_cast<QObject*>(m_worker);
  Q_ASSERT(worker != nullptr);
  worker->moveToThread(m_workerThread);
  m_workerThread->start();
}

BlockChainExplorerAdapter::~BlockChainExplorerAdapter() {
  m_workerThread->quit();
  m_workerThread->wait();
}

IBlockChainExplorerAdapter::InitStatus BlockChainExplorerAdapter::init() {
  m_worker->addObserver(this);
  return m_worker->init();
}

void BlockChainExplorerAdapter::deinit() {
  m_worker->deinit();
  m_worker->removeObserver(this);
}

CryptoNote::BlockDetails* BlockChainExplorerAdapter::getBlock(quint32 _blockIndex) {
  return m_worker->getBlock(_blockIndex);
}

IBlockChainExplorerAdapter::PreloadBlocksStatus BlockChainExplorerAdapter::preloadBlocks(quint32 _minBlockIndex, quint32 _maxBlockIndex, quint32* _minLoadedBlockIndex, quint32* _maxLoadedBlockIndex) {
  return m_worker->preloadBlocks(_minBlockIndex, _maxBlockIndex, _minLoadedBlockIndex, _maxLoadedBlockIndex);
}

IBlockChainExplorerAdapter::PreloadBlocksStatus BlockChainExplorerAdapter::preloadBlocks(const QVector<quint32>& _blockIndexes, quint32* _minLoadedBlockIndex, quint32* _maxLoadedBlockIndex) {
  return m_worker->preloadBlocks(_blockIndexes, _minLoadedBlockIndex, _maxLoadedBlockIndex);
}

IBlockChainExplorerAdapter::PreloadBlocksStatus BlockChainExplorerAdapter::preloadBlocks(const QDateTime& _timestampBegin, const QDateTime& _timestampEnd, quint32* _minLoadedBlockIndex, quint32* _maxLoadedBlockIndex) {
  return m_worker->preloadBlocks(_timestampBegin, _timestampEnd, _minLoadedBlockIndex, _maxLoadedBlockIndex);
}

IBlockChainExplorerAdapter::PreloadBlocksStatus BlockChainExplorerAdapter::preloadBlocks(const QVector<Crypto::Hash>& _blockHashes, quint32* _minLoadedBlockIndex, quint32* _maxLoadedBlockIndex) {
  return m_worker->preloadBlocks(_blockHashes, _minLoadedBlockIndex, _maxLoadedBlockIndex);
}

IBlockChainExplorerAdapter::GetPoolStatus BlockChainExplorerAdapter::getPoolState(QVector<CryptoNote::TransactionDetails>& _transactions) {
  return m_worker->getPoolState(_transactions);
}

bool BlockChainExplorerAdapter::isBlockchainSynchronized() {
  return m_worker->isBlockchainSynchronized();
}

void BlockChainExplorerAdapter::addObserver(IBlockChainExplorerAdapterObserver* _observer) {
  QObject* observer = dynamic_cast<QObject*>(_observer);
  connect(this, SIGNAL(blockChainUpdatedSignal(QVector<CryptoNote::BlockDetails>,QVector<CryptoNote::BlockDetails>)),
    observer, SLOT(blockChainUpdated(QVector<CryptoNote::BlockDetails>,QVector<CryptoNote::BlockDetails>)), Qt::DirectConnection);
  connect(this, SIGNAL(poolUpdatedSignal(QVector<CryptoNote::TransactionDetails>,QVector<Crypto::Hash>)), observer,
    SLOT(poolUpdated(QVector<CryptoNote::TransactionDetails>,QVector<Crypto::Hash>)), Qt::DirectConnection);
  connect(this, SIGNAL(blockchainSynchronizedSignal(CryptoNote::BlockDetails)), observer,
    SLOT(blockchainSynchronized(CryptoNote::BlockDetails)), Qt::DirectConnection);
}

void BlockChainExplorerAdapter::removeObserver(IBlockChainExplorerAdapterObserver* _observer) {
  QObject* observer = dynamic_cast<QObject*>(_observer);
  disconnect(this, SIGNAL(blockChainUpdatedSignal(QVector<CryptoNote::BlockDetails>,QVector<CryptoNote::BlockDetails>)),
    observer, SLOT(blockChainUpdated(QVector<CryptoNote::BlockDetails>,QVector<CryptoNote::BlockDetails>)));
  disconnect(this, SIGNAL(poolUpdatedSignal(QVector<CryptoNote::TransactionDetails>,QVector<Crypto::Hash>)), observer,
    SLOT(poolUpdated(QVector<CryptoNote::TransactionDetails>,QVector<Crypto::Hash>)));
  disconnect(this, SIGNAL(blockchainSynchronizedSignal(CryptoNote::BlockDetails)), observer,
    SLOT(blockchainSynchronized(CryptoNote::BlockDetails)));
}

void BlockChainExplorerAdapter::blockChainUpdated(const QVector<CryptoNote::BlockDetails>& _newBlocks,
  const QVector<CryptoNote::BlockDetails>& _orphanedBlocks) {
  Q_EMIT blockChainUpdatedSignal(_newBlocks, _orphanedBlocks);
}

void BlockChainExplorerAdapter::poolUpdated(const QVector<CryptoNote::TransactionDetails>& _newTransactions,
  const QVector<Crypto::Hash>& _removedTransactions) {
  Q_EMIT poolUpdatedSignal(_newTransactions, _removedTransactions);
}

void BlockChainExplorerAdapter::blockchainSynchronized(const CryptoNote::BlockDetails& _topBlock) {
  Q_EMIT blockchainSynchronizedSignal(_topBlock);
}

}
