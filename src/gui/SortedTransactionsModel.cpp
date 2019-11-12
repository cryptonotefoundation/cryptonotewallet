// Copyright (c) 2011-2015 The Cryptonote developers
// Copyright (c) 2016-2019 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QDateTime>

#include "SortedTransactionsModel.h"
#include "TransactionsModel.h"
#include "Settings.h"

namespace WalletGui {

SortedTransactionsModel& SortedTransactionsModel::instance() {
  static SortedTransactionsModel inst;
  return inst;
}

// Earliest date that can be represented (far in the past)
const QDateTime SortedTransactionsModel::MIN_DATE = QDateTime::fromTime_t(0);
// Last date that can be represented (far in the future)
const QDateTime SortedTransactionsModel::MAX_DATE = QDateTime::fromTime_t(0xFFFFFFFF);

SortedTransactionsModel::SortedTransactionsModel() : QSortFilterProxyModel() {
  setSourceModel(&TransactionsModel::instance());
  setDynamicSortFilter(true);
  sort(TransactionsModel::COLUMN_DATE, Qt::DescendingOrder);
}

SortedTransactionsModel::~SortedTransactionsModel() {
}

bool SortedTransactionsModel::filterAcceptsRow(int _row, const QModelIndex &_parent) const {
  QModelIndex _index = sourceModel()->index(_row, 0, _parent);

  QDateTime datetime = _index.data(TransactionsModel::ROLE_DATE).toDateTime();

  if(datetime < dateFrom || datetime > dateTo)
    return false;

  int txType = _index.data(TransactionsModel::ROLE_TYPE).value<quint8>();

  if(selectedtxtype != -1) {
    if(txType != selectedtxtype)
      return false;
  }

  if (Settings::instance().skipFusionTransactions() && txType == 4) {
    return false;
  }

  QModelIndex index2 = sourceModel()->index(_row, 2, _parent);
  QModelIndex index3 = sourceModel()->index(_row, 3, _parent);
  QModelIndex index4 = sourceModel()->index(_row, 4, _parent);
  QModelIndex index5 = sourceModel()->index(_row, 5, _parent);

  return (sourceModel()->data(index2).toString().contains(searchstring,Qt::CaseInsensitive)
       || sourceModel()->data(index3).toString().contains(searchstring,Qt::CaseInsensitive)
       || sourceModel()->data(index4).toString().contains(searchstring,Qt::CaseInsensitive)
       || sourceModel()->data(index5).toString().contains(searchstring,Qt::CaseInsensitive));

  return true;
 }

bool SortedTransactionsModel::lessThan(const QModelIndex& _left, const QModelIndex& _right) const {
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

void SortedTransactionsModel::setDateRange(const QDateTime &from, const QDateTime &to)
{
  this->dateFrom = from;
  this->dateTo = to;
  invalidateFilter();
}

void SortedTransactionsModel::setSearchFor(const QString &searchstring) {
    this->searchstring = searchstring;
    invalidateFilter();
}

void SortedTransactionsModel::setTxType(const int type) {
    this->selectedtxtype = type;
    invalidateFilter();
}

}
