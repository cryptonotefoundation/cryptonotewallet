// Copyright (c) 2011-2015 The Cryptonote developers
// Copyright (c) 2016-2019 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QDateTime>

#include "RecentSortedTransactionsModel.h"
#include "TransactionsModel.h"
#include "Settings.h"

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
  if (Settings::instance().skipFusionTransactions()
          && (_left.data(TransactionsModel::ROLE_TYPE).value<quint8>() == 4
              || _right.data(TransactionsModel::ROLE_TYPE).value<quint8>() == 4)) {
    return false;
  }

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
