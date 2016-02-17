// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "SortedTransactionsModel.h"
#include "TransactionsListModel.h"
#include "TransactionsModel.h"

namespace WalletGui {

TransactionsListModel::TransactionsListModel() : QSortFilterProxyModel() {
  setSourceModel(&SortedTransactionsModel::instance());
}

TransactionsListModel::~TransactionsListModel() {
}

bool TransactionsListModel::filterAcceptsColumn(int _sourceColumn, const QModelIndex& _sourceParent) const {
  quint32 column = sourceModel()->headerData(_sourceColumn, Qt::Horizontal, TransactionsModel::ROLE_COLUMN).toUInt();
  return column == TransactionsModel::COLUMN_STATE || column == TransactionsModel::COLUMN_DATE ||
    column == TransactionsModel::COLUMN_AMOUNT || column == TransactionsModel::COLUMN_ADDRESS ||
    column == TransactionsModel::COLUMN_MESSAGE || column == TransactionsModel::COLUMN_PAYMENT_ID;
}

}
