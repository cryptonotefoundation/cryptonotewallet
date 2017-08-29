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

#pragma once

#include <QObject>

#include "IBlockChainExplorerAdapter.h"

namespace Logging {
  class ILogger;
}

namespace CryptoNote {
  class IDataBase;
  class INode;
}

namespace WalletGui {

class BlockChainExplorerAdapter : public QObject, public IBlockChainExplorerAdapter, public IBlockChainExplorerAdapterObserver {
  Q_OBJECT
  Q_DISABLE_COPY(BlockChainExplorerAdapter)

public:
  BlockChainExplorerAdapter(CryptoNote::INode& _node, Logging::ILogger& _loggerManager, QObject* _parent);
  virtual ~BlockChainExplorerAdapter();

  // IBlockChainExplorerAdapter
  virtual InitStatus init() override;
  virtual void deinit() override;
  virtual CryptoNote::BlockDetails* getBlock(quint32 _blockIndex) override;
  virtual PreloadBlocksStatus preloadBlocks(quint32 _minBlockIndex, quint32 _maxBlockIndex, quint32* _minLoadedBlockIndex = nullptr, quint32* _maxLoadedBlockIndex = nullptr) override;
  virtual PreloadBlocksStatus preloadBlocks(const QVector<quint32>& _blockIndexes, quint32* _minLoadedBlockIndex = nullptr, quint32* _maxLoadedBlockIndex = nullptr) override;
  virtual PreloadBlocksStatus preloadBlocks(const QDateTime& _timestampBegin, const QDateTime& _timestampEnd, quint32* _minLoadedBlockIndex = nullptr, quint32* _maxLoadedBlockIndex = nullptr) override;
  virtual PreloadBlocksStatus preloadBlocks(const QVector<Crypto::Hash>& _blockHashes, quint32* _minLoadedBlockIndex = nullptr, quint32* _maxLoadedBlockIndex = nullptr) override;
  virtual GetPoolStatus getPoolState(QVector<CryptoNote::TransactionDetails>& _transactions) override;
  virtual bool isBlockchainSynchronized() override;

  virtual void addObserver(IBlockChainExplorerAdapterObserver* _observer) override;
  virtual void removeObserver(IBlockChainExplorerAdapterObserver* _observer) override;

  // IBlockChainExplorerAdapterObserver
  Q_SLOT virtual void blockChainUpdated(const QVector<CryptoNote::BlockDetails>& _newBlocks,
    const QVector<CryptoNote::BlockDetails>& _orphanedBlocks) override;
  Q_SLOT virtual void poolUpdated(const QVector<CryptoNote::TransactionDetails>& _newTransactions,
    const QVector<Crypto::Hash>& _removedTransactions) override;
  Q_SLOT virtual void blockchainSynchronized(const CryptoNote::BlockDetails& _topBlock) override;

private:
  QThread* m_workerThread;
  IBlockChainExplorerAdapter* m_worker;

Q_SIGNALS:
  void blockChainUpdatedSignal(const QVector<CryptoNote::BlockDetails>& _newBlocks,
    const QVector<CryptoNote::BlockDetails>& _orphanedBlocks);
  void poolUpdatedSignal(const QVector<CryptoNote::TransactionDetails>& _newTransactions,
    const QVector<Crypto::Hash>& _removedTransactions);
  void blockchainSynchronizedSignal(const CryptoNote::BlockDetails& _topBlock);
};

}
