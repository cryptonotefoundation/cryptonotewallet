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

#include "Gui/Common/WalletWindowedItemModel.h"
#include "FilteredBlockModel.h"

namespace WalletGui {

class WindowedBlockModel : public WalletWindowedItemModel {
  Q_OBJECT
  Q_DISABLE_COPY(WindowedBlockModel)

public:
  WindowedBlockModel(ICryptoNoteAdapter* _cryptoNoteAdapter, QAbstractItemModel* _blockChainModel, QObject* _parent);
  ~WindowedBlockModel();

  Qt::ItemFlags flags(const QModelIndex &_index) const override;

  void setWindowSize(quint32 _newWindowSize) override;
  void nextPage();
  void prevPage();
  void lastPage();
  QPair<quint32, quint32> prevPageInterval() const;
  QPair<quint32, quint32> nextPageInterval() const;

  void setFilter(FilteredBlockModel::Filter _filter, bool _enablePeriod, const QDateTime &_begin, const QDateTime &_end,
    const QString &_hash);
  void resetFilter();

protected:
  bool filterAcceptsColumn(int _sourceColumn, const QModelIndex &_sourceParent) const override;
  bool lessThan(const QModelIndex &_left, const QModelIndex &_right) const override;

private:
  ICryptoNoteAdapter* m_cryptoNoteAdapter;

  void blocksAdded(const QModelIndex &_parent, int _first, int _last);
  void blocksRemoved(const QModelIndex &_parent, int _first, int _last);

Q_SIGNALS:
  void pageCountUpdated();
};

}
