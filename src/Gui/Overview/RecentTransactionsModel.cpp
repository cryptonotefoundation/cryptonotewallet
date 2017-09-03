// Copyright (c) 2015-2017, The Bytecoin developers
//
// This file is part of Bytecoin.
//
// Intensecoin is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Intensecoin is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Intensecoin.  If not, see <http://www.gnu.org/licenses/>.

#include <QSize>

#include "RecentTransactionsModel.h"
#include "Settings/Settings.h"
#include "Models/SortedTransactionsModel.h"
#include "Models/TransactionsModel.h"

namespace WalletGui {

RecentTransactionsModel::RecentTransactionsModel(QAbstractItemModel* _sourceModel, QObject* _parent) : WalletWindowedItemModel(-1, _parent) {
  setSourceModel(_sourceModel);
  connect(sourceModel(), &QAbstractItemModel::rowsInserted, this, &RecentTransactionsModel::invalidateFilter);
  connect(sourceModel(), &QAbstractItemModel::rowsRemoved, this, &RecentTransactionsModel::invalidateFilter);
}

RecentTransactionsModel::~RecentTransactionsModel() {
}

bool RecentTransactionsModel::filterAcceptsColumn(int _sourceColumn, const QModelIndex& _sourceParent) const {
  return _sourceColumn == TransactionsModel::COLUMN_NEW_TRANSACTION || _sourceColumn == TransactionsModel::COLUMN_TIME ||
    _sourceColumn == TransactionsModel::COLUMN_AMOUNT || _sourceColumn == TransactionsModel::COLUMN_HASH;
}

}
