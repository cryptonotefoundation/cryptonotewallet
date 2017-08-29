// Copyright (c) 2015-2017, The Bytecoin developers
//
// This file is part of Bytecoin.
//
// Chavezcoin is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Chavezcoin is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Chavezcoin.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <QObject>

class QDateTime;

namespace Crypto {
  struct Hash;
}

namespace CryptoNote {
  struct BlockDetails;
  struct TransactionDetails;
}

namespace WalletGui {

class IBlockChainExplorerAdapterObserver {
public:
  virtual ~IBlockChainExplorerAdapterObserver() {}
  virtual void blockChainUpdated(const QVector<CryptoNote::BlockDetails>& _newBlocks,
    const QVector<CryptoNote::BlockDetails>& _orphanedBlocks) = 0;
  virtual void poolUpdated(const QVector<CryptoNote::TransactionDetails>& _newTransactions,
    const QVector<Crypto::Hash>& _removedTransactions) = 0;
  virtual void blockchainSynchronized(const CryptoNote::BlockDetails& _topBlock) = 0;
};

class IBlockChainExplorerAdapter {
public:
  enum InitStatus {
    INIT_SUCCESS = 0, INIT_FAIL
  };

  enum PreloadBlocksStatus {
    PRELOAD_SUCCESS = 0, PRELOAD_FAIL
  };

  enum GetPoolStatus {
    POOL_SUCCESS = 0, POOL_FAIL
  };

  virtual ~IBlockChainExplorerAdapter() {}

  virtual InitStatus init() = 0;
  virtual void deinit() = 0;

  virtual CryptoNote::BlockDetails* getBlock(quint32 _blockIndex) = 0;
  virtual PreloadBlocksStatus preloadBlocks(quint32 _minBlockIndex, quint32 _maxBlockIndex, quint32* _minLoadedBlockIndex = nullptr, quint32* _maxLoadedBlockIndex = nullptr) = 0;
  virtual PreloadBlocksStatus preloadBlocks(const QVector<quint32>& _blockIndexes, quint32* _minLoadedBlockIndex = nullptr, quint32* _maxLoadedBlockIndex = nullptr) = 0;
  virtual PreloadBlocksStatus preloadBlocks(const QDateTime& _timestampBegin, const QDateTime& _timestampEnd, quint32* _minLoadedBlockIndex = nullptr, quint32* _maxLoadedBlockIndex = nullptr) = 0;
  virtual PreloadBlocksStatus preloadBlocks(const QVector<Crypto::Hash>& _blockHashes, quint32* _minLoadedBlockIndex = nullptr, quint32* _maxLoadedBlockIndex = nullptr) = 0;
  virtual GetPoolStatus getPoolState(QVector<CryptoNote::TransactionDetails>& _transactions) = 0;
  virtual bool isBlockchainSynchronized() = 0;

  virtual void addObserver(IBlockChainExplorerAdapterObserver* _observer) = 0;
  virtual void removeObserver(IBlockChainExplorerAdapterObserver* _observer) = 0;
};

}
