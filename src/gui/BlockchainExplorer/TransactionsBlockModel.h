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

#pragma once

#include <QSortFilterProxyModel>

namespace WalletGui {

class TransactionsBlockModel : public QSortFilterProxyModel {
  Q_OBJECT
  Q_DISABLE_COPY(TransactionsBlockModel)

public:
  TransactionsBlockModel(QAbstractItemModel* _blockChainModel, QObject* _parent);
  ~TransactionsBlockModel();

  QVariant data(const QModelIndex &_index, int _role = Qt::DisplayRole) const override;
  void setBlockIndex(const QModelIndex &_index);

protected:
  bool filterAcceptsRow(int _sourceRow, const QModelIndex &_sourceParent) const override;
  bool filterAcceptsColumn(int _sourceColumn, const QModelIndex &_sourceParent) const override;
  bool lessThan(const QModelIndex &_left, const QModelIndex &_right) const override;

private:
  QPersistentModelIndex m_blockIndex;
};

}
