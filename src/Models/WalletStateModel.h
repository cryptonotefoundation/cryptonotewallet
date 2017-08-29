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

#include <QAbstractItemModel>

#include "ICryptoNoteAdapter.h"
#include "IWalletAdapter.h"

namespace WalletGui {

class WalletStateModel : public QAbstractItemModel, public IWalletAdapterObserver, public ICryptoNoteAdapterObserver {
  Q_OBJECT
  Q_DISABLE_COPY(WalletStateModel)
  Q_ENUMS(Columns)

public:
  enum Columns {
    COLUMN_IS_OPEN = 0, COLUMN_IS_CLOSED, COLUMN_IS_ENCRYPTED, COLUMN_IS_NOT_ENCRYPTED, COLUMN_ADDRESS, COLUMN_ACTUAL_BALANCE,
      COLUMN_PENDING_BALANCE, COLUMN_TOTAL_BALANCE, COLUMN_IS_SYNCHRONIZED, COLUMN_IS_NOT_SYNCHRONIZED, COLUMN_ABOUT_TO_BE_SYNCHRONIZED
  };

  enum Roles {
    ROLE_IS_OPEN = Qt::UserRole, ROLE_IS_ENCRYPTED, ROLE_ADDRESS, ROLE_ACTUAL_BALANCE, ROLE_PENDING_BALANCE, ROLE_TOTAL_BALANCE,
      ROLE_IS_SYNCHRONIZED, ROLE_ABOUT_TO_BE_SYNCHRONIZED
  };

  WalletStateModel(ICryptoNoteAdapter* m_cryptoNoteAdapter, QObject* _parent);
  ~WalletStateModel();

  // QAbstractItemModel
  Qt::ItemFlags flags(const QModelIndex& _index) const override;
  int columnCount(const QModelIndex& _parent = QModelIndex()) const override;
  int rowCount(const QModelIndex& _parent = QModelIndex()) const override;
  QVariant headerData(int _section, Qt::Orientation _orientation, int _role = Qt::DisplayRole) const override;
  QVariant data(const QModelIndex& _index, int _role = Qt::DisplayRole) const override;
  QModelIndex index(int _row, int _column, const QModelIndex& _parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex& _index) const override;

  // IWalletAdapterObserver
  Q_SLOT virtual void walletOpened() override;
  Q_SLOT virtual void walletOpenError(int _initStatus) override;
  Q_SLOT virtual void walletClosed() override;
  Q_SLOT virtual void passwordChanged() override;
  Q_SLOT virtual void synchronizationProgressUpdated(quint32 _current, quint32 _total) override;
  Q_SLOT virtual void synchronizationCompleted() override;
  Q_SLOT virtual void balanceUpdated(quint64 _actualBalance, quint64 _pendingBalance) override;
  Q_SLOT virtual void externalTransactionCreated(quintptr _transactionId, const FullTransactionInfo& _transaction) override;
  Q_SLOT virtual void transactionUpdated(quintptr _transactionId, const FullTransactionInfo& _transaction) override;

  // ICryptoNoteAdapterObserver
  Q_SLOT virtual void cryptoNoteAdapterInitCompleted(int _status) override;
  Q_SLOT virtual void cryptoNoteAdapterDeinitCompleted() override;

private:
  ICryptoNoteAdapter* m_cryptoNoteAdapter;
  bool m_isOpen;
  bool m_isEncrypted;
  QString m_address;
  quint64 m_actualBalance;
  quint64 m_pendingBalance;
  quint64 m_totalBalance;
  quint32 m_currentHeight;
  quint32 m_totalHeight;
  bool m_isSynchronized;
  const int m_columnCount;

  QVariant getDisplayRole(const QModelIndex& _index) const;
  QVariant getUserRoleRole(const QModelIndex& _index, int _role) const;
};

}
