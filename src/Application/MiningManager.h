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

#include <QMap>
#include <QMetaObject>
#include <QModelIndex>
#include <QObject>

#include "IMiningManager.h"
#include "ICryptoNoteAdapter.h"
#include "IDonationManager.h"
#include "IPoolMiner.h"
#include "IWalletAdapter.h"

namespace WalletGui {

class ICryptoNoteAdapter;

class MiningManager : public QObject, public IMiningManager, public IPoolMinerObserver, public IDonationManagerObserver {
  Q_OBJECT
  Q_DISABLE_COPY(MiningManager)

public:
  MiningManager(ICryptoNoteAdapter* _cryptoNoteAdapter, IDonationManager* _donationManager, QObject* _parent);
  ~MiningManager();

  // IMinerManager
  virtual void startMining() override;
  virtual void stopMining() override;
  virtual SchedulePolicy getSchedulePolicy() const override;
  virtual quint32 getCpuCoreCount() const override;
  virtual quintptr getMinerCount() const override;
  virtual IPoolMiner* getMiner(quintptr _minerIndex) const override;
  virtual void setSchedulePolicy(SchedulePolicy _policy) override;
  virtual void setCpuCoreCount(quint32 _cpuCoreCount) override;
  virtual quintptr addMiner(const QString& _host, quint16 _port, quint32 _difficulty) override;
  virtual void removeMiner(quintptr _minerIndex) override;
  virtual void moveMiner(quintptr _fromIndex, quintptr _toIndex) override;
  virtual void restoreDefaultMinerList() override;
  virtual void addObserver(IMinerManagerObserver* _observer) override;
  virtual void removeObserver(IMinerManagerObserver* _observer) override;

  // IPoolMinerObserver
  Q_SLOT virtual void stateChanged(int _newState) override;
  Q_SLOT virtual void hashRateChanged(quint32 _hashRate) override;
  Q_SLOT virtual void alternateHashRateChanged(quint32 _hashRate) override;
  Q_SLOT virtual void difficultyChanged(quint32 _difficulty) override;
  Q_SLOT virtual void goodShareCountChanged(quint32 _goodShareCount) override;
  Q_SLOT virtual void goodAlternateShareCountChanged(quint32 _goodShareCount) override;
  Q_SLOT virtual void badShareCountChanged(quint32 _badShareCount) override;
  Q_SLOT virtual void connectionErrorCountChanged(quint32 _connectionErrorCount) override;
  Q_SLOT virtual void lastConnectionErrorTimeChanged(const QDateTime& _lastConnectionErrorTime) override;

  // IDonationManagerObserver
  Q_SLOT virtual void donationManagerOpened() override;
  Q_SLOT virtual void donationManagerClosed() override;
  Q_SLOT virtual void donationMiningEnabled(bool _on) override;
  Q_SLOT virtual void donationMiningAddressChanged(const QString& _address) override;
  Q_SLOT virtual void donationMiningAmountChanged(int _amount) override;
  Q_SLOT virtual void donationChangeEnabled(bool _on) override;
  Q_SLOT virtual void donationChangeAddressChanged(const QString& _address) override;
  Q_SLOT virtual void donationChangeAmountChanged(int _amount) override;

private:
  ICryptoNoteAdapter* m_cryptoNoteAdapter;
  IDonationManager* m_donationManager;
  QList<IPoolMiner*> m_miners;
  quint32 m_defaultCpuCoreCount;
  QMap<IMinerManagerObserver*, QList<QMetaObject::Connection>> m_observerConnections;
  int m_activeMinerIndex;

  void switchToNextPool();
  void stopMinersWithLowPriority(quintptr _minerIndex);
  void stopOtherMiners(quintptr _minerIndex);
  QList<quintptr> getStoppedMiners() const;
  QList<quintptr> getErrorMiners() const;
  void updateActiveMinerIndex();
  void addNewMiner(const QString& _host, quint16 _port, quint32 _difficulty);
  void loadMiners();
  void saveMiners();

Q_SIGNALS:
  void minersLoadedSignal();
  void minersUnloadedSignal();
  void miningStartedSignal();
  void miningStoppedSignal();
  void activeMinerChangedSignal(quintptr _minerIndex);
  void schedulePolicyChangedSignal(int _schedulePolicy);
  void cpuCoreCountChangedSignal(quint32 _cpuCoreCount);
  void minerAddedSignal(quintptr _minerIndex);
  void minerRemovedSignal(quintptr _minerIndex);

  void stateChangedSignal(quintptr _minerIndex, int _newState);
  void hashRateChangedSignal(quintptr _minerIndex, quint32 _hashRate);
  void alternateHashRateChangedSignal(quintptr _minerIndex, quint32 _hashRate);
  void difficultyChangedSignal(quintptr _minerIndex, quint32 _difficulty);
  void goodShareCountChangedSignal(quintptr _minerIndex, quint32 _goodShareCount);
  void goodAlternateShareCountChangedSignal(quintptr _minerIndex, quint32 _goodShareCount);
  void badShareCountChangedSignal(quintptr _minerIndex, quint32 _badShareCount);
  void connectionErrorCountChangedSignal(quintptr _minerIndex, quint32 _connectionErrorCount);
  void lastConnectionErrorTimeChangedSignal(quintptr _minerIndex, const QDateTime& _lastConnectionErrorTime);
};

}
