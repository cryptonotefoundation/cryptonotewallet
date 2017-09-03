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

class WalletWindowedItemModel : public QSortFilterProxyModel {
  Q_OBJECT
  Q_DISABLE_COPY(WalletWindowedItemModel)

public:
  WalletWindowedItemModel(int _filterRole, QObject* _parent);
  virtual ~WalletWindowedItemModel();

  quint32 getWindowSize() const;
  quint32 getWindowBegin() const;
  virtual void setWindowSize(quint32 _windowSize);
  virtual void setWindowBegin(quint32 _windowBegin);

protected:
  virtual bool filterAcceptsRow(int _sourceRow, const QModelIndex &_sourceParent) const override;

private:
  int m_filterRole;
  quint32 m_windowSize;
  quint32 m_windowBegin;

};

}
