// Copyright (c) 2011-2015 The Cryptonote developers
// Copyright (c) 2016-2019 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <QSortFilterProxyModel>

namespace WalletGui {

class RecentSortedTransactionsModel : public QSortFilterProxyModel {
  Q_OBJECT
  Q_DISABLE_COPY(RecentSortedTransactionsModel)

public:
  static RecentSortedTransactionsModel& instance();

protected:
  bool lessThan(const QModelIndex& _left, const QModelIndex& _right) const Q_DECL_OVERRIDE;

private:
  RecentSortedTransactionsModel();
  ~RecentSortedTransactionsModel();
};



}
