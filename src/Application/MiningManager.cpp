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

#include <QDateTime>
#include <QThread>
#include <QUrl>

#include "MiningManager.h"
#include "INodeAdapter.h"
#include "Miner/Miner.h"
#include "Settings/Settings.h"
#include "WalletLogger/WalletLogger.h"

namespace WalletGui {

namespace {

const char FAILOVER_POOL_SWITCH_STRATEGY[] = "Failover";
const char RANDOM_POOL_SWITCH_STRATEGY[] = "Random";

}

MiningManager::MiningManager(ICryptoNoteAdapter* _cryptoNoteAdapter, IDonationManager* _donationManager, QObject* _parent) :
  QObject(_parent), m_cryptoNoteAdapter(_cryptoNoteAdapter), m_donationManager(_donationManager), m_activeMinerIndex(-1) {
  m_donationManager->addObserver(this);

  int maxCpuCoreCount = QThread::idealThreadCount();
  if (maxCpuCoreCount == -1) {
    maxCpuCoreCount = 1;
  }

  if (maxCpuCoreCount % 2 == 0) {
    m_defaultCpuCoreCount = maxCpuCoreCount / 2;
  } else {
    m_defaultCpuCoreCount = (maxCpuCoreCount + 1) / 2;
  }
}

MiningManager::~MiningManager() {
}

void MiningManager::startMining() {
  switchToNextPool();
  Q_EMIT miningStartedSignal();
}

void MiningManager::stopMining() {
  for (IPoolMiner* miner : m_miners) {
    if (miner->getCurrentState() != IPoolMiner::STATE_STOPPED) {
      miner->stop();
    }
  }

  m_activeMinerIndex = -1;
  Q_EMIT miningStoppedSignal();
}

IMiningManager::SchedulePolicy MiningManager::getSchedulePolicy() const {
  QString schedulePolicyString = Settings::instance().getMiningPoolSwitchStrategy(FAILOVER_POOL_SWITCH_STRATEGY);
  if (!schedulePolicyString.compare(RANDOM_POOL_SWITCH_STRATEGY)) {
    return POLICY_RANDOM;
  }

  return POLICY_FAILOVER;
}

quint32 MiningManager::getCpuCoreCount() const {
  return Settings::instance().getMiningCpuCoreCount(m_defaultCpuCoreCount);
}

quintptr MiningManager::getMinerCount() const {
  return m_miners.size();
}

IPoolMiner*MiningManager::getMiner(quintptr _minerIndex) const {
  Q_ASSERT(_minerIndex < m_miners.size());
  return m_miners.at(_minerIndex);
}

void MiningManager::setSchedulePolicy(IMiningManager::SchedulePolicy _policy) {
  switch (_policy) {
  case POLICY_FAILOVER:
    Settings::instance().setMiningPoolSwitchStrategy(FAILOVER_POOL_SWITCH_STRATEGY);
    break;
  case POLICY_RANDOM:
    Settings::instance().setMiningPoolSwitchStrategy(RANDOM_POOL_SWITCH_STRATEGY);
    break;
  default:
    break;
  }
  Q_EMIT schedulePolicyChangedSignal(_policy);
}

void MiningManager::setCpuCoreCount(quint32 _cpuCoreCount) {
  Settings::instance().setMiningCpuCoreCount(_cpuCoreCount);
  Q_EMIT cpuCoreCountChangedSignal(_cpuCoreCount);
}

quintptr MiningManager::addMiner(const QString& _host, quint16 _port, quint32 _difficulty) {
  addNewMiner(_host, _port, _difficulty);
  saveMiners();
  Q_EMIT minerAddedSignal(m_miners.size() - 1);
  return m_miners.size() - 1;
}

void MiningManager::removeMiner(quintptr _minerIndex) {
  Q_ASSERT(_minerIndex < m_miners.size());
  IPoolMiner* miner = m_miners.takeAt(_minerIndex);
  if (miner->getCurrentState() != IPoolMiner::STATE_STOPPED) {
    miner->stop();
  }

  dynamic_cast<QObject*>(miner)->deleteLater();
  saveMiners();
  Q_EMIT minerRemovedSignal(_minerIndex);
}

void MiningManager::moveMiner(quintptr _fromIndex, quintptr _toIndex) {
  m_miners.move(_fromIndex, _toIndex);
  saveMiners();
  updateActiveMinerIndex();
}

void MiningManager::restoreDefaultMinerList() {
  Q_ASSERT(m_miners.isEmpty());
  Settings::instance().restoreDefaultPoolList();
  loadMiners();
}

void MiningManager::addObserver(IMinerManagerObserver* _observer) {
  QObject* observer = dynamic_cast<QObject*>(_observer);
  m_observerConnections[_observer] << connect(this, SIGNAL(minersLoadedSignal()), observer, SLOT(minersLoaded()));
  m_observerConnections[_observer] << connect(this, SIGNAL(minersUnloadedSignal()), observer, SLOT(minersUnloaded()));
  m_observerConnections[_observer] << connect(this, SIGNAL(miningStartedSignal()), observer, SLOT(miningStarted()));
  m_observerConnections[_observer] << connect(this, SIGNAL(miningStoppedSignal()), observer, SLOT(miningStopped()));
  m_observerConnections[_observer] << connect(this, SIGNAL(activeMinerChangedSignal(quintptr)), observer, SLOT(activeMinerChanged(quintptr)));
  m_observerConnections[_observer] << connect(this, SIGNAL(schedulePolicyChangedSignal(int)), observer, SLOT(schedulePolicyChanged(int)));
  m_observerConnections[_observer] << connect(this, SIGNAL(cpuCoreCountChangedSignal(quint32)), observer, SLOT(cpuCoreCountChanged(quint32)));
  m_observerConnections[_observer] << connect(this, SIGNAL(minerAddedSignal(quintptr)), observer, SLOT(minerAdded(quintptr)));
  m_observerConnections[_observer] << connect(this, SIGNAL(minerRemovedSignal(quintptr)), observer, SLOT(minerRemoved(quintptr)));
  m_observerConnections[_observer] << connect(this, SIGNAL(stateChangedSignal(quintptr,int)), observer, SLOT(stateChanged(quintptr,int)));
  m_observerConnections[_observer] << connect(this, SIGNAL(hashRateChangedSignal(quintptr,quint32)), observer, SLOT(hashRateChanged(quintptr,quint32)));
  m_observerConnections[_observer] << connect(this, SIGNAL(alternateHashRateChangedSignal(quintptr,quint32)), observer, SLOT(alternateHashRateChanged(quintptr,quint32)));
  m_observerConnections[_observer] << connect(this, SIGNAL(difficultyChangedSignal(quintptr,quint32)), observer, SLOT(difficultyChanged(quintptr,quint32)));
  m_observerConnections[_observer] << connect(this, SIGNAL(goodShareCountChangedSignal(quintptr,quint32)), observer, SLOT(goodShareCountChanged(quintptr,quint32)));
  m_observerConnections[_observer] << connect(this, SIGNAL(goodAlternateShareCountChangedSignal(quintptr,quint32)), observer, SLOT(goodAlternateShareCountChanged(quintptr,quint32)));
  m_observerConnections[_observer] << connect(this, SIGNAL(badShareCountChangedSignal(quintptr,quint32)), observer, SLOT(badShareCountChanged(quintptr,quint32)));
  m_observerConnections[_observer] << connect(this, SIGNAL(connectionErrorCountChangedSignal(quintptr,quint32)), observer, SLOT(connectionErrorCountChanged(quintptr,quint32)));
  m_observerConnections[_observer] << connect(this, SIGNAL(lastConnectionErrorTimeChangedSignal(quintptr,QDateTime)), observer, SLOT(lastConnectionErrorTimeChanged(quintptr,QDateTime)));
}

void MiningManager::removeObserver(IMinerManagerObserver* _observer) {
  if (!m_observerConnections.contains(_observer) || m_observerConnections.value(_observer).isEmpty()) {
    return;
  }

  for (const auto& connection : m_observerConnections[_observer]) {
    disconnect(connection);
  }

  m_observerConnections[_observer].clear();
}

void MiningManager::stateChanged(int _newState) {
  SchedulePolicy policy = getSchedulePolicy();
  switch (_newState) {
  case IPoolMiner::STATE_ERROR:
    WalletLogger::info("[MiningManager] Switching to next pool...");
    switchToNextPool();
    break;
  case IPoolMiner::STATE_RUNNING: {
    int activeMinerIndex = m_miners.indexOf(dynamic_cast<IPoolMiner*>(sender()));
    if (policy == POLICY_FAILOVER) {
      stopMinersWithLowPriority(activeMinerIndex);
    } else {
      stopOtherMiners(activeMinerIndex);
    }

    m_activeMinerIndex = activeMinerIndex;
    Q_EMIT activeMinerChangedSignal(m_activeMinerIndex);
    break;
  }
  }
}

void MiningManager::hashRateChanged(quint32 _hashRate) {
  int minerIndex = m_miners.indexOf(dynamic_cast<IPoolMiner*>(sender()));
  Q_EMIT hashRateChangedSignal(minerIndex, _hashRate);
}

void MiningManager::alternateHashRateChanged(quint32 _hashRate) {
  int minerIndex = m_miners.indexOf(dynamic_cast<IPoolMiner*>(sender()));
  Q_EMIT alternateHashRateChangedSignal(minerIndex, _hashRate);
}

void MiningManager::difficultyChanged(quint32 _difficulty) {
  int minerIndex = m_miners.indexOf(dynamic_cast<IPoolMiner*>(sender()));
  Q_EMIT difficultyChangedSignal(minerIndex, _difficulty);
}

void MiningManager::goodShareCountChanged(quint32 _goodShareCount) {
  int minerIndex = m_miners.indexOf(dynamic_cast<IPoolMiner*>(sender()));
  Q_EMIT goodShareCountChangedSignal(minerIndex, _goodShareCount);
}

void MiningManager::goodAlternateShareCountChanged(quint32 _goodShareCount) {
  int minerIndex = m_miners.indexOf(dynamic_cast<IPoolMiner*>(sender()));
  Q_EMIT goodAlternateShareCountChangedSignal(minerIndex, _goodShareCount);
}

void MiningManager::badShareCountChanged(quint32 _badShareCount) {
  int minerIndex = m_miners.indexOf(dynamic_cast<IPoolMiner*>(sender()));
  Q_EMIT badShareCountChangedSignal(minerIndex, _badShareCount);
}

void MiningManager::connectionErrorCountChanged(quint32 _connectionErrorCount) {
  int minerIndex = m_miners.indexOf(dynamic_cast<IPoolMiner*>(sender()));
  Q_EMIT connectionErrorCountChangedSignal(minerIndex, _connectionErrorCount);
}

void MiningManager::lastConnectionErrorTimeChanged(const QDateTime& _lastConnectionErrorTime) {
  int minerIndex = m_miners.indexOf(dynamic_cast<IPoolMiner*>(sender()));
  Q_EMIT lastConnectionErrorTimeChangedSignal(minerIndex, _lastConnectionErrorTime);
}

void MiningManager::donationManagerOpened() {
  loadMiners();
}

void MiningManager::donationManagerClosed() {
  for (IPoolMiner* miner : m_miners) {
    miner->removeObserver(this);
    if (miner->getCurrentState() != Miner::STATE_STOPPED) {
      miner->stop();
    }

    dynamic_cast<QObject*>(miner)->deleteLater();
  }

  m_miners.clear();
  Q_EMIT minersUnloadedSignal();
}

void MiningManager::donationMiningEnabled(bool _on) {
  for (IPoolMiner* miner : m_miners) {
    if (_on) {
      miner->setAlternateAccount(m_donationManager->getDonationMiningAddress(), m_donationManager->getDonationMiningAmount());
    } else {
      miner->unsetAlternateAccount();
    }
  }
}

void MiningManager::donationMiningAddressChanged(const QString& _address) {
  for (IPoolMiner* miner : m_miners) {
    if (m_donationManager->isDonationMiningEnabled()) {
      miner->setAlternateAccount(_address, m_donationManager->getDonationMiningAmount());
    } else {
      miner->unsetAlternateAccount();
    }
  }
}

void MiningManager::donationMiningAmountChanged(int _amount) {
  for (IPoolMiner* miner : m_miners) {
    if (m_donationManager->isDonationMiningEnabled()) {
      miner->setAlternateAccount(m_donationManager->getDonationMiningAddress(), _amount);
    } else {
      miner->unsetAlternateAccount();
    }
  }
}

void MiningManager::donationChangeEnabled(bool _on) {
  // Do nothing
}

void MiningManager::donationChangeAddressChanged(const QString& _address) {
  // Do nothing
}

void MiningManager::donationChangeAmountChanged(int _amount) {
  // Do nothing
}

void MiningManager::switchToNextPool() {
  QList<quintptr> errorMinerIndexes = getErrorMiners();
  if (errorMinerIndexes.size() == m_miners.size()) {
    return;
  }

  QList<quintptr> stoppedMinerIndexes = getStoppedMiners();
  if (stoppedMinerIndexes.isEmpty()) {
    return;
  }

  SchedulePolicy policy = getSchedulePolicy();
  int nextMinerIndex = -1;
  switch (policy) {
  case MiningManager::POLICY_FAILOVER:
    nextMinerIndex = stoppedMinerIndexes.first();
    break;
  case MiningManager::POLICY_RANDOM: {
    if (m_activeMinerIndex != -1) {
      m_miners[m_activeMinerIndex]->stop();
    }

    nextMinerIndex = stoppedMinerIndexes.at(qrand() % stoppedMinerIndexes.size());
    break;
  }
  }

  m_activeMinerIndex = nextMinerIndex;
  m_miners[m_activeMinerIndex]->start(getCpuCoreCount());
  Q_EMIT activeMinerChangedSignal(m_activeMinerIndex);
}

void MiningManager::stopMinersWithLowPriority(quintptr _minerIndex) {
  if (_minerIndex == m_miners.size() - 1) {
    return;
  }

  for (quintptr i = _minerIndex + 1; i < m_miners.size(); ++i) {
    if (m_miners[i]->getCurrentState() != IPoolMiner::STATE_STOPPED) {
      m_miners[i]->stop();
    }
  }
}

void MiningManager::stopOtherMiners(quintptr _minerIndex) {
  for (quintptr i = 0; i < m_miners.size(); ++i) {
    if (i != _minerIndex && m_miners[i]->getCurrentState() != IPoolMiner::STATE_STOPPED) {
      m_miners[i]->stop();
    }
  }
}

QList<quintptr> MiningManager::getStoppedMiners() const {
  QList<quintptr> result;
  for (quintptr i = 0; i < m_miners.size(); ++i) {
    if (m_miners[i]->getCurrentState() == IPoolMiner::STATE_STOPPED) {
      result.append(i);
    }
  }

  return result;
}

QList<quintptr> MiningManager::getErrorMiners() const {
  QList<quintptr> result;
  for (quintptr i = 0; i < m_miners.size(); ++i) {
    if (m_miners[i]->getCurrentState() == IPoolMiner::STATE_ERROR) {
      result.append(i);
    }
  }

  return result;
}

void MiningManager::updateActiveMinerIndex() {
  m_activeMinerIndex = -1;
  for (quintptr i = 0; i < m_miners.size(); ++i) {
    if (m_miners[i]->getCurrentState() == IPoolMiner::STATE_RUNNING) {
      m_activeMinerIndex = i;
      break;
    }
  }
}

void MiningManager::addNewMiner(const QString& _host, quint16 _port, quint32 _difficulty) {
  IWalletAdapter* walletAdapter = m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter();
  Miner* miner = new Miner(_host, _port, _difficulty, walletAdapter->getAddress(0), "x", this);
  miner->addObserver(this);
  m_miners.append(miner);
  if (m_donationManager->isDonationMiningEnabled()) {
    miner->setAlternateAccount(m_donationManager->getDonationMiningAddress(), m_donationManager->getDonationMiningAmount());
  }
}

void MiningManager::loadMiners() {
  QStringList poolList = Settings::instance().getMiningPoolList();
  for (const QString& pool : poolList) {
    QStringList poolParamList = pool.split(":");
    if (poolParamList.size() < 2) {
      continue;
    }

    QUrl poolUrl = QUrl::fromUserInput(QString("%1:%2").arg(poolParamList[0]).arg(poolParamList[1]));
    if (!poolUrl.isValid()) {
      continue;
    }

    quint32 difficulty = 0;
    if (poolParamList.size() >= 3) {
      difficulty = poolParamList[2].toUInt();
    }

    addNewMiner(poolUrl.host(), poolUrl.port(), difficulty);
  }

  Q_EMIT minersLoadedSignal();
}

void MiningManager::saveMiners() {
  QStringList minerList;
  for (IPoolMiner* miner : m_miners) {
    if (miner->getDifficulty() > 0) {
      minerList << QString("%1:%2:%3").arg(miner->getPoolHost()).arg(miner->getPoolPort()).arg(miner->getDifficulty());
    } else {
      minerList << QString("%1:%2").arg(miner->getPoolHost()).arg(miner->getPoolPort());
    }
  }

  Settings::instance().setMiningPoolList(minerList);

}

}
