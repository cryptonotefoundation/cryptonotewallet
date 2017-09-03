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

#include <cstring>

#include "FilteredBlockModel.h"
#include "IBlockChainExplorerAdapter.h"
#include "ICryptoNoteAdapter.h"
#include "INodeAdapter.h"
#include "Models/BlockchainModel.h"

namespace WalletGui {

namespace {

const quint32 INVALID_BLOCK_HEIGHT = std::numeric_limits<quint32>::max();

}

FilteredBlockModel::FilteredBlockModel(ICryptoNoteAdapter* _cryptoNoteAdapter, QAbstractItemModel* _blockChainModel,
  QObject* _parent) : QSortFilterProxyModel(_parent), m_cryptoNoteAdapter(_cryptoNoteAdapter), m_filter(FilteredBlockModel::All),
  m_enablePeriod(false), m_beginHeight(0), m_endHeight(INVALID_BLOCK_HEIGHT) {
  setSourceModel(_blockChainModel);
}

FilteredBlockModel::~FilteredBlockModel() {
}

void FilteredBlockModel::setFilter(FilteredBlockModel::Filter _filter, bool _enablePeriod,
  const QDateTime& _begin, const QDateTime& _end, const QString &_hash) {
  IBlockChainExplorerAdapter* blockChainExplorerAdapter = m_cryptoNoteAdapter->getNodeAdapter()->getBlockChainExplorerAdapter();
  quint32 minLoadedBlockIndex = INVALID_BLOCK_HEIGHT;
  quint32 maxLoadedBlockIndex = INVALID_BLOCK_HEIGHT;
  bool needInvalidate = false;
  if(m_filter != _filter) {
    m_filter = _filter;
    needInvalidate = true;
  }

  if(m_enablePeriod != _enablePeriod) {
    m_enablePeriod = _enablePeriod;
    if (!m_enablePeriod) {
      m_begin = QDateTime();
      m_end = QDateTime();
    } else {
      m_begin = _begin;
      m_end = _end;
    }

    needInvalidate = true;
  }

  if(m_enablePeriod && (m_begin != _begin || m_end != _end)) {
    m_begin = _begin;
    m_end = _end;
    needInvalidate = true;
  }

  if(m_hash != _hash) {
    m_hash = _hash;
    needInvalidate = true;
  }

  if (needInvalidate) {
    QDateTime begin;
    QDateTime end = QDateTime::currentDateTime();
    std::vector<CryptoNote::BlockDetails> blocks;

    if (!m_hash.isEmpty()) {
      QByteArray hash = QByteArray::fromHex(m_hash.toUtf8());
      if(hash.size() == sizeof(Crypto::Hash)) {
        QVector<Crypto::Hash> hashes;
        Crypto::Hash arr;
        std::memcpy(&arr, hash.begin(), hash.size());
        hashes.append(arr);
        if (blockChainExplorerAdapter->preloadBlocks(hashes, &minLoadedBlockIndex, &maxLoadedBlockIndex) ==
          IBlockChainExplorerAdapter::PRELOAD_FAIL) {
          m_beginHeight = INVALID_BLOCK_HEIGHT;
          m_endHeight = INVALID_BLOCK_HEIGHT;
          invalidateFilter();
          return;
        }
      } else {
        bool ok;
        quint32 height = m_hash.toUInt(&ok);
        if (!ok) {
          m_beginHeight = INVALID_BLOCK_HEIGHT;
          m_endHeight = INVALID_BLOCK_HEIGHT;
          invalidateFilter();
          return;
        }

        QVector<quint32> heights;
        heights.append(height);
        if (blockChainExplorerAdapter->preloadBlocks(heights, &minLoadedBlockIndex, &maxLoadedBlockIndex) ==
          IBlockChainExplorerAdapter::PRELOAD_FAIL) {
          m_beginHeight = INVALID_BLOCK_HEIGHT;
          m_endHeight = INVALID_BLOCK_HEIGHT;
          invalidateFilter();
          return;
        }
      }
    } else if (m_enablePeriod) {
      begin = m_begin;
      end = m_end;
    } else {
      switch(_filter) {
      case FilteredBlockModel::All:
        break;
      case FilteredBlockModel::LastHour:
        begin = end.addSecs(-3600);
        break;
      case FilteredBlockModel::LastDay:
        begin = end.addDays(-1);
        break;
      case FilteredBlockModel::LastWeek:
        begin = end.addDays(-7);
        break;
      case FilteredBlockModel::LastMonth:
        begin = end.addMonths(-1);
        break;
      case FilteredBlockModel::LastYear:
        begin = end.addYears(-1);
        break;
      default:
        break;
      }
    }

    if (begin.isValid()) {
      if (blockChainExplorerAdapter->preloadBlocks(begin, end, &minLoadedBlockIndex, &maxLoadedBlockIndex) ==
        IBlockChainExplorerAdapter::PRELOAD_FAIL) {
        m_beginHeight = INVALID_BLOCK_HEIGHT;
        m_endHeight = INVALID_BLOCK_HEIGHT;
        invalidateFilter();
        return;
      }

      m_beginHeight = minLoadedBlockIndex;
      m_endHeight = maxLoadedBlockIndex;
    } else if (blocks.empty() && m_filter == FilteredBlockModel::All && !m_begin.isValid() && !m_end.isValid() &&
      m_hash.isEmpty()) {
      m_beginHeight = 0;
      m_endHeight = INVALID_BLOCK_HEIGHT;
    } else {
      m_beginHeight = minLoadedBlockIndex;
      m_endHeight = maxLoadedBlockIndex;
    }

    invalidateFilter();
  }
}

void FilteredBlockModel::resetFilter() {
  beginResetModel();
  m_filter = FilteredBlockModel::All;
  m_enablePeriod = false;
  m_begin = QDateTime();
  m_end = QDateTime();
  m_hash = QString();
  m_beginHeight = 0;
  m_endHeight = INVALID_BLOCK_HEIGHT;
  endResetModel();
}

bool FilteredBlockModel::filterAcceptsRow(int _sourceRow, const QModelIndex &_sourceParent) const {
  return !_sourceParent.isValid() && _sourceRow >= m_beginHeight && _sourceRow <= m_endHeight;
}

}
