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

#include "TransactionsBlockModel.h"
#include "Models/BlockchainModel.h"

namespace WalletGui {

TransactionsBlockModel::TransactionsBlockModel(QAbstractItemModel* _blockChainModel, QObject* _parent) : QSortFilterProxyModel(_parent), m_blockIndex() {
  setSourceModel(_blockChainModel);
  sort(BlockchainModel::COLUMN_TRANSACTION_TIME, Qt::DescendingOrder);
}

TransactionsBlockModel::~TransactionsBlockModel() {
}

QVariant TransactionsBlockModel::data(const QModelIndex &_index, int _role) const {
  if (!_index.isValid()) {
    return QVariant();
  }

  switch(_role) {
  case Qt::DisplayRole: {
    switch(_index.data(BlockchainModel::ROLE_COLUMN).toUInt()) {
    case BlockchainModel::COLUMN_TRANSACTION_HASH:
      return _index.data(BlockchainModel::ROLE_TRANSACTION_HASH).toByteArray().toHex();
    case BlockchainModel::COLUMN_TRANSACTION_PAYMENT_ID:
      return QSortFilterProxyModel::data(_index, _role).toString();
    default:
      break;
    }

    break;
  }

  case Qt::TextAlignmentRole:
    return headerData(_index.column(), Qt::Horizontal, _role);
  default:
    break;
  }

  return QSortFilterProxyModel::data(_index, _role);
}

void TransactionsBlockModel::setBlockIndex(const QModelIndex& _index) {
  m_blockIndex = _index;
  invalidateFilter();
}

bool TransactionsBlockModel::filterAcceptsRow(int _sourceRow, const QModelIndex &_sourceParent) const {
  if (!m_blockIndex.isValid()) {
    return false;
  }

  if (_sourceParent.isValid()) {
    return _sourceParent.row() == m_blockIndex.row();
  }

  return _sourceRow == m_blockIndex.row();
}

bool TransactionsBlockModel::filterAcceptsColumn(int _sourceColumn, const QModelIndex &_sourceParent) const
{
  return _sourceColumn == BlockchainModel::COLUMN_TRANSACTION_HASH || _sourceColumn == BlockchainModel::COLUMN_TRANSACTION_PAYMENT_ID ||
    _sourceColumn == BlockchainModel::COLUMN_TRANSACTION_FEE || _sourceColumn == BlockchainModel::COLUMN_TRANSACTION_SIZE ||
    _sourceColumn == BlockchainModel::COLUMN_TRANSACTION_OUTPUT_AMOUNT;
}

bool TransactionsBlockModel::lessThan(const QModelIndex &_left, const QModelIndex &_right) const {
  return _left.data(BlockchainModel::ROLE_TRANSACTION_TIME).toDateTime() < _right.data(BlockchainModel::ROLE_TRANSACTION_TIME).toDateTime();
}

}
