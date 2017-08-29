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
#include "IDonationManager.h"
#include "IWalletAdapter.h"
#include "IMiningManager.h"

namespace WalletGui {

class MinerModel : public QAbstractItemModel, public IMinerManagerObserver {
  Q_OBJECT
  Q_DISABLE_COPY(MinerModel)
  Q_ENUMS(Columns)
  Q_ENUMS(Roles)

public:
  enum Columns {
    COLUMN_POOL_URL = 0, COLUMN_DIFFICULTY, COLUMN_GOOD_SHARES, COLUMN_BAD_SHARES, COLUMN_DONATED_SHARES,
      COLUMN_CONNECTION_ERROR_COUNT, COLUMN_LAST_CONNECTION_ERROR_TIME, COLUMN_REMOVE, COLUMN_HASHRATE,
      COLUMN_DONATION_HASHRATE
  };

  enum Roles {
    ROLE_POOL_HOST = Qt::UserRole, ROLE_POOL_PORT, ROLE_DIFFICULTY, ROLE_GOOD_SHARES, ROLE_BAD_SHARES, ROLE_DONATED_SHARES,
      ROLE_CONNECTION_ERROR_COUNT, ROLE_LAST_CONNECTION_ERROR_TIME, ROLE_STATE, ROLE_START_MINER, ROLE_HASHRATE,
      ROLE_DONATION_HASHRATE
  };

  MinerModel(IMiningManager* _minerManager, QObject* _parent);
  ~MinerModel();

  // QAbstractItemModel
  int columnCount(const QModelIndex& _parent = QModelIndex()) const override;
  int rowCount(const QModelIndex& _parent = QModelIndex()) const override;
  Qt::DropActions supportedDragActions() const override;
  Qt::DropActions supportedDropActions() const override;
  Qt::ItemFlags flags(const QModelIndex& _index) const override;
  QVariant headerData(int _section, Qt::Orientation _orientation, int _role = Qt::DisplayRole) const override;
  QVariant data(const QModelIndex& _index, int _role = Qt::DisplayRole) const override;
  QModelIndex index(int _row, int _column, const QModelIndex& _parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex& _index) const override;
  bool moveRows(const QModelIndex& _sourceParent, int _sourceRow, int _count, const QModelIndex& _destinationParent,
    int _destinationChild) override;
  bool removeRows(int _row, int _count, const QModelIndex& _parent = QModelIndex()) override;

  // IMinerManagerObserver
  Q_SLOT virtual void minersLoaded() override;
  Q_SLOT virtual void minersUnloaded() override;
  Q_SLOT virtual void miningStarted() override;
  Q_SLOT virtual void miningStopped() override;
  Q_SLOT virtual void activeMinerChanged(quintptr _minerIndex) override;
  Q_SLOT virtual void schedulePolicyChanged(int _schedulePolicy) override;
  Q_SLOT virtual void cpuCoreCountChanged(quint32 _cpuCoreCount) override;
  Q_SLOT virtual void minerAdded(quintptr _minerIndex) override;
  Q_SLOT virtual void minerRemoved(quintptr _minerIndex) override;
  Q_SLOT virtual void stateChanged(quintptr _minerIndex, int _newState) override;
  Q_SLOT virtual void hashRateChanged(quintptr _minerIndex, quint32 _hashRate) override;
  Q_SLOT virtual void alternateHashRateChanged(quintptr _minerIndex, quint32 _hashRate) override;
  Q_SLOT virtual void difficultyChanged(quintptr _minerIndex, quint32 _difficulty) override;
  Q_SLOT virtual void goodShareCountChanged(quintptr _minerIndex, quint32 _goodShareCount) override;
  Q_SLOT virtual void goodAlternateShareCountChanged(quintptr _minerIndex, quint32 _goodShareCount) override;
  Q_SLOT virtual void badShareCountChanged(quintptr _minerIndex, quint32 _badShareCount) override;
  Q_SLOT virtual void connectionErrorCountChanged(quintptr _minerIndex, quint32 _connectionErrorCount) override;
  Q_SLOT virtual void lastConnectionErrorTimeChanged(quintptr _minerIndex, const QDateTime& _lastConnectionErrorTime) override;

protected:
  virtual void timerEvent(QTimerEvent* _event) override;

private:
  IMiningManager* m_miningManager;
  int m_lastConnectionErrorTimerId;
  const int m_columnCount;

  QVariant getDecorationRole(const QModelIndex& _index) const;
  QVariant getDislayRole(const QModelIndex& _index) const;
  QVariant getUserRoles(const QModelIndex& _index, int _role) const;
};

}
