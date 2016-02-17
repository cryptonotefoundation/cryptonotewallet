// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <QAbstractItemModel>
#include <QSortFilterProxyModel>

#include <IWalletLegacy.h>

namespace WalletGui {

class DepositModel : public QAbstractItemModel {
  Q_OBJECT
  Q_ENUMS(Columns)
  Q_ENUMS(Roles)
  Q_ENUMS(DepositState)

public:
  enum Columns{
    COLUMN_STATE = 0, COLUMN_AMOUNT, COLUMN_INTEREST, COLUMN_SUM, COLUMN_YEAR_RATE, COLUMN_TERM, COLUMN_UNLOCK_HEIGHT,
    COLUMN_UNLOCK_TIME, COLUMN_CREATRING_TRANSACTION_HASH, COLUMN_CREATING_HEIGHT, COLUMN_CREATING_TIME,
    COLUMN_SPENDING_TRANSACTION_HASH, COLUMN_SPENDING_HEIGHT, COLUMN_SPENDING_TIME
  };

  enum Roles {
    ROLE_DEPOSIT_TERM = Qt::UserRole, ROLE_DEPOSIT_AMOUNT, ROLE_DEPOSIT_INTEREST, ROLE_STATE,
    ROLE_CREATING_TRANSACTION_ID, ROLE_SPENDING_TRANSACTION_ID, ROLE_UNLOCK_HEIGHT, ROLE_ROW, ROLE_COLUMN
  };

  enum DepositState {
    STATE_LOCKED, STATE_UNLOCKED, STATE_SPENT
  };

  static DepositModel& instance();

  Qt::ItemFlags flags(const QModelIndex& _index) const Q_DECL_OVERRIDE;
  int columnCount(const QModelIndex& _parent = QModelIndex()) const Q_DECL_OVERRIDE;
  int rowCount(const QModelIndex& _parent = QModelIndex()) const Q_DECL_OVERRIDE;

  QVariant headerData(int _section, Qt::Orientation _orientation, int _role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
  QVariant data(const QModelIndex& _index, int _role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
  QModelIndex index(int _row, int _column, const QModelIndex& _parent = QModelIndex()) const Q_DECL_OVERRIDE;
  QModelIndex	parent(const QModelIndex& _index) const Q_DECL_OVERRIDE;
  static qreal calculateRate(quint64 _amount, quint64 _interest, quint32 _term);

private:
  quint32 m_depositCount;
  QHash<CryptoNote::TransactionId, QList<CryptoNote::DepositId> > m_spendingTransactions;

  DepositModel();
  ~DepositModel();

  QVariant getDisplayRole(const QModelIndex& _index) const;
  QVariant getDecorationRole(const QModelIndex& _index) const;
  QVariant getAlignmentRole(const QModelIndex& _index) const;
  QVariant getUserRole(const QModelIndex& _index, int _role) const;

  void reloadWalletDeposits();
  void appendDeposit(CryptoNote::DepositId _depositId);
  void transactionCreated(CryptoNote::TransactionId _transactionId);
  void reset();
  void depositsUpdated(const QVector<CryptoNote::DepositId>& _depositIds);
  void transactionUpdated(CryptoNote::TransactionId _transactionId);
};

}
