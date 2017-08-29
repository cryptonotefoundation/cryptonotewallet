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
#include <QMutex>
#include <QObject>

#include <atomic>

#include "IMinerWorker.h"

class QReadWriteLock;

namespace Crypto {
  struct Hash;
  class cn_context;
}

namespace WalletGui {

class Worker : public QObject, public IMinerWorker {
  Q_OBJECT
  Q_DISABLE_COPY(Worker)

public:
  Worker(Job& _mainJob, Job& _alternateJob, QReadWriteLock& _mainJobLock, QReadWriteLock& _alternateJobLock,
    std::atomic<quint32>& _mainNonce, std::atomic<quint32>& _alternateNonce, std::atomic<quint32>& _alternateProbability,
    std::atomic<quint32>& _hashCounter, std::atomic<quint32>& _alternateHashCounter, QObject* _parent);
  ~Worker();

  virtual void start() override;
  virtual void stop() override;
  virtual void addObserver(IMinerWorkerObserver* _observer) override;
  virtual void removeObserver(IMinerWorkerObserver* _observer) override;
  virtual void addAlternateObserver(IMinerWorkerObserver* _observer) override;
  virtual void removeAlternateObserver(IMinerWorkerObserver* _observer) override;

private:
  Job& m_mainJob;
  Job& m_alternateJob;
  QReadWriteLock& m_mainJobLock;
  QReadWriteLock& m_alternateJobLock;
  std::atomic<quint32>& m_mainNonce;
  std::atomic<quint32>& m_alternateNonce;
  std::atomic<quint32>& m_hashCounter;
  std::atomic<quint32>& m_alternateHashCounter;
  std::atomic<quint32>& m_alternateProbability;
  std::atomic<bool> m_isStopped;
  QMutex m_alternateObserverMutex;
  QMap<IMinerWorkerObserver*, QList<QMetaObject::Connection>> m_observerConnections;
  QMap<IMinerWorkerObserver*, QList<QMetaObject::Connection>> m_alternateObserverConnections;

  Q_INVOKABLE void run();
  void mainJobMiningRound(Job& _localJob, quint32& _localNonce, Crypto::Hash& _hash, Crypto::cn_context& _context);
  void alternateJobMiningRound(Job& _localJob, quint32& _localNonce, Crypto::Hash& _hash, Crypto::cn_context& _context);

Q_SIGNALS:
  void shareFoundSignal(const QString& _jobId, quint32 _nonce, const QByteArray& _result);
  void alternateShareFoundSignal(const QString& _jobId, quint32 _nonce, const QByteArray& _result);
};

}
