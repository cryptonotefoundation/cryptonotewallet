// Copyright (c) 2011-2015 The Cryptonote developers
// Copyright (c) 2016-2017 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QDateTime>

#include "SortedAddressBookModel.h"
#include "AddressBookModel.h"

namespace WalletGui {

SortedAddressBookModel& SortedAddressBookModel::instance() {
  static SortedAddressBookModel inst;
  return inst;
}

SortedAddressBookModel::SortedAddressBookModel() : QSortFilterProxyModel() {
  setSourceModel(&AddressBookModel::instance());
  setDynamicSortFilter(true);
  sort(AddressBookModel::COLUMN_LABEL, Qt::DescendingOrder);
}

SortedAddressBookModel::~SortedAddressBookModel() {
}

bool SortedAddressBookModel::filterAcceptsRow(int _row, const QModelIndex &_parent) const {
  QModelIndex index0 = sourceModel()->index(_row, 0, _parent);
  QModelIndex index1 = sourceModel()->index(_row, 1, _parent);
  QModelIndex index2 = sourceModel()->index(_row, 2, _parent);

  return (sourceModel()->data(index0).toString().contains(searchstring)
       || sourceModel()->data(index1).toString().contains(searchstring)
       || sourceModel()->data(index2).toString().contains(searchstring));

  return true;
 }

void SortedAddressBookModel::setSearchFor(const QString &searchstring) {
    this->searchstring = searchstring;
    invalidateFilter();
}

}
