// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <QObject>
#include <QReadWriteLock>

#include <atomic>

#include "Worker.h"

namespace WalletGui {

class StratumClient;

class Miner : public QObject {
  Q_OBJECT

public:
  Miner(QObject* _parent, const QString& _host, quint16 _port, const QString& _login, const QString& _password = "x");
  ~Miner();

  void start(quint32 _coreCount);
  void stop();
  bool isRunning() const;

  QString getPoolHost() const;
  quint16 getPoolPort() const;
  quint32 getHashRate() const;

protected:
  void timerEvent(QTimerEvent* _event) Q_DECL_OVERRIDE;

private:
  Job m_currentJob;
  QReadWriteLock m_jobLock;
  StratumClient* m_stratumClient;
  std::atomic<quint32> m_nonce;
  std::atomic<quint32> m_hashCounter;
  quint32 m_hashCountPerSecond;
  QList<QPair<QThread*, Worker*> > m_workerThreadList;
  int m_hashRateTimerId;

Q_SIGNALS:
  void socketErrorSignal(const QString& _errorText);
};

}
