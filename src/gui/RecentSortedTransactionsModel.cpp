// Copyright (c) 2011-2015 The Cryptonote developers
// Copyright (c) 2016-2017 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QDateTime>

#include "RecentSortedTransactionsModel.h"
#include "TransactionsModel.h"

namespace WalletGui {

RecentSortedTransactionsModel& RecentSortedTransactionsModel::instance() {
  static RecentSortedTransactionsModel inst;
  return inst;
}

RecentSortedTransactionsModel::RecentSortedTransactionsModel() : QSortFilterProxyModel() {
  setSourceModel(&TransactionsModel::instance());
  setDynamicSortFilter(true);
  sort(TransactionsModel::COLUMN_DATE, Qt::DescendingOrder);
}

RecentSortedTransactionsModel::~RecentSortedTransactionsModel() {
}

bool RecentSortedTransactionsModel::lessThan(const QModelIndex& _left, const QModelIndex& _right) const {
  QDateTime leftDate = _left.data(TransactionsModel::ROLE_DATE).toDateTime();
  QDateTime rightDate = _right.data(TransactionsModel::ROLE_DATE).toDateTime();
  if((rightDate.isNull() || !rightDate.isValid()) && (leftDate.isNull() || !leftDate.isValid())) {
    return _left.row() < _right.row();
  }

  if(leftDate.isNull() || !leftDate.isValid()) {
    return false;
  }

  if(rightDate.isNull() || !rightDate.isValid()) {
    return true;
  }

  return leftDate < rightDate;
}

}
