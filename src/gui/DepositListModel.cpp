// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "DepositListModel.h"
#include "DepositModel.h"
#include "SortedDepositModel.h"

namespace WalletGui {

DepositListModel::DepositListModel() : QSortFilterProxyModel() {
  setSourceModel(&SortedDepositModel::instance());
}

DepositListModel::~DepositListModel() {
}

bool DepositListModel::filterAcceptsColumn(int _sourceColumn, const QModelIndex& _sourceParent) const {
  return _sourceColumn == DepositModel::COLUMN_STATE || _sourceColumn == DepositModel::COLUMN_AMOUNT || _sourceColumn == DepositModel::COLUMN_INTEREST ||
    _sourceColumn == DepositModel::COLUMN_SUM || _sourceColumn == DepositModel::COLUMN_YEAR_RATE || _sourceColumn == DepositModel::COLUMN_UNLOCK_HEIGHT ||
    _sourceColumn == DepositModel::COLUMN_SPENDING_TIME;
}

}
