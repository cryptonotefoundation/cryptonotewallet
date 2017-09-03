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

#include "WindowedBlockModel.h"
#include "IBlockChainExplorerAdapter.h"
#include "ICryptoNoteAdapter.h"
#include "Models/BlockchainModel.h"

Q_DECLARE_METATYPE(WalletGui::BlockchainModel::Type)

namespace WalletGui {

namespace {

const quint32 INVALID_BLOCK_HEIGHT = std::numeric_limits<quint32>::max();

}

WindowedBlockModel::WindowedBlockModel(ICryptoNoteAdapter* _cryptoNoteAdapter, QAbstractItemModel* _blockChainModel,
  QObject* _parent) : WalletWindowedItemModel(BlockchainModel::ROLE_BLOCK_HEIGHT, _parent),
  m_cryptoNoteAdapter(_cryptoNoteAdapter) {
  setSourceModel(new FilteredBlockModel(m_cryptoNoteAdapter, _blockChainModel, this));
  setDynamicSortFilter(true);
  sort(BlockchainModel::COLUMN_BLOCK_HEIGHT, Qt::DescendingOrder);
  connect(sourceModel(), &QAbstractItemModel::rowsInserted, this, &WindowedBlockModel::blocksAdded);
  connect(sourceModel(), &QAbstractItemModel::rowsRemoved, this, &WindowedBlockModel::blocksRemoved);
}

WindowedBlockModel::~WindowedBlockModel() {
}

Qt::ItemFlags WindowedBlockModel::flags(const QModelIndex &_index) const {
  return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemNeverHasChildren;
}

void WindowedBlockModel::setWindowSize(quint32 _newWindowSize) {
  quint32 windowBegin = getWindowBegin();
  quint32 oldWindowSize = getWindowSize();
  if (_newWindowSize < oldWindowSize) {
    WalletWindowedItemModel::setWindowSize(_newWindowSize);
    WalletWindowedItemModel::setWindowBegin(windowBegin + (oldWindowSize - _newWindowSize));
    return;
  } else if (oldWindowSize < _newWindowSize) {
    quint32 lowerThreshold = sourceModel()->index(0, 0).data(BlockchainModel::ROLE_BLOCK_HEIGHT).toUInt();
    quint32 upperThreshold = sourceModel()->index(sourceModel()->rowCount() - 1, 0).data(BlockchainModel::ROLE_BLOCK_HEIGHT).toUInt();
    quint32 sizeDiff = _newWindowSize - oldWindowSize;
    if (lowerThreshold + sizeDiff > windowBegin) {
      windowBegin = lowerThreshold;
    } else {
      windowBegin = windowBegin - sizeDiff;
    }

    quint32 newWindowEnd = windowBegin + _newWindowSize - 1;
    if (upperThreshold < newWindowEnd) {
      newWindowEnd = upperThreshold;
    }

    WalletWindowedItemModel::setWindowSize(newWindowEnd - windowBegin + 1);
    WalletWindowedItemModel::setWindowBegin(windowBegin);
  }
}

void WindowedBlockModel::nextPage() {
  quint32 windowBegin = getWindowBegin();
  quint32 windowSize = getWindowSize();
  quint32 upperThreshold = sourceModel()->index(sourceModel()->rowCount() - 1, 0).data(BlockchainModel::ROLE_BLOCK_HEIGHT).toUInt();

  quint32 nextWindowBegin = windowBegin + windowSize;
  quint32 nextWindowEnd = nextWindowBegin + windowSize - 1;
  if (upperThreshold < nextWindowEnd) {
    nextWindowEnd = upperThreshold;
    nextWindowBegin = nextWindowEnd - windowSize + 1;
  }

  IBlockChainExplorerAdapter* blockChainExplorerAdapter = m_cryptoNoteAdapter->getNodeAdapter()->getBlockChainExplorerAdapter();
  if (blockChainExplorerAdapter != nullptr) {
    blockChainExplorerAdapter->preloadBlocks(nextWindowBegin, nextWindowEnd);
    setWindowBegin(nextWindowBegin);
  }
}

void WindowedBlockModel::prevPage() {
  quint32 windowBegin = getWindowBegin();
  quint32 windowSize = getWindowSize();
  quint32 lowerThreshold = sourceModel()->index(0, 0).data(BlockchainModel::ROLE_BLOCK_HEIGHT).toUInt();

  quint32 prevWindowBegin = windowBegin;
  if (lowerThreshold + windowSize > windowBegin) {
    prevWindowBegin = lowerThreshold;
  } else {
    prevWindowBegin = windowBegin - windowSize;
  }

  IBlockChainExplorerAdapter* blockChainExplorerAdapter = m_cryptoNoteAdapter->getNodeAdapter()->getBlockChainExplorerAdapter();
  if (blockChainExplorerAdapter != nullptr) {
    blockChainExplorerAdapter->preloadBlocks(prevWindowBegin, prevWindowBegin + windowSize - 1);
    setWindowBegin(prevWindowBegin);
  }
}

void WindowedBlockModel::lastPage() {
  quint32 windowBegin = getWindowBegin();
  quint32 windowSize = getWindowSize();
  if (windowSize == 0) {
    return;
  }

  quint32 lowerThreshold = sourceModel()->index(0, 0).data(BlockchainModel::ROLE_BLOCK_HEIGHT).toUInt();
  quint32 upperThreshold = sourceModel()->index(sourceModel()->rowCount() - 1, 0).data(BlockchainModel::ROLE_BLOCK_HEIGHT).toUInt();

  quint32 windowEnd = upperThreshold;
  if (upperThreshold < lowerThreshold + windowSize - 1) {
    windowBegin = lowerThreshold;
  } else {
    windowBegin = windowEnd - windowSize + 1;
  }

  IBlockChainExplorerAdapter* blockChainExplorerAdapter = m_cryptoNoteAdapter->getNodeAdapter()->getBlockChainExplorerAdapter();
  if (blockChainExplorerAdapter != nullptr) {
    blockChainExplorerAdapter->preloadBlocks(windowBegin, windowEnd);
    WindowedBlockModel::setWindowBegin(windowBegin);
  }
}

QPair<quint32, quint32> WindowedBlockModel::prevPageInterval() const {
  quint32 windowBegin = getWindowBegin();
  quint32 windowSize = getWindowSize();
  quint32 lowerThreshold = sourceModel()->index(0, 0).data(BlockchainModel::ROLE_BLOCK_HEIGHT).toUInt();
  if (windowBegin == lowerThreshold) {
    return qMakePair(INVALID_BLOCK_HEIGHT, INVALID_BLOCK_HEIGHT);
  }

  quint32 prevWindowBegin = windowBegin;
  if (lowerThreshold + windowSize > windowBegin) {
    prevWindowBegin = lowerThreshold;
  } else {
    prevWindowBegin = windowBegin - windowSize;
  }

  return qMakePair(prevWindowBegin, prevWindowBegin + windowSize - 1);
}

QPair<quint32, quint32> WindowedBlockModel::nextPageInterval() const {
  quint32 windowBegin = getWindowBegin();
  quint32 windowSize = getWindowSize();
  quint32 upperThreshold = sourceModel()->index(sourceModel()->rowCount() - 1, 0).data(BlockchainModel::ROLE_BLOCK_HEIGHT).toUInt();
  if (upperThreshold < windowBegin + windowSize) {
    return qMakePair(INVALID_BLOCK_HEIGHT, INVALID_BLOCK_HEIGHT);
  }

  quint32 nextWindowBegin = windowBegin + windowSize;
  quint32 nextWindowEnd = std::min(nextWindowBegin + windowSize - 1, upperThreshold);
  return qMakePair(nextWindowBegin, nextWindowEnd);
}

void WindowedBlockModel::setFilter(FilteredBlockModel::Filter _filter, bool _enablePeriod, const QDateTime &_begin, const QDateTime &_end,
  const QString &_hash) {
  static_cast<FilteredBlockModel*>(sourceModel())->setFilter(_filter, _enablePeriod, _begin, _end, _hash);
}

void WindowedBlockModel::resetFilter() {
  static_cast<FilteredBlockModel*>(sourceModel()) -> resetFilter();
  lastPage();
}

bool WindowedBlockModel::filterAcceptsColumn(int _sourceColumn, const QModelIndex &_sourceParent) const {
  Q_UNUSED(_sourceParent);
  return _sourceColumn == BlockchainModel::COLUMN_BLOCK_HEIGHT || _sourceColumn == BlockchainModel::COLUMN_BLOCK_TIME ||
      _sourceColumn == BlockchainModel::COLUMN_BLOCK_SIZE || _sourceColumn == BlockchainModel::COLUMN_BLOCK_TRANSACTION_COUNT ||
      _sourceColumn == BlockchainModel::COLUMN_BLOCK_HASH;
}

bool WindowedBlockModel::lessThan(const QModelIndex &_left, const QModelIndex &_right) const {
  if (!_left.isValid() || !_right.isValid() || _left.data(BlockchainModel::ROLE_ITEM_TYPE).value<BlockchainModel::Type>() != BlockchainModel::TYPE_BLOCK ||
     _right.data(BlockchainModel::ROLE_ITEM_TYPE).value<BlockchainModel::Type>() != BlockchainModel::TYPE_BLOCK) {
    return false;
  }

  return _left.data(BlockchainModel::ROLE_BLOCK_HEIGHT).value<quint32>() < _right.data(BlockchainModel::ROLE_BLOCK_HEIGHT).value<quint32>();
}

void WindowedBlockModel::blocksAdded(const QModelIndex &_parent, int _first, int _last) {
  Q_EMIT pageCountUpdated();
}

void WindowedBlockModel::blocksRemoved(const QModelIndex &_parent, int _first, int _last) {
  Q_EMIT pageCountUpdated();
}

}
