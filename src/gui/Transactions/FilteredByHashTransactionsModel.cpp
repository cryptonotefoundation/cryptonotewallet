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

#include "FilteredByHashTransactionsModel.h"
#include "Models/TransactionsModel.h"

namespace WalletGui {

FilteredByHashTransactionsModel::FilteredByHashTransactionsModel(QObject* _parent) : QSortFilterProxyModel(_parent),
  m_hash() {
  setDynamicSortFilter(true);
  setFilterRole(TransactionsModel::ROLE_HASH);
}

FilteredByHashTransactionsModel::~FilteredByHashTransactionsModel() {
}

void FilteredByHashTransactionsModel::setFilter(const QString& _hash) {
  if (m_hash.compare(_hash, Qt::CaseInsensitive)) {
    if (_hash.isEmpty()) {
      beginResetModel();
      m_hash = _hash;
      endResetModel();
    } else {
      m_hash = _hash;
      invalidateFilter();
    }
  }
}

bool FilteredByHashTransactionsModel::filterAcceptsRow(int _sourceRow, const QModelIndex& _sourceParent) const {
  if(m_hash.isEmpty()) {
    return true;
  }

  QModelIndex index = sourceModel()->index(_sourceRow, 0, _sourceParent);
  QString transactionHash = index.data(TransactionsModel::ROLE_HASH).toByteArray().toHex();
  return transactionHash.startsWith(m_hash, Qt::CaseInsensitive);
}

}
