// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <QObject>

#include <atomic>

class QReadWriteLock;

namespace WalletGui {

struct Job {
  QString jobId;
  quint32 target;
  QByteArray blob;
};

class IWorkerObserver {
public:
  virtual void processShare(const QString& _jobId, quint32 _nonce, const QByteArray& _result) = 0;
};

class Worker : public QObject {
  Q_OBJECT

public:
  Worker(QObject* _parent, IWorkerObserver* _observer, Job& _currentJob, QReadWriteLock& _jobLock, std::atomic<quint32>& _nonce,
    std::atomic<quint32>& _hashCounter);

  void start();
  void stop();

private:
  IWorkerObserver* m_observer;
  Job& m_currentJob;
  QReadWriteLock& m_jobLock;
  std::atomic<quint32>& m_nonce;
  std::atomic<quint32>& m_hashCounter;
  std::atomic<bool> m_isStopped;

  void run();

Q_SIGNALS:
  void runSignal();
};

}
