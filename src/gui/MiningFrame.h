// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Copyright (c) 2016-2022 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <QFrame>
#include "qcustomplot.h"
#include "Miner.h"
#include <Logging/LoggerMessage.h>

class QAbstractButton;

namespace Ui {
class MiningFrame;
}

class Miner;

namespace WalletGui {

class MiningFrame : public QFrame {
  Q_OBJECT

public:
  MiningFrame(QWidget* _parent);
  ~MiningFrame();

  void addPoint(double x, double y);
  void plot();

  bool isSoloRunning() const;

protected:
  void timerEvent(QTimerEvent* _event) Q_DECL_OVERRIDE;

private:
  QScopedPointer<Ui::MiningFrame> m_ui;
  int m_soloHashRateTimerId;
  int m_minerRoutineTimerId;
  QVector<double> m_hX, m_hY;
  std::unique_ptr<Miner> m_miner;
  QString m_miner_log;

  void initCpuCoreList();
  void startSolo();
  void stopSolo();

  bool m_wallet_closed = false;
  bool m_solo_mining = false;
  bool m_sychronized = false;
  bool m_mining_was_stopped = false;

  void walletOpened();
  void walletClosed();
  quint32 getHashRate() const;
  double m_maxHr = 10;

  Q_SLOT void startStopSoloClicked(QAbstractButton* _button);
  Q_SLOT void enableSolo();
  Q_SLOT void setMiningThreads();
  Q_SLOT void onBlockHeightUpdated();
  Q_SLOT void onSynchronizationCompleted();
  Q_SLOT void updateBalance(quint64 _balance);
  Q_SLOT void updatePendingBalance(quint64 _balance);
  Q_SLOT void updateMinerLog(const QString& _message);
  Q_SLOT void coreDealTurned(int _cores);
  Q_SLOT void poolChanged();
};

}
