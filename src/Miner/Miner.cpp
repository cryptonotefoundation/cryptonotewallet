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

#include <QThread>
#include <QTimerEvent>

#include "Miner.h"
#include "StratumClient.h"
#include "Worker.h"

namespace WalletGui {

namespace {

const int HASHRATE_TIMER_INTERVAL = 1000;

}

Miner::Miner(const QString& _host, quint16 _port, quint32 _difficulty, const QString& _login,
  const QString& _password, QObject* _parent) : QObject(_parent), m_minerState(STATE_STOPPED),
  m_mainJob(), m_alternateJob(), m_mainJobLock(), m_alternateJobLock(), m_alternateStratumClient(nullptr),
  m_mainNonce(0), m_alternateNonce(0), m_hashCounter(0), m_alternameHashCounter(0), m_alternateProbability(0),
  m_hashCountPerSecond(0), m_alternateHashCountPerSecond(0), m_hashRateTimerId(-1) {
  m_mainStratumClient = new StratumClient(m_mainJob, m_mainJobLock, m_mainNonce, _host, _port, _difficulty, _login, _password, this);
  m_mainStratumClient->addObserver(this);
}

Miner::~Miner() {
  if (m_minerState != STATE_STOPPED) {
    stop();
  }
}

void Miner::start(quint32 _coreCount) {
  Q_ASSERT(m_minerState == STATE_STOPPED);
  m_mainStratumClient->start();
  if (m_alternateStratumClient != nullptr) {
    m_alternateStratumClient->start();
  }

  if (m_hashRateTimerId == -1) {
    m_hashRateTimerId = startTimer(HASHRATE_TIMER_INTERVAL);
  }

  for (quint32 i = 0; i < _coreCount; ++i) {
    if (m_workerThreadList.size() < i + 1) {
      Worker* worker = new Worker(m_mainJob, m_alternateJob, m_mainJobLock, m_alternateJobLock,
        m_mainNonce, m_alternateNonce, m_alternateProbability, m_hashCounter, m_alternameHashCounter, nullptr);
      worker->addObserver(m_mainStratumClient);
      if (m_alternateStratumClient != nullptr) {
        worker->addAlternateObserver(m_alternateStratumClient);
      }

      QThread* thread = new QThread(this);
      connect(thread, &QThread::started, worker, &Worker::start);
      worker->moveToThread(thread);
      m_workerThreadList.append(qMakePair(thread, worker));
    }

    m_workerThreadList[i].first->start();
  }
}

void Miner::stop() {
  Q_ASSERT(m_minerState != STATE_STOPPED);
  m_mainStratumClient->stop();
  if (m_alternateStratumClient != nullptr) {
    m_alternateStratumClient->stop();
  }

  if (m_hashRateTimerId != -1) {
    killTimer(m_hashRateTimerId);
    m_hashRateTimerId = -1;
  }

  m_hashCountPerSecond = 0;
  m_alternateHashCountPerSecond = 0;
  for (auto& workerThread : m_workerThreadList) {
    workerThread.second->stop();
    workerThread.first->quit();
  }

  for (auto& workerThread : m_workerThreadList) {
    if (workerThread.first->isRunning()) {
      workerThread.first->wait();
    }
  }
}

Miner::State Miner::getCurrentState() const {
  return m_minerState;
}

QString Miner::getPoolHost() const {
  return m_mainStratumClient->getPoolHost();
}

quint16 Miner::getPoolPort() const {
  return m_mainStratumClient->getPoolPort();
}

quint32 Miner::getDifficulty() const {
  return m_mainStratumClient->getDifficulty();
}

quint32 Miner::getHashRate() const {
  return m_hashCountPerSecond;
}

quint32 Miner::getAlternateHashRate() const {
  return m_alternateHashCountPerSecond;
}

quint32 Miner::getGoodShareCount() const {
  return m_mainStratumClient->getGoodShareCount();
}

quint32 Miner::getGoodAlternateShareCount() const {
  if (m_alternateStratumClient != nullptr) {
    return m_alternateStratumClient->getGoodShareCount();
  }

  return 0;
}

quint32 Miner::getBadShareCount() const {
  return m_mainStratumClient->getBadShareCount();
}

quint32 Miner::getConnectionErrorCount() const {
  return m_mainStratumClient->getConnectionErrorCount();
}

QDateTime Miner::getLastConnectionErrorTime() const {
  return m_mainStratumClient->getLastConnectionErrorTime();
}

void Miner::setAlternateAccount(const QString& _login, quint32 _probability) {
  if (m_alternateStratumClient != nullptr) {
    if (m_alternateStratumClient->getLogin() == _login && m_alternateProbability == _probability) {
      return;
    }

    if (m_alternateStratumClient->getLogin() == _login) {
      m_alternateProbability = _probability;
      return;
    }

    m_alternateStratumClient->stop();
    m_alternateStratumClient->deleteLater();
  }

  m_alternateProbability = _probability;
  m_alternateStratumClient = new StratumClient(m_alternateJob, m_alternateJobLock, m_alternateNonce,
    m_mainStratumClient->getPoolHost(), m_mainStratumClient->getPoolPort(), m_mainStratumClient->getDifficulty(), _login, "x", this);
  connect(m_alternateStratumClient, &StratumClient::goodShareCountChangedSignal, this, &Miner::goodAlternateShareCountChangedSignal);
  if (m_minerState != STATE_STOPPED) {
    m_alternateStratumClient->start();
  }

  for (auto& workerThread : m_workerThreadList) {
    workerThread.second->addAlternateObserver(m_alternateStratumClient);
  }
}

void Miner::unsetAlternateAccount() {
  if (m_alternateStratumClient == nullptr) {
    return;
  }

  for (auto& workerThread : m_workerThreadList) {
    workerThread.second->removeAlternateObserver(m_alternateStratumClient);
  }

  if (m_minerState != STATE_STOPPED) {
    m_alternateStratumClient->stop();
  }

  m_alternateStratumClient->deleteLater();
  m_alternateStratumClient = nullptr;
  m_alternateProbability = 0;
}

void Miner::addObserver(IPoolMinerObserver* _observer) {
  QObject* observer = dynamic_cast<QObject*>(_observer);
  m_observerConnections[_observer] << connect(this, SIGNAL(stateChangedSignal(int)), observer, SLOT(stateChanged(int)));
  m_observerConnections[_observer] << connect(this, SIGNAL(hashRateChangedSignal(quint32)), observer, SLOT(hashRateChanged(quint32)));
  m_observerConnections[_observer] << connect(this, SIGNAL(alternateHashRateChangedSignal(quint32)), observer, SLOT(alternateHashRateChanged(quint32)));
  m_observerConnections[_observer] << connect(this, SIGNAL(difficultyChangedSignal(quint32)), observer, SLOT(difficultyChanged(quint32)));
  m_observerConnections[_observer] << connect(this, SIGNAL(goodShareCountChangedSignal(quint32)), observer, SLOT(goodShareCountChanged(quint32)));
  m_observerConnections[_observer] << connect(this, SIGNAL(goodAlternateShareCountChangedSignal(quint32)), observer, SLOT(goodAlternateShareCountChanged(quint32)));
  m_observerConnections[_observer] << connect(this, SIGNAL(badShareCountChangedSignal(quint32)), observer, SLOT(badShareCountChanged(quint32)));
  m_observerConnections[_observer] << connect(this, SIGNAL(connectionErrorCountChangedSignal(quint32)), observer, SLOT(connectionErrorCountChanged(quint32)));
  m_observerConnections[_observer] << connect(this, SIGNAL(lastConnectionErrorTimeChangedSignal(QDateTime)), observer, SLOT(lastConnectionErrorTimeChanged(QDateTime)));
}

void Miner::removeObserver(IPoolMinerObserver* _observer) {
  if (!m_observerConnections.contains(_observer) || m_observerConnections.value(_observer).isEmpty()) {
    return;
  }

  for (const auto& connection : m_observerConnections[_observer]) {
    disconnect(connection);
  }

  m_observerConnections[_observer].clear();
}

void Miner::started() {
  setState(STATE_RUNNING);
}

void Miner::stopped() {
  setState(STATE_STOPPED);
}

void Miner::socketError() {
  setState(STATE_ERROR);
  m_hashCountPerSecond = 0;
}

void Miner::difficultyChanged(quint32 _difficulty) {
  Q_EMIT difficultyChangedSignal(_difficulty);
}

void Miner::goodShareCountChanged(quint32 _goodShareCount) {
  Q_EMIT goodShareCountChangedSignal(_goodShareCount);
}

void Miner::badShareCountChanged(quint32 _badShareCount) {
  Q_EMIT badShareCountChangedSignal(_badShareCount);
}

void Miner::connectionErrorCountChanged(quint32 _connectionErrorCount) {
  Q_EMIT connectionErrorCountChangedSignal(_connectionErrorCount);
}

void Miner::lastConnectionErrorTimeChanged(const QDateTime& _lastConnectionErrorTime) {
  Q_EMIT lastConnectionErrorTimeChangedSignal(_lastConnectionErrorTime);
}

void Miner::timerEvent(QTimerEvent* _event) {
  if (_event->timerId() == m_hashRateTimerId) {
    m_hashCountPerSecond = m_hashCounter;
    m_alternateHashCountPerSecond = m_alternameHashCounter;
    m_hashCounter = 0;
    m_alternameHashCounter = 0;
    Q_EMIT hashRateChangedSignal(m_hashCountPerSecond);
    Q_EMIT alternateHashRateChangedSignal(m_alternateHashCountPerSecond);
    return;
  }

  QObject::timerEvent(_event);
}

void Miner::setState(State _newState) {
  if (m_minerState != _newState) {
    m_minerState = _newState;
    Q_EMIT stateChangedSignal(m_minerState);
  }
}

}
