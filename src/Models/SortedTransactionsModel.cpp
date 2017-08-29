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

#include <QDateTime>

#include "SortedTransactionsModel.h"
#include "TransactionsModel.h"

namespace WalletGui {

SortedTransactionsModel::SortedTransactionsModel(QAbstractItemModel* _sourceModel, QObject* _parent) : QSortFilterProxyModel(_parent) {
  setSourceModel(_sourceModel);
  setDynamicSortFilter(true);
  setSortRole(TransactionsModel::ROLE_RAW_TIME);
  sort(TransactionsModel::COLUMN_TIME, Qt::DescendingOrder);
}

SortedTransactionsModel::~SortedTransactionsModel() {
}

bool SortedTransactionsModel::lessThan(const QModelIndex& _left, const QModelIndex& _right) const {
  quint64 leftDate = _left.data(TransactionsModel::ROLE_RAW_TIME).value<quint64>();
  quint64 rightDate = _right.data(TransactionsModel::ROLE_RAW_TIME).value<quint64>();
  if (rightDate == 0 && leftDate == 0) {
    return _left.row() < _right.row();
  }

  if (leftDate == 0) {
    return false;
  }

  if (rightDate == 0) {
    return true;
  }

  return leftDate < rightDate;
}

}
