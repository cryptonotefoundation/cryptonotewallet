// Copyright (c) 2011-2015 The Cryptonote developers
// Copyright (c) 2016-2017 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <QDateTime>
#include <QSortFilterProxyModel>

namespace WalletGui {

class SortedAddressBookModel : public QSortFilterProxyModel {
  Q_OBJECT
  Q_DISABLE_COPY(SortedAddressBookModel)

public:
  static SortedAddressBookModel& instance();
  void setSearchFor(const QString &searchstring);

protected:
  bool filterAcceptsRow(int _row, const QModelIndex &_parent) const Q_DECL_OVERRIDE;

private:
  SortedAddressBookModel();
  ~SortedAddressBookModel();

  QString searchstring;
};

}
