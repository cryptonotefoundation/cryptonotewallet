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

#include "FilteredByPeriodTransactionsModel.h"
#include "Models/TransactionsModel.h"

namespace WalletGui {

FilteredByPeriodTransactionsModel::FilteredByPeriodTransactionsModel(QObject* _parent) : QSortFilterProxyModel(_parent),
  m_begin(), m_end() {
  setDynamicSortFilter(true);
}

FilteredByPeriodTransactionsModel::~FilteredByPeriodTransactionsModel() {
}

void FilteredByPeriodTransactionsModel::setFilter(const QDateTime& _begin, const QDateTime& _end) {
  if (m_begin != _begin || m_end != _end) {
    m_begin = _begin;
    m_end = _end;
    invalidateFilter();
  }
}

bool FilteredByPeriodTransactionsModel::filterAcceptsRow(int _sourceRow, const QModelIndex& _sourceParent) const {
  QModelIndex index = sourceModel()->index(_sourceRow, 0, _sourceParent);
  QDateTime transactionTime = index.data(TransactionsModel::ROLE_TIME).toDateTime();
  if (transactionTime.isNull()) {
    transactionTime = QDateTime::currentDateTime();
  }

  if (!m_begin.isNull() && transactionTime < m_begin) {
    return false;
  }

  if (!m_end.isNull() && transactionTime > m_end) {
    return false;
  }

  return true;
}

}
