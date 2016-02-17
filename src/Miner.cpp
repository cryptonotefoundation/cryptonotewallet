// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QThread>
#include <QTimerEvent>

#include "Miner.h"
#include "StratumClient.h"

namespace WalletGui {

const int HASHRATE_TIMER_INTERVAL = 1000;

Miner::Miner(QObject* _parent, const QString& _host, quint16 _port, const QString& _login, const QString& _password) : QObject(_parent),
  m_currentJob(), m_jobLock(), m_nonce(0), m_hashCounter(0), m_hashCountPerSecond(0), m_hashRateTimerId(-1) {
  m_stratumClient = new StratumClient(this, m_currentJob, m_jobLock, m_nonce, _host, _port, _login, _password);
  connect(m_stratumClient, &StratumClient::socketErrorSignal, this, &Miner::socketErrorSignal);
}

Miner::~Miner() {
  stop();
}

void Miner::start(quint32 _coreCount) {
  m_stratumClient->start();
  if (m_hashRateTimerId == -1) {
    m_hashRateTimerId = startTimer(HASHRATE_TIMER_INTERVAL);
  }

  for (quint32 i = 0; i < _coreCount; ++i) {
    if (m_workerThreadList.size() < i + 1) {
      Worker* worker = new Worker(nullptr, m_stratumClient, m_currentJob, m_jobLock, m_nonce, m_hashCounter);
      QThread* thread = new QThread(this);
      connect(thread, &QThread::started, worker, &Worker::start);
      worker->moveToThread(thread);
      m_workerThreadList.append(qMakePair(thread, worker));
    }

    m_workerThreadList[i].first->start();
  }
}

void Miner::stop() {
  m_stratumClient->stop();
  if (m_hashRateTimerId != -1) {
    killTimer(m_hashRateTimerId);
    m_hashRateTimerId = -1;
  }

  m_hashCountPerSecond = 0;
  Q_FOREACH (auto& workerThread, m_workerThreadList) {
    workerThread.second->stop();
    workerThread.first->quit();
  }

  Q_FOREACH (auto& workerThread, m_workerThreadList) {
    if (workerThread.first->isRunning()) {
      workerThread.first->wait();
    }
  }
}

bool Miner::isRunning() const {
  Q_FOREACH (const auto& worketThread, m_workerThreadList) {
    if (worketThread.first->isRunning()) {
      return true;
    }
  }

  return false;
}

QString Miner::getPoolHost() const {
  return m_stratumClient->getPoolHost();
}

quint16 Miner::getPoolPort() const {
  return m_stratumClient->getPoolPort();
}

quint32 Miner::getHashRate() const {
  return m_hashCountPerSecond;
}

void Miner::timerEvent(QTimerEvent* _event) {
  if (_event->timerId() == m_hashRateTimerId) {
    m_hashCountPerSecond = m_hashCounter;
    m_hashCounter = 0;
    return;
  }

  QObject::timerEvent(_event);
}

}
