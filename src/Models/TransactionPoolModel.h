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

#include "IBlockChainExplorerAdapter.h"
#include "ICryptoNoteAdapter.h"

namespace CryptoNote {
struct TransactionDetails;
}

namespace WalletGui {

class ICryptoNoteAdapter;

class TransactionPoolModel : public QAbstractItemModel, public IBlockChainExplorerAdapterObserver, public ICryptoNoteAdapterObserver {
  Q_OBJECT
  Q_DISABLE_COPY(TransactionPoolModel)
  Q_ENUMS(Columns)
  Q_ENUMS(Roles)

public:
  enum Columns {
    COLUMN_TIME = 0, COLUMN_HASH, COLUMN_AMOUNT, COLUMN_FEE, COLUMN_SIZE, COLUMN_MIXIN, COLUMN_PAYMENT_ID
  };

  enum Roles {
    ROLE_TIME = Qt::UserRole, ROLE_AMOUNT, ROLE_FEE, ROLE_SIZE, ROLE_HASH, ROLE_MIXIN, ROLE_PAYMENT_ID, ROLE_COLUMN
  };

  TransactionPoolModel(ICryptoNoteAdapter* _cryptoNoteAdapter, QObject* _parent);
  ~TransactionPoolModel();

  Qt::ItemFlags flags(const QModelIndex &_index) const override;
  int columnCount(const QModelIndex &_parent = QModelIndex()) const override;
  int rowCount(const QModelIndex &_parent = QModelIndex()) const override;
  QVariant headerData(int _section, Qt::Orientation _orientation, int _role = Qt::DisplayRole) const override;
  QVariant data(const QModelIndex &_index, int _role = Qt::DisplayRole) const override;
  QModelIndex index(int _row, int _column, const QModelIndex &_parent = QModelIndex()) const override;
  QModelIndex	parent(const QModelIndex &_index) const override;

  // IBlockChainExplorerAdapterObserver
  Q_SLOT virtual void blockChainUpdated(const QVector<CryptoNote::BlockDetails>& _newBlocks,
    const QVector<CryptoNote::BlockDetails>& _orphanedBlocks) override;
  Q_SLOT virtual void poolUpdated(const QVector<CryptoNote::TransactionDetails>& _newTransactions,
    const QVector<Crypto::Hash>& _removedTransactions) override;
  Q_SLOT virtual void blockchainSynchronized(const CryptoNote::BlockDetails& _topBlock) override;

  // ICryptoNoteAdapterObserver
  Q_SLOT virtual void cryptoNoteAdapterInitCompleted(int _status) override;
  Q_SLOT virtual void cryptoNoteAdapterDeinitCompleted() override;

  static int findProxyColumn(QAbstractItemModel* _proxyModel, int _originColumn);

private:
  ICryptoNoteAdapter* m_cryptoNoteAdapter;
  QList<QByteArray> m_rowHashes;
  QHash<QByteArray, CryptoNote::TransactionDetails> m_transactions;
  bool m_isSynchronized;
  const int m_columnCount;

  QVariant getDisplayRole(const QModelIndex& _index) const;
  QVariant getUserRole(const QModelIndex& _index, int _role) const;

Q_SIGNALS:
  void initCompleted();
};

}
