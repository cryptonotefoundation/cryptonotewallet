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

#include <QAbstractProxyModel>

namespace WalletGui {

class IAddressBookManager;
class ICryptoNoteAdapter;

class DrawableTransfersModel : public QAbstractProxyModel {
  Q_OBJECT
  Q_DISABLE_COPY(DrawableTransfersModel)

public:
  DrawableTransfersModel(ICryptoNoteAdapter* _cryptoNoteAdapter, IAddressBookManager* _addressBookManager,
    const QString& _walletAddress, QObject* _parent);
  ~DrawableTransfersModel();

  void showDetails(bool _show);

  // QAbstractItemModel
  virtual Qt::ItemFlags flags(const QModelIndex& _index) const override;
  virtual int columnCount(const QModelIndex& _parent = QModelIndex()) const override;
  virtual int rowCount(const QModelIndex& _parent = QModelIndex()) const override;
  virtual QVariant data(const QModelIndex& _index, int _role = Qt::DisplayRole) const override;
  virtual QModelIndex index(int _row, int _column, const QModelIndex& _parent = QModelIndex()) const override;
  virtual QModelIndex parent(const QModelIndex& _index) const override;

  // QAbstractProxyModel
  virtual QModelIndex mapFromSource(const QModelIndex& _sourceIndex) const override;
  virtual QModelIndex mapToSource(const QModelIndex& _proxyIndex) const override;
  virtual void setSourceModel(QAbstractItemModel* _sourceModel) override;

private:
  ICryptoNoteAdapter* m_cryptoNoteAdapter;
  IAddressBookManager* m_addressBookManager;
  const QString m_walletAddress;
  bool m_showDetails;

  QVariant getDisplayRole(const QModelIndex& _index) const;
  QVariant getDecorationRole(const QModelIndex& _index) const;
  QVariant getTextColorRole(const QModelIndex& _index) const;
  QVariant getAggregatedDisplayRole(const QModelIndex& _index) const;
  QVariant getAggregateTextAlignmentRole(const QModelIndex& _index) const;
  QVariant getAggregateTextColorRole(const QModelIndex& _index) const;
  qint64 getAmountSum() const;
};

}
