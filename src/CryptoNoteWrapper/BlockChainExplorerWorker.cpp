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

#include <QEventLoop>
#include <QDateTime>
#include <QReadLocker>
#include <QVector>
#include <QWriteLocker>

#include <fstream>

#include "BlockChainExplorerWorker.h"
#include "Settings/Settings.h"
#include "WalletLogger/WalletLogger.h"
#include "SemaphoreUtils.h"

namespace WalletGui {

namespace {

const quint32 INVALID_BLOCK_HEIGHT = std::numeric_limits<quint32>::max();

}

BlockChainExplorerWorker::BlockChainExplorerWorker(CryptoNote::INode& _node, Logging::ILogger& _loggerManager, QObject* _parent) : QObject(_parent),
  m_blockChainExplorer(_node, _loggerManager), m_preloadSemaphore(1) {
  qRegisterMetaType<QVector<CryptoNote::BlockDetails>>("QVector<CryptoNote::BlockDetails>");
  qRegisterMetaType<QVector<CryptoNote::TransactionDetails>>("QVector<CryptoNote::TransactionDetails>");
  qRegisterMetaType<QVector<Crypto::Hash>>("QVector<Crypto::Hash>");
  qRegisterMetaType<QVector<quint32>>("QVector<quint32>");
  qRegisterMetaType<CryptoNote::BlockDetails>("CryptoNote::BlockDetails");
}

BlockChainExplorerWorker::~BlockChainExplorerWorker() {
}

IBlockChainExplorerAdapter::InitStatus BlockChainExplorerWorker::init() {
  QEventLoop waitLoop;
  connect(this, &BlockChainExplorerWorker::initCompletedSignal, &waitLoop, &QEventLoop::exit);
  WalletLogger::info(tr("[Blockchain explorer] Initializing..."));
  QMetaObject::invokeMethod(this, "initImpl", Qt::QueuedConnection);
  int status = waitLoop.exec();
  return static_cast<InitStatus>(status);
}

void BlockChainExplorerWorker::deinit() {
  for (auto it = m_observerConnections.begin(); it != m_observerConnections.end(); ++it) {
    removeObserver(it.key());
  }

  m_observerConnections.clear();
  try {
    m_blockChainExplorer.removeObserver(this);
    m_blockChainExplorer.shutdown();
  } catch (const std::exception& _error) {
    WalletLogger::critical(tr("[Blockchain explorer] Shutdown error: %1").arg(_error.what()));
  }
}

CryptoNote::BlockDetails* BlockChainExplorerWorker::getBlock(quint32 _blockIndex) {
  if (!m_blocksCache.contains(_blockIndex)) {
    SemaphoreLocker locker(m_preloadSemaphore);
    QMetaObject::invokeMethod(this, "preloadBlocksImpl", Qt::QueuedConnection, Q_ARG(quint32, _blockIndex),
      Q_ARG(quint32, _blockIndex));
    locker.wait();
  }

  return m_blocksCache.object(_blockIndex);
}

IBlockChainExplorerAdapter::PreloadBlocksStatus BlockChainExplorerWorker::preloadBlocks(quint32 _minBlockIndex, quint32 _maxBlockIndex, quint32* _minLoadedBlockIndex, quint32* _maxLoadedBlockIndex) {
  SemaphoreLocker locker(m_preloadSemaphore);
  QEventLoop waitLoop;
  QMetaObject::Connection connection = connect(this, &BlockChainExplorerWorker::blocksPreloadCompletedSignal, this,
    [_minLoadedBlockIndex, _maxLoadedBlockIndex, &waitLoop, this](int _status, quint32 _minLoadedIndex, quint32 _maxLoadedIndex) {
      if (_status == PRELOAD_FAIL) {
        if (_minLoadedBlockIndex != nullptr) {
          *_minLoadedBlockIndex = INVALID_BLOCK_HEIGHT;
        }

        if (_maxLoadedBlockIndex != nullptr) {
          *_maxLoadedBlockIndex = INVALID_BLOCK_HEIGHT;
        }
      } else {
        if (_minLoadedBlockIndex != nullptr) {
          *_minLoadedBlockIndex = _minLoadedIndex;
        }

        if (_maxLoadedBlockIndex != nullptr) {
          *_maxLoadedBlockIndex = _maxLoadedIndex;
        }
      }

      waitLoop.exit(_status);
    }, Qt::QueuedConnection);

  QMetaObject::invokeMethod(this, "preloadBlocksImpl", Qt::QueuedConnection, Q_ARG(quint32, _minBlockIndex),
    Q_ARG(quint32, _maxBlockIndex));
  int status = waitLoop.exec();
  locker.wait();
  disconnect(connection);
  return static_cast<PreloadBlocksStatus>(status);
}

IBlockChainExplorerAdapter::PreloadBlocksStatus BlockChainExplorerWorker::preloadBlocks(const QVector<quint32>& _blockIndexes, quint32* _minLoadedBlockIndex, quint32* _maxLoadedBlockIndex) {
  SemaphoreLocker locker(m_preloadSemaphore);
  QEventLoop waitLoop;
  QMetaObject::Connection connection = connect(this, &BlockChainExplorerWorker::blocksPreloadCompletedSignal, this,
    [_minLoadedBlockIndex, _maxLoadedBlockIndex, &waitLoop, this](int _status, quint32 _minLoadedIndex, quint32 _maxLoadedIndex) {
      if (_status == PRELOAD_FAIL) {
        if (_minLoadedBlockIndex != nullptr) {
          *_minLoadedBlockIndex = INVALID_BLOCK_HEIGHT;
        }

        if (_maxLoadedBlockIndex != nullptr) {
          *_maxLoadedBlockIndex = INVALID_BLOCK_HEIGHT;
        }
      } else {
        if (_minLoadedBlockIndex != nullptr) {
          *_minLoadedBlockIndex = _minLoadedIndex;
        }

        if (_maxLoadedBlockIndex != nullptr) {
          *_maxLoadedBlockIndex = _maxLoadedIndex;
        }
      }

      waitLoop.exit(_status);
    }, Qt::QueuedConnection);

  QMetaObject::invokeMethod(this, "preloadBlocksImpl", Qt::QueuedConnection, Q_ARG(QVector<quint32>, _blockIndexes));
  int status = waitLoop.exec();
  locker.wait();
  disconnect(connection);
  return static_cast<PreloadBlocksStatus>(status);
}

IBlockChainExplorerAdapter::PreloadBlocksStatus BlockChainExplorerWorker::preloadBlocks(const QDateTime& _timestampBegin,
  const QDateTime& _timestampEnd, quint32* _minLoadedBlockIndex, quint32* _maxLoadedBlockIndex) {
  SemaphoreLocker locker(m_preloadSemaphore);
  QEventLoop waitLoop;
  QMetaObject::Connection connection = connect(this, &BlockChainExplorerWorker::blocksPreloadCompletedSignal, this,
    [_minLoadedBlockIndex, _maxLoadedBlockIndex, &waitLoop, this](int _status, quint32 _minLoadedIndex, quint32 _maxLoadedIndex) {
      if (_status == PRELOAD_FAIL) {
        if (_minLoadedBlockIndex != nullptr) {
          *_minLoadedBlockIndex = INVALID_BLOCK_HEIGHT;
        }

        if (_maxLoadedBlockIndex != nullptr) {
          *_maxLoadedBlockIndex = INVALID_BLOCK_HEIGHT;
        }
      } else {
        if (_minLoadedBlockIndex != nullptr) {
          *_minLoadedBlockIndex = _minLoadedIndex;
        }

        if (_maxLoadedBlockIndex != nullptr) {
          *_maxLoadedBlockIndex = _maxLoadedIndex;
        }
      }

      waitLoop.exit(_status);
    }, Qt::QueuedConnection);

  QMetaObject::invokeMethod(this, "preloadBlocksImpl", Qt::QueuedConnection, Q_ARG(QDateTime, _timestampBegin),
    Q_ARG(QDateTime, _timestampEnd));
  int status = waitLoop.exec();
  locker.wait();
  disconnect(connection);
  return static_cast<PreloadBlocksStatus>(status);
}

IBlockChainExplorerAdapter::PreloadBlocksStatus BlockChainExplorerWorker::preloadBlocks(const QVector<Crypto::Hash>& _blockHashes, quint32* _minLoadedBlockIndex, quint32* _maxLoadedBlockIndex) {
  SemaphoreLocker locker(m_preloadSemaphore);
  QEventLoop waitLoop;
  QMetaObject::Connection connection = connect(this, &BlockChainExplorerWorker::blocksPreloadCompletedSignal, this,
    [_minLoadedBlockIndex, _maxLoadedBlockIndex, &waitLoop, this](int _status, quint32 _minLoadedIndex, quint32 _maxLoadedIndex) {
      if (_status == PRELOAD_FAIL) {
        if (_minLoadedBlockIndex != nullptr) {
          *_minLoadedBlockIndex = INVALID_BLOCK_HEIGHT;
        }

        if (_maxLoadedBlockIndex != nullptr) {
          *_maxLoadedBlockIndex = INVALID_BLOCK_HEIGHT;
        }
      } else {
        if (_minLoadedBlockIndex != nullptr) {
          *_minLoadedBlockIndex = _minLoadedIndex;
        }

        if (_maxLoadedBlockIndex != nullptr) {
          *_maxLoadedBlockIndex = _maxLoadedIndex;
        }
      }

      waitLoop.exit(_status);
    }, Qt::QueuedConnection);

  QMetaObject::invokeMethod(this, "preloadBlocksImpl", Qt::QueuedConnection, Q_ARG(QVector<Crypto::Hash>, _blockHashes));
  int status = waitLoop.exec();
  locker.wait();
  disconnect(connection);
  return static_cast<PreloadBlocksStatus>(status);
}

IBlockChainExplorerAdapter::GetPoolStatus BlockChainExplorerWorker::getPoolState(QVector<CryptoNote::TransactionDetails>& _transactions) {
  SemaphoreLocker locker(m_preloadSemaphore);
  QEventLoop waitLoop;
  QMetaObject::Connection connection = connect(this, &BlockChainExplorerWorker::getPoolStateCompleted, this,
    [&_transactions, &waitLoop](int _status, const QVector<CryptoNote::TransactionDetails>& _receivedTransactions) {
      if (_status == POOL_SUCCESS) {
        _transactions = _receivedTransactions;
      }

      waitLoop.exit(_status);
    }, Qt::QueuedConnection);

  QMetaObject::invokeMethod(this, "getPoolStateImpl", Qt::QueuedConnection);
  int status = waitLoop.exec();
  locker.wait();
  disconnect(connection);
  return static_cast<GetPoolStatus>(status);
}

bool BlockChainExplorerWorker::isBlockchainSynchronized() {
  try {
    return m_blockChainExplorer.isSynchronized();
  } catch (const std::exception& _error) {
    WalletLogger::critical(tr("[Blockchain explorer] Is synchronized query error: %1").arg(_error.what()));
  }

  return false;
}

void BlockChainExplorerWorker::addObserver(IBlockChainExplorerAdapterObserver* _observer) {
  QObject* observer = dynamic_cast<QObject*>(_observer);
  Q_ASSERT(observer != nullptr);
  m_observerConnections[_observer] << connect(this, SIGNAL(blockChainUpdatedSignal(QVector<CryptoNote::BlockDetails>,QVector<CryptoNote::BlockDetails>)),
    observer, SLOT(blockChainUpdated(QVector<CryptoNote::BlockDetails>,QVector<CryptoNote::BlockDetails>)), Qt::QueuedConnection);
  m_observerConnections[_observer] << connect(this, SIGNAL(poolUpdatedSignal(QVector<CryptoNote::TransactionDetails>,QVector<Crypto::Hash>)), observer,
    SLOT(poolUpdated(QVector<CryptoNote::TransactionDetails>,QVector<Crypto::Hash>)), Qt::QueuedConnection);
  m_observerConnections[_observer] << connect(this, SIGNAL(blockchainSynchronizedSignal(CryptoNote::BlockDetails)), observer,
    SLOT(blockchainSynchronized(CryptoNote::BlockDetails)), Qt::QueuedConnection);
}

void BlockChainExplorerWorker::removeObserver(IBlockChainExplorerAdapterObserver* _observer) {
  if (!m_observerConnections.contains(_observer) || m_observerConnections.value(_observer).isEmpty()) {
    return;
  }

  for (const auto& connection : m_observerConnections[_observer]) {
    disconnect(connection);
  }

  m_observerConnections[_observer].clear();
}

void BlockChainExplorerWorker::blockchainUpdated(const std::vector<CryptoNote::BlockDetails>& _newBlocks,
  const std::vector<CryptoNote::BlockDetails>& _orphanedBlocks) {
  WalletLogger::debug(tr("[Blockchain explorer] Event: Blockchain updated, %1 new blocks, %2 orphaned blocks").arg(_newBlocks.size()).
    arg(_orphanedBlocks.size()));
  // Do nothing
  auto newBlocks(std::move(QVector<CryptoNote::BlockDetails>::fromStdVector(_newBlocks)));
  auto orphanedBlocks(std::move(QVector<CryptoNote::BlockDetails>::fromStdVector(_orphanedBlocks)));
  Q_EMIT blockChainUpdatedSignal(newBlocks, orphanedBlocks);
}

void BlockChainExplorerWorker::poolUpdated(const std::vector<CryptoNote::TransactionDetails>& _newTransactions,
  const std::vector<std::pair<Crypto::Hash, CryptoNote::TransactionRemoveReason> >& _removedTransactions) {
  WalletLogger::debug(tr("[Blockchain explorer] Event: Pool updated, %1 new transactions, %2 removed transactions").
    arg(_newTransactions.size()).arg(_removedTransactions.size()));
  QVector<CryptoNote::TransactionDetails> newTransactions = QVector<CryptoNote::TransactionDetails>::fromStdVector(_newTransactions);
  QVector<Crypto::Hash> removedTransactions(_removedTransactions.size());
  for (quintptr i = 0; i < _removedTransactions.size(); ++i) {
    removedTransactions[i] = _removedTransactions[i].first;
  }

  Q_EMIT poolUpdatedSignal(newTransactions, removedTransactions);
}

void BlockChainExplorerWorker::blockchainSynchronized(const CryptoNote::BlockDetails& _topBlock) {
  WalletLogger::info(tr("[Blockchain explorer] Event: Blockchain synchronized, top block: %1").arg(_topBlock.index));
  Q_EMIT blockchainSynchronizedSignal(_topBlock);
}

void BlockChainExplorerWorker::initImpl() {
  int errorCode = INIT_SUCCESS;
  try {
    m_blockChainExplorer.init();
    m_blockChainExplorer.addObserver(this);
  } catch(const std::system_error& _error) {
    WalletLogger::critical(tr("[Blockchain explorer] Init error: %1 (%2)").arg(_error.code().message().data()).
      arg(_error.code().value()));
    errorCode = INIT_FAIL;
  } catch (const std::exception& _error) {
    WalletLogger::critical(tr("[Blockchain explorer] Init runtime error: %1").arg(_error.what()));
    errorCode = INIT_FAIL;
  }

  Q_EMIT initCompletedSignal(errorCode);
}

void BlockChainExplorerWorker::preloadBlocksImpl(quint32 _minBlockIndex, quint32 _maxBlockIndex) {
  SemaphoreUnlocker unlocker(m_preloadSemaphore);
  WalletLogger::debug(tr("[Blockchain explorer] Preloading blocks by interval, min=%1 max=%2").arg(_minBlockIndex).arg(_maxBlockIndex));
  CryptoNote::BlockDetails topBlock;
  try {
    if (!m_blockChainExplorer.getBlockchainTop(topBlock)) {
      Q_EMIT blocksPreloadCompletedSignal(PRELOAD_FAIL, INVALID_BLOCK_HEIGHT, INVALID_BLOCK_HEIGHT);
      return;
    }
  } catch (const std::exception& _error) {
    WalletLogger::critical(tr("[Blockchain explorer] Get blockchain top query error: %1").arg(_error.what()));
    Q_EMIT blocksPreloadCompletedSignal(PRELOAD_FAIL, INVALID_BLOCK_HEIGHT, INVALID_BLOCK_HEIGHT);
    return;
  }

  quint32 maxBlockIndex = std::min(_maxBlockIndex, topBlock.index);
  if (maxBlockIndex < _minBlockIndex) {
    WalletLogger::critical(tr("[Blockchain explorer] Preload blocks error: Incorrect interval, min=%1 max=%2").arg(_minBlockIndex).arg(maxBlockIndex));
    Q_EMIT blocksPreloadCompletedSignal(PRELOAD_FAIL, INVALID_BLOCK_HEIGHT, INVALID_BLOCK_HEIGHT);
    return;
  }

  std::vector<uint32_t> blockIndexes;
  QReadLocker readLock(&m_cacheLock);
  std::vector<std::vector<CryptoNote::BlockDetails>> blocks;
  for(quint32 i = _minBlockIndex; i < maxBlockIndex + 1; ++i) {
    if(!m_blocksCache.contains(i)) {
      blockIndexes.push_back(i);
    }
  }

  readLock.unlock();

  if(blockIndexes.empty()) {
    Q_EMIT blocksPreloadCompletedSignal(PRELOAD_FAIL, INVALID_BLOCK_HEIGHT, INVALID_BLOCK_HEIGHT);
    return;
  }

  try {
    m_blockChainExplorer.getBlocks(blockIndexes, blocks);
  } catch (const std::system_error& _error) {
    WalletLogger::critical(tr("[Blockchain explorer] Get blocks by interval error: %1 (%2)").arg(_error.code().message().data()).
      arg(_error.code().value()));
    Q_EMIT blocksPreloadCompletedSignal(PRELOAD_FAIL, INVALID_BLOCK_HEIGHT, INVALID_BLOCK_HEIGHT);
    return;
  } catch (const std::exception& _error) {
    WalletLogger::critical(tr("[Blockchain explorer] Get blocks by interval runtime error: %1").arg(_error.what()));
    Q_EMIT blocksPreloadCompletedSignal(PRELOAD_FAIL, INVALID_BLOCK_HEIGHT, INVALID_BLOCK_HEIGHT);
    return;
  }

  QWriteLocker writeLock(&m_cacheLock);
  for(quint32 i = 0; i < blocks.size(); ++i) {
    CryptoNote::BlockDetails *block = new CryptoNote::BlockDetails;
    *block = std::move(blocks[i][0]);
    m_blocksCache.insert(blockIndexes[i], block);
  }

  WalletLogger::debug(tr("[Blockchain explorer] Blocks loaded successfully"));
  Q_EMIT blocksPreloadCompletedSignal(PRELOAD_SUCCESS, _minBlockIndex, _maxBlockIndex);
}

void BlockChainExplorerWorker::preloadBlocksImpl(const QVector<quint32>& _blockIndexes) {
  SemaphoreUnlocker unlocker(m_preloadSemaphore);
  WalletLogger::debug(tr("[Blockchain explorer] Preloading blocks by indexes, indexes size=%1").arg(_blockIndexes.size()));
  std::vector<uint32_t> blockIndexes = _blockIndexes.toStdVector();
  std::vector<std::vector<CryptoNote::BlockDetails>> blocks;
  if(blockIndexes.empty()) {
    Q_EMIT blocksPreloadCompletedSignal(PRELOAD_FAIL, INVALID_BLOCK_HEIGHT, INVALID_BLOCK_HEIGHT);
    return;
  }

  try {
    m_blockChainExplorer.getBlocks(blockIndexes, blocks);
  } catch (const std::system_error& _error) {
    WalletLogger::critical(tr("[Blockchain explorer] Get blocks by indexes error: %1 (%2)").arg(_error.code().message().data()).
      arg(_error.code().value()));
    Q_EMIT blocksPreloadCompletedSignal(PRELOAD_FAIL, INVALID_BLOCK_HEIGHT, INVALID_BLOCK_HEIGHT);
    return;
  } catch (const std::exception& _error) {
    WalletLogger::critical(tr("[Blockchain explorer] Get blocks by indexes runtime error: %1").arg(_error.what()));
    Q_EMIT blocksPreloadCompletedSignal(PRELOAD_FAIL, INVALID_BLOCK_HEIGHT, INVALID_BLOCK_HEIGHT);
    return;
  }

  QWriteLocker writeLock(&m_cacheLock);
  quint32 minBlockIndex = INVALID_BLOCK_HEIGHT;
  quint32 maxBlockIndex = 0;
  for(quint32 i = 0; i < blocks.size(); ++i) {
    CryptoNote::BlockDetails *block = new CryptoNote::BlockDetails;
    *block = std::move(blocks[i][0]);
    if (block->index < minBlockIndex) {
      minBlockIndex = block->index;
    }

    if (block->index > maxBlockIndex) {
      maxBlockIndex = block->index;
    }

    m_blocksCache.insert(blockIndexes[i], block);
  }

  WalletLogger::debug(tr("[Blockchain explorer] Blocks loaded successfully"));
  Q_EMIT blocksPreloadCompletedSignal(PRELOAD_SUCCESS, minBlockIndex, maxBlockIndex);
}

void BlockChainExplorerWorker::preloadBlocksImpl(const QDateTime& _timestampBegin, const QDateTime& _timestampEnd) {
  SemaphoreUnlocker unlocker(m_preloadSemaphore);
  WalletLogger::debug(tr("[Blockchain explorer] Preloading blocks by time interval, min=%1 max=%2").arg(_timestampBegin.toString("yyyy-MM-dd hh:mm:ss")).
    arg(_timestampEnd.toString("yyyy-MM-dd hh:mm:ss")));
  quint64 timestampBegin = _timestampBegin.toTime_t();
  quint64 timestampEnd = _timestampEnd.toTime_t();
  std::vector<CryptoNote::BlockDetails> blocks;
  if(timestampBegin > timestampEnd) {
    Q_EMIT blocksPreloadCompletedSignal(PRELOAD_FAIL, INVALID_BLOCK_HEIGHT, INVALID_BLOCK_HEIGHT);
    return;
  }

  quint32 blockCountWithinTimestamps = 0;
  try {
    m_blockChainExplorer.getBlocks(timestampBegin, timestampEnd, 1, blocks, blockCountWithinTimestamps);
  } catch (const std::system_error& _error) {
    WalletLogger::critical(tr("[Blockchain explorer] Get blocks by timestamp error: %1 (%2)").arg(_error.code().message().data()).
      arg(_error.code().value()));
    Q_EMIT blocksPreloadCompletedSignal(PRELOAD_FAIL, INVALID_BLOCK_HEIGHT, INVALID_BLOCK_HEIGHT);
    return;
  } catch (const std::exception& _error) {
    WalletLogger::critical(tr("[Blockchain explorer] Get blocks by timestamp runtime error: %1").arg(_error.what()));
    Q_EMIT blocksPreloadCompletedSignal(PRELOAD_FAIL, INVALID_BLOCK_HEIGHT, INVALID_BLOCK_HEIGHT);
    return;
  }

  QWriteLocker writeLock(&m_cacheLock);
  quint32 minBlockIndex = INVALID_BLOCK_HEIGHT;
  quint32 maxBlockIndex = 0;
  if (!blocks.empty()) {
    minBlockIndex = blocks.at(0).index;
    maxBlockIndex = minBlockIndex + blockCountWithinTimestamps - 1;
  }

  WalletLogger::debug(tr("[Blockchain explorer] Blocks loaded successfully"));
  Q_EMIT blocksPreloadCompletedSignal(PRELOAD_SUCCESS, minBlockIndex, maxBlockIndex);
}

void BlockChainExplorerWorker::preloadBlocksImpl(const QVector<Crypto::Hash>& _blockHashes) {
  SemaphoreUnlocker unlocker(m_preloadSemaphore);
  WalletLogger::debug(tr("[Blockchain explorer] Preloading blocks by hashes, hashes size=%1").arg(_blockHashes.size()));
  std::vector<Crypto::Hash> blockHashes = _blockHashes.toStdVector();
  std::vector<CryptoNote::BlockDetails> blocks;
  if(blockHashes.empty()) {
    Q_EMIT blocksPreloadCompletedSignal(PRELOAD_FAIL, INVALID_BLOCK_HEIGHT, INVALID_BLOCK_HEIGHT);
    return;
  }

  try {
    m_blockChainExplorer.getBlocks(blockHashes, blocks);
  } catch (const std::system_error& _error) {
    WalletLogger::critical(tr("[Blockchain explorer] Get blocks by hashes error: %1 (%2)").arg(_error.code().message().data()).
      arg(_error.code().value()));
    Q_EMIT blocksPreloadCompletedSignal(PRELOAD_FAIL, INVALID_BLOCK_HEIGHT, INVALID_BLOCK_HEIGHT);
    return;
  } catch (const std::exception& _error) {
    WalletLogger::critical(tr("[Blockchain explorer] Get blocks by hashes runtime error: %1").arg(_error.what()));
    Q_EMIT blocksPreloadCompletedSignal(PRELOAD_FAIL, INVALID_BLOCK_HEIGHT, INVALID_BLOCK_HEIGHT);
    return;
  }

  QWriteLocker writeLock(&m_cacheLock);
  quint32 minBlockIndex = INVALID_BLOCK_HEIGHT;
  quint32 maxBlockIndex = 0;
  for(quint32 i = 0; i < blocks.size(); ++i) {
    CryptoNote::BlockDetails *block = new CryptoNote::BlockDetails;
    *block = std::move(blocks[i]);
    if (block->index < minBlockIndex) {
      minBlockIndex = block->index;
    }

    if (block->index > maxBlockIndex) {
      maxBlockIndex = block->index;
    }

    m_blocksCache.insert(block->index, block);
  }

  Q_EMIT blocksPreloadCompletedSignal(PRELOAD_SUCCESS, minBlockIndex, maxBlockIndex);
}

void BlockChainExplorerWorker::getPoolStateImpl() {
  SemaphoreUnlocker unlocker(m_preloadSemaphore);
  WalletLogger::debug(tr("[Blockchain explorer] Get pool state..."));
  QVector<CryptoNote::TransactionDetails> result;
  try {
    CryptoNote::BlockDetails topBlock;
    m_blockChainExplorer.getBlockchainTop(topBlock);
    std::vector<CryptoNote::TransactionDetails> transactions;
    std::vector<Crypto::Hash> hashes;
    std::vector<Crypto::Hash> removedTransactions;
    bool isActualBlockChain;
    if (!m_blockChainExplorer.getPoolState(hashes, topBlock.hash, isActualBlockChain, transactions, removedTransactions)) {
      WalletLogger::critical(tr("[Blockchain explorer] Get pool state: FAIL."));
      Q_EMIT getPoolStateCompleted(POOL_FAIL, result);
    } else {
      WalletLogger::debug(tr("[Blockchain explorer] Get pool state: SUCCESS."));
      result = QVector<CryptoNote::TransactionDetails>::fromStdVector(transactions);
      Q_EMIT getPoolStateCompleted(POOL_SUCCESS, result);
    }
  } catch (const std::system_error& _error) {
    WalletLogger::critical(tr("[Blockchain explorer] Get pool state error: %1 (%2)").arg(_error.code().message().data()).
      arg(_error.code().value()));
    Q_EMIT getPoolStateCompleted(POOL_FAIL, result);
  } catch (const std::exception& _error) {
    WalletLogger::critical(tr("[Blockchain explorer] Get pool state runtime error: %1").arg(_error.what()));
    Q_EMIT getPoolStateCompleted(POOL_FAIL, result);
  }
}


}
