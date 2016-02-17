// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <QSortFilterProxyModel>

namespace WalletGui {

class SortedDepositModel : public QSortFilterProxyModel {
  Q_OBJECT

public:
  static SortedDepositModel& instance();

protected:
  bool lessThan(const QModelIndex& _left, const QModelIndex& _right) const Q_DECL_OVERRIDE;

private:
  SortedDepositModel();
  ~SortedDepositModel();
};

}
