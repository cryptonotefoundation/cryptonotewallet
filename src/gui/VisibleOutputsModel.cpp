// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Copyright (c) 2016-2021 Karbo developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "VisibleOutputsModel.h"
#include "OutputsModel.h"
#include "SortedOutputsModel.h"

namespace WalletGui {

VisibleOutputsModel::VisibleOutputsModel() : QSortFilterProxyModel() {
  setSourceModel(&SortedOutputsModel::instance());
}

VisibleOutputsModel::~VisibleOutputsModel() {
}

bool VisibleOutputsModel::filterAcceptsColumn(int _sourceColumn, const QModelIndex& _sourceParent) const {
  return _sourceColumn == OutputsModel::COLUMN_STATE ||
         _sourceColumn == OutputsModel::COLUMN_TYPE ||
         _sourceColumn == OutputsModel::COLUMN_OUTPUT_KEY ||
         _sourceColumn == OutputsModel::COLUMN_TX_HASH ||
         _sourceColumn == OutputsModel::COLUMN_AMOUNT ||
         _sourceColumn == OutputsModel::COLUMN_GLOBAL_OUTPUT_INDEX ||
         _sourceColumn == OutputsModel::COLUMN_OUTPUT_IN_TRANSACTION ||
         _sourceColumn == OutputsModel::COLUMN_SPENDING_BLOCK_HEIGHT;
}

}
