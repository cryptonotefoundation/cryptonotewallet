// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QDebug>
#include <QReadWriteLock>
#include <QThread>

#include <crypto/hash.h>

#include "Worker.h"

namespace WalletGui {

Worker::Worker(QObject *parent, IWorkerObserver* _observer, Job& _currentJob, QReadWriteLock& _jobLock, std::atomic<quint32>& _nonce,
  std::atomic<quint32>& _hashCounter) : QObject(parent),
  m_observer(_observer), m_currentJob(_currentJob), m_jobLock(_jobLock), m_nonce(_nonce), m_hashCounter(_hashCounter), m_isStopped(true) {
  connect(this, &Worker::runSignal, this, &Worker::run, Qt::QueuedConnection);
}

void Worker::start() {
  m_isStopped = false;
  Q_EMIT runSignal();
}

void Worker::stop() {
  m_isStopped = true;
}

void Worker::run() {
  Job localJob;
  quint32 localNonce;
  Crypto::Hash hash;
  Crypto::cn_context context;
  while (!m_isStopped) {
    {
      QReadLocker lock(&m_jobLock);
      if (m_currentJob.jobId.isEmpty()) {
        lock.unlock();
        QThread::msleep(100);
        continue;
      }

      if (localJob.jobId != m_currentJob.jobId) {
        localJob = m_currentJob;
      }
    }

    localNonce = ++m_nonce;
    localJob.blob.replace(39, sizeof(localNonce), reinterpret_cast<char*>(&localNonce), sizeof(localNonce));
    std::memset(&hash, 0, sizeof(hash));
    Crypto::cn_slow_hash(context, localJob.blob.data(), localJob.blob.size(), hash);
    ++m_hashCounter;
    if (Q_UNLIKELY(((quint32*)&hash)[7] < localJob.target)) {
      m_observer->processShare(localJob.jobId, localNonce, QByteArray(reinterpret_cast<char*>(&hash), sizeof(hash)));
    }
  }
}

}
