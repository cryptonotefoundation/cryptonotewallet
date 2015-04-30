// Copyright (c) 2011-2015 The Cryptonote developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "RecentTransactionsModel.h"
#include "SortedTransactionsModel.h"

namespace WalletGui {

RecentTransactionsModel::RecentTransactionsModel() : QSortFilterProxyModel() {
  setSourceModel(&SortedTransactionsModel::instance());
  setDynamicSortFilter(true);
  connect(sourceModel(), &QAbstractItemModel::rowsInserted, this, &RecentTransactionsModel::invalidateFilter);
}

RecentTransactionsModel::~RecentTransactionsModel() {
}

int RecentTransactionsModel::columnCount(const QModelIndex& _parent) const {
  return 1;
}

QVariant RecentTransactionsModel::data(const QModelIndex& _index, int _role) const {
  if(_role == Qt::DecorationRole) {
    return QVariant();
  }

  return QSortFilterProxyModel::data(_index, _role);
}

bool RecentTransactionsModel::filterAcceptsRow(int _sourceRow, const QModelIndex& _sourceParent) const {
  return _sourceRow < 6;
}


}
