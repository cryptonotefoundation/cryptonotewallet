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

#include "FilteredByAddressTransactionsModel.h"
#include "Models/TransactionsModel.h"

Q_DECLARE_METATYPE(QList<CryptoNote::WalletTransfer>)

namespace WalletGui {

FilteredByAddressTransactionsModel::FilteredByAddressTransactionsModel(QObject* _parent) : QSortFilterProxyModel(_parent),
  m_address() {
  setDynamicSortFilter(true);
}

FilteredByAddressTransactionsModel::~FilteredByAddressTransactionsModel() {
}

void FilteredByAddressTransactionsModel::setFilter(const QString& _address) {
  if (m_address.compare(_address, Qt::CaseInsensitive)) {
    if (_address.isEmpty()) {
      beginResetModel();
      m_address = _address;
      endResetModel();
    } else {
      m_address = _address;
      invalidateFilter();
    }
  }
}

bool FilteredByAddressTransactionsModel::filterAcceptsRow(int _sourceRow, const QModelIndex& _sourceParent) const {
  if(m_address.isEmpty()) {
    return true;
  }

  QModelIndex index = sourceModel()->index(_sourceRow, 0, _sourceParent);
  QList<CryptoNote::WalletTransfer> transfers = index.data(TransactionsModel::ROLE_TRANSFERS).value<QList<CryptoNote::WalletTransfer>>();
  if (transfers.isEmpty()) {
    return false;
  }

  for (const CryptoNote::WalletTransfer& transfer : transfers) {
    if (QString::fromStdString(transfer.address).startsWith(m_address, Qt::CaseInsensitive)) {
      return true;
    }
  }

  return false;
}

}
