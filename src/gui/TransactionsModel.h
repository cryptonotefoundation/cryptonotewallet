// Copyright (c) 2011-2015 The Cryptonote developers
// Copyright (c) 2016-2019 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <QAbstractItemModel>
#include <QSortFilterProxyModel>

#include <IWalletLegacy.h>

namespace WalletGui {

enum class TransactionType : quint8 {MINED, INPUT, OUTPUT, INOUT, FUSION};

typedef QPair<CryptoNote::TransactionId, CryptoNote::TransferId> TransactionTransferId;

class TransactionsModel : public QAbstractItemModel {
  Q_OBJECT
  Q_ENUMS(Columns)
  Q_ENUMS(Roles)

public:
  enum Columns{
    COLUMN_STATE = 0, COLUMN_DATE, COLUMN_AMOUNT, COLUMN_FEE, COLUMN_ADDRESS, COLUMN_PAYMENT_ID, COLUMN_HASH,
    COLUMN_HEIGHT, COLUMN_TYPE, COLUMN_SECRET_KEY
  };

  enum Roles{
    ROLE_DATE = Qt::UserRole, ROLE_TYPE, ROLE_HASH, ROLE_ADDRESS, ROLE_AMOUNT, ROLE_PAYMENT_ID, ROLE_ICON,
    ROLE_TRANSACTION_ID, ROLE_HEIGHT, ROLE_FEE, ROLE_NUMBER_OF_CONFIRMATIONS, ROLE_SECRET_KEY, ROLE_COLUMN, ROLE_ROW
  };

  static TransactionsModel& instance();

  Qt::ItemFlags flags(const QModelIndex& _index) const Q_DECL_OVERRIDE;
  int columnCount(const QModelIndex& _parent = QModelIndex()) const Q_DECL_OVERRIDE;
  int rowCount(const QModelIndex& _parent = QModelIndex()) const Q_DECL_OVERRIDE;

  QVariant headerData(int _section, Qt::Orientation _orientation, int _role = Qt::EditRole) const Q_DECL_OVERRIDE;
  QVariant data(const QModelIndex& _index, int _role = Qt::EditRole) const Q_DECL_OVERRIDE;
  QModelIndex index(int _row, int _column, const QModelIndex& _parent = QModelIndex()) const Q_DECL_OVERRIDE;
  QModelIndex parent(const QModelIndex& _index) const Q_DECL_OVERRIDE;

  QByteArray toCsv() const;

  void reloadWalletTransactions();

private:
  QVector<TransactionTransferId> m_transfers;
  QHash<CryptoNote::TransactionId, QPair<quint32, quint32> > m_transactionRow;

  TransactionsModel();
  ~TransactionsModel();

  QVariant getDisplayRole(const QModelIndex& _index) const;
  QVariant getEditRole(const QModelIndex& _index) const;
  QVariant getDecorationRole(const QModelIndex& _index) const;
  QVariant getAlignmentRole(const QModelIndex& _index) const;
  QVariant getToolTipRole(const QModelIndex& _index) const;
  QVariant getUserRole(const QModelIndex& _index, int _role, CryptoNote::TransactionId _transactionId, CryptoNote::WalletLegacyTransaction& _transaction,
    CryptoNote::TransferId _transferId, CryptoNote::WalletLegacyTransfer& _transfer) const;

  void appendTransaction(CryptoNote::TransactionId _id, quint32& _row_count);
  void appendTransaction(CryptoNote::TransactionId _id);
  void updateWalletTransaction(CryptoNote::TransactionId _id);
  void localBlockchainUpdated(quint64 _height);
  void reset();
};

}
