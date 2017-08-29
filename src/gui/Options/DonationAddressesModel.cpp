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

#include "DonationAddressesModel.h"
#include "Models/AddressBookModel.h"

namespace WalletGui {

DonationAddressesModel::DonationAddressesModel(QAbstractItemModel* _sourceModel, QObject* _parent) : QSortFilterProxyModel(_parent) {
  setSourceModel(_sourceModel);
  setDynamicSortFilter(true);
}

DonationAddressesModel::~DonationAddressesModel() {
}

QVariant DonationAddressesModel::data(const QModelIndex& _index, int _role) const {
  if (_role == Qt::DisplayRole) {
    return QString("%1 (%2)").arg(_index.data(AddressBookModel::ROLE_LABEL).toString()).
      arg(_index.data(AddressBookModel::ROLE_ADDRESS).toString());
  }

  return QSortFilterProxyModel::data(_index, _role);
}

bool DonationAddressesModel::filterAcceptsRow(int _sourceRow, const QModelIndex& _sourceParent) const {
  QModelIndex sourceIndex = sourceModel()->index(_sourceRow, 0, _sourceParent);
  return sourceIndex.data(AddressBookModel::ROLE_IS_DONATION_ADDRESS).toBool();
}

bool DonationAddressesModel::filterAcceptsColumn(int _sourceColumn, const QModelIndex& _sourceParent) const {
  return _sourceColumn == AddressBookModel::COLUMN_LABEL;
}

}
