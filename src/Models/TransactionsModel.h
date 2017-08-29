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
#include <QBitArray>
#include <QSet>

#include "IWallet.h"

#include "ICryptoNoteAdapter.h"
#include "INodeAdapter.h"
#include "IOptimizationManager.h"
#include "IWalletAdapter.h"

namespace WalletGui {

typedef QPair<quintptr, quintptr> TransactionTransferPair;

class TransactionsModel : public QAbstractItemModel, public IWalletAdapterObserver,
  public ICryptoNoteAdapterObserver, public IOptimizationManagerObserver {
  Q_OBJECT
  Q_DISABLE_COPY(TransactionsModel)
  Q_ENUMS(Columns)
  Q_ENUMS(Roles)

public:
  enum Columns {
    COLUMN_NEW_TRANSACTION = 0, COLUMN_TIME, COLUMN_HASH, COLUMN_AMOUNT, COLUMN_TRANSFERS, COLUMN_PAYMENT_ID,
    COLUMN_TYPE, COLUMN_HEIGHT, COLUMN_FEE, COLUMN_STATE, COLUMN_SHOW_TRANSFERS
  };

  enum Roles {
    ROLE_TIME = Qt::UserRole, ROLE_RAW_TIME, ROLE_TYPE, ROLE_HASH, ROLE_TRANSFER_COUNT, ROLE_AMOUNT, ROLE_PAYMENT_ID,
    ROLE_TRANSACTION_ID, ROLE_HEIGHT, ROLE_FEE, ROLE_NUMBER_OF_CONFIRMATIONS, ROLE_COLUMN, ROLE_ROW, ROLE_STATE,
    ROLE_IS_FUSION_TRANSACTION, ROLE_SHOW_TRANSFERS, ROLE_TRANSFERS
  };

  TransactionsModel(ICryptoNoteAdapter* _cryptoNoteAdapter, IOptimizationManager* _optimizationManager,
    QAbstractItemModel* _nodeStateModel, QObject* _parent);
  ~TransactionsModel();

  // QAbstractItemModel
  Qt::ItemFlags flags(const QModelIndex& _index) const override;
  int columnCount(const QModelIndex& _parent = QModelIndex()) const override;
  int rowCount(const QModelIndex& _parent = QModelIndex()) const override;
  QVariant headerData(int _section, Qt::Orientation _orientation, int _role = Qt::DisplayRole) const override;
  QVariant data(const QModelIndex& _index, int _role = Qt::DisplayRole) const override;
  QModelIndex index(int _row, int _column, const QModelIndex& _parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex& _index) const override;
  bool setData(const QModelIndex& _index, const QVariant& _value, int _role) override;

  // IWalletAdapterObserver
  Q_SLOT virtual void walletOpened() override;
  Q_SLOT virtual void walletOpenError(int _initStatus) override;
  Q_SLOT virtual void walletClosed() override;
  Q_SLOT virtual void passwordChanged() override;
  Q_SLOT virtual void synchronizationProgressUpdated(quint32 _current, quint32 _total) override;
  Q_SLOT virtual void synchronizationCompleted() override;
  Q_SLOT virtual void balanceUpdated(quint64 _actualBalance, quint64 _pendingBalance) override;
  Q_SLOT virtual void externalTransactionCreated(quintptr _transactionIndex, const FullTransactionInfo& _transaction) override;
  Q_SLOT virtual void transactionUpdated(quintptr _transactionIndex, const FullTransactionInfo& _transaction) override;

  // ICryptoNoteAdapterObserver
  Q_SLOT virtual void cryptoNoteAdapterInitCompleted(int _status) override;
  Q_SLOT virtual void cryptoNoteAdapterDeinitCompleted() override;

  // IOptimizationManagerObserver
  Q_SLOT virtual void fusionTransactionsVisibilityChanged(bool _isVisible) override;

  QByteArray toCsv() const;
  static int findProxyColumn(QAbstractItemModel* _proxyModel, int _originColumn);

protected:
  virtual void timerEvent(QTimerEvent* _event) override;

private:
  ICryptoNoteAdapter* m_cryptoNoteAdapter;
  QAbstractItemModel* m_nodeStateModel;
  const int m_columnCount;
  QBitArray m_showTransfers;
  QHash<quintptr, FullTransactionInfo> m_transactions;
  QSet<quintptr> m_fusionTransactions;
  int m_syncTimerId;
  quintptr m_lastVisibleTransactionIndex;
  bool m_isSynchronized;

  void nodeStateChanged(const QModelIndex& _topLeft, const QModelIndex& _bottomRight, const QVector<int>& _roles);
  void ensureSyncTimerStarted();
  void ensureSyncTimerStopped();
  QVariant getDisplayRoleData(const QModelIndex &_index) const;
  QVariant getEditRoleData(const QModelIndex &_index) const;
  QVariant getDecorationRoleData(const QModelIndex &_index) const;
  QVariant getToolTipRole(const QModelIndex& _index) const;
  QVariant getUserRolesData(const QModelIndex &_index, int _role) const;

  Q_INVOKABLE void showAllCachedTransactions();
  Q_INVOKABLE void updateTransaction(quintptr _transactionIndex, const FullTransactionInfo& _transaction);
};

}
