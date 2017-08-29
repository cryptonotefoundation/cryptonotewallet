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
#include <QCache>
#include <QDateTime>
#include <QSortFilterProxyModel>

#include "ICryptoNoteAdapter.h"
#include "INodeAdapter.h"

#include <BlockchainExplorerData.h>

namespace WalletGui {

class BlockchainModel : public QAbstractItemModel, public ICryptoNoteAdapterObserver
{
  Q_OBJECT
  Q_DISABLE_COPY(BlockchainModel)
  Q_ENUMS(Type)
  Q_ENUMS(Columns)
  Q_ENUMS(Roles)

public:
  enum Type {
    TYPE_BLOCK, TYPE_TRANSACTION
  };

  enum Columns {
    COLUMN_BLOCK_TIME = 0, COLUMN_BLOCK_HEIGHT, COLUMN_BLOCK_SIZE, COLUMN_BLOCK_TRANSACTION_COUNT, COLUMN_BLOCK_HASH, COLUMN_BLOCK_TRANSACTIONS_SIZE,
    COLUMN_BLOCK_DIFFICULTY, COLUMN_BLOCK_IS_ORPHANED, COLUMN_BLOCK_REWARD_PENALTY, COLUMN_BLOCK_BASE_REWARD, COLUMN_BLOCK_REWARD,
    COLUMN_BLOCK_TOTAL_COIN_COUNT, COLUMN_BLOCK_TOTAL_TRANSACTION_COUNT, COLUMN_BLOCK_VERSION, COLUMN_BLOCK_FEE,
    COLUMN_TRANSACTION_OUTPUT_AMOUNT, COLUMN_TRANSACTION_HASH, COLUMN_TRANSACTION_SIZE, COLUMN_TRANSACTION_FEE, COLUMN_TRANSACTION_PAYMENT_ID,
    COLUMN_TRANSACTION_INPUT_AMOUNT, COLUMN_TRANSACTION_MIXIN, COLUMN_TRANSACTION_UNLOCK_TIME, COLUMN_TRANSACTION_TIME,
    COLUMN_TRANSACTION_IS_IN_BLOCKCHAIN, COLUMN_TRANSACTION_BLOCK_HASH, COLUMN_TRANSACTION_BLOCK_HEIGHT
  };

  enum Roles {
    ROLE_ITEM_TYPE = Qt::UserRole, ROLE_BLOCK_HEIGHT, ROLE_BLOCK_HASH, ROLE_BLOCK_TIME, ROLE_BLOCK_DIFFICULTY, ROLE_BLOCK_IS_ORPHANED,
    ROLE_BLOCK_TRANSACTION_COUNT, ROLE_BLOCK_TRANSACTIONS_SIZE, ROLE_BLOCK_SIZE, ROLE_BLOCK_MAJOR_VERSION,
    ROLE_BLOCK_MINOR_VERSION, ROLE_BLOCK_TOTAL_COIN_COUNT, ROLE_BLOCK_TOTAL_TRANSACTION_COUNT, ROLE_BLOCK_BASE_REWARD, ROLE_BLOCK_REWARD_PENALTY,
    ROLE_BLOCK_REWARD, ROLE_BLOCK_FEE, ROLE_COLUMN,
    ROLE_TRANSACTION_HASH, ROLE_TRANSACTION_SIZE, ROLE_TRANSACTION_FEE, ROLE_TRANSACTION_INPUT_AMOUNT, ROLE_TRANSACTION_OUTPUT_AMOUNT,
    ROLE_TRANSACTION_MIXIN, ROLE_TRANSACTION_UNLOCK_TIME, ROLE_TRANSACTION_TIME, ROLE_TRANSACTION_PAYMENT_ID,
    ROLE_TRANSACTION_IS_IN_BLOCKCHAIN, ROLE_TRANSACTION_BLOCK_HASH, ROLE_TRANSACTION_BLOCK_HEIGHT
  };

  BlockchainModel(ICryptoNoteAdapter* _cryptoNoteAdapter, QAbstractItemModel* _nodeStateModel, QObject* _parent);
  ~BlockchainModel();
  Qt::ItemFlags flags(const QModelIndex &_index) const override;
  int columnCount(const QModelIndex &_parent = QModelIndex()) const override;
  int rowCount(const QModelIndex &_parent = QModelIndex()) const override;
  QVariant headerData(int _section, Qt::Orientation _orientation, int _role = Qt::DisplayRole) const override;
  QVariant data(const QModelIndex &_index, int _role = Qt::DisplayRole) const override;
  QModelIndex index(int _row, int _column, const QModelIndex &_parent = QModelIndex()) const override;
  QModelIndex	parent(const QModelIndex &_index) const override;

  // ICryptoNoteAdapterObserver
  Q_SLOT virtual void cryptoNoteAdapterInitCompleted(int _status) override;
  Q_SLOT virtual void cryptoNoteAdapterDeinitCompleted() override;

private:
  ICryptoNoteAdapter* m_cryptoNoteAdapter;
  QAbstractItemModel* m_nodeStateModel;
  const int m_columnCount;
  quint32 m_lastLocalBlockIndex;

  void nodeStateChanged(const QModelIndex& _topLeft, const QModelIndex& _bottomRight, const QVector<int>& _roles);
};

}
