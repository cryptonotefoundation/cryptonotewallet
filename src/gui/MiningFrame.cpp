// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Copyright (c) 2016-2022 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QDebug>
#include <QThread>
#include <QUrl>
#include <QtConcurrent/QtConcurrent>
#include <QtConcurrent/QtConcurrentRun>
#include <QDebug>

#include "MiningFrame.h"
#include "MainWindow.h"
#include "Settings.h"
#include "WalletAdapter.h"
#include "NodeAdapter.h"
#include "CryptoNoteWrapper.h"
#include "CurrencyAdapter.h"
#include "Settings.h"
#include "Logging/LoggerManager.h"
#include "LoggerAdapter.h"

#include "ui_miningframe.h"

namespace WalletGui {

const quint32 HASHRATE_TIMER_INTERVAL = 1000;
const quint32 MINER_ROUTINE_TIMER_INTERVAL = 60000;

MiningFrame::MiningFrame(QWidget* _parent) :
    QFrame(_parent), m_ui(new Ui::MiningFrame),
    m_miner(new Miner(this, LoggerAdapter::instance().getLoggerManager())),
    m_soloHashRateTimerId(-1) {
  m_ui->setupUi(this);
  initCpuCoreList();

  QFont fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
  fixedFont.setStyleHint(QFont::TypeWriter);
  m_ui->m_minerLog->setFont(fixedFont);

  m_ui->m_startSolo->setEnabled(false);
  m_ui->m_stopSolo->setEnabled(false);

  NodeType node = NodeAdapter::instance().getNodeType();
  if (node != NodeType::IN_PROCESS) {
    m_ui->m_startSolo->setDisabled(true);
  }

  m_ui->m_hashRateChart->addGraph();
  m_ui->m_hashRateChart->graph(0)->setScatterStyle(QCPScatterStyle::ssDot);
  m_ui->m_hashRateChart->graph(0)->setLineStyle(QCPGraph::lsLine);
  m_ui->m_hashRateChart->graph()->setPen(QPen(QRgb(0xe3e1e3)));
  m_ui->m_hashRateChart->graph()->setBrush(QBrush(QRgb(0x0b1620)));

  QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
  dateTicker->setDateTimeFormat("hh:mm:ss");
  m_ui->m_hashRateChart->xAxis->setTicker(dateTicker);
  m_ui->m_hashRateChart->yAxis->setRange(0, m_maxHr);
  m_ui->m_hashRateChart->yAxis->setLabel("Hashrate");
  m_ui->m_hashRateChart->xAxis->setTickPen(QPen(QRgb(0xe3e1e3)));
  m_ui->m_hashRateChart->xAxis->setLabelColor(QRgb(0xe3e1e3));

  // make top and right axes visible but without ticks and labels
  m_ui->m_hashRateChart->xAxis2->setVisible(true);
  m_ui->m_hashRateChart->yAxis2->setVisible(true);
  m_ui->m_hashRateChart->xAxis2->setTicks(false);
  m_ui->m_hashRateChart->yAxis2->setTicks(false);
  m_ui->m_hashRateChart->xAxis2->setTickLabels(false);
  m_ui->m_hashRateChart->yAxis2->setTickLabels(false);

  m_ui->m_hashRateChart->setBackground(QBrush(QRgb(0x19232d)));

  addPoint(QDateTime::currentDateTime().toTime_t(), 0);
  plot();

  connect(&WalletAdapter::instance(), &WalletAdapter::walletCloseCompletedSignal, this, &MiningFrame::walletClosed, Qt::QueuedConnection);
  connect(&WalletAdapter::instance(), &WalletAdapter::walletInitCompletedSignal, this, &MiningFrame::walletOpened, Qt::QueuedConnection);
  connect(&WalletAdapter::instance(), &WalletAdapter::walletActualBalanceUpdatedSignal, this, &MiningFrame::updateBalance, Qt::QueuedConnection);
  connect(&WalletAdapter::instance(), &WalletAdapter::walletPendingBalanceUpdatedSignal, this, &MiningFrame::updatePendingBalance, Qt::QueuedConnection);
  connect(&WalletAdapter::instance(), &WalletAdapter::walletSynchronizationCompletedSignal, this, &MiningFrame::onSynchronizationCompleted, Qt::QueuedConnection);
  connect(&NodeAdapter::instance(), &NodeAdapter::localBlockchainUpdatedSignal, this, &MiningFrame::onBlockHeightUpdated, Qt::QueuedConnection);
  connect(&NodeAdapter::instance(), &NodeAdapter::poolChangedSignal, this, &MiningFrame::poolChanged,Qt::QueuedConnection);
  connect(&*m_miner, &Miner::minerMessageSignal, this, &MiningFrame::updateMinerLog, Qt::QueuedConnection);
}

MiningFrame::~MiningFrame() {
  stopSolo();
}

void MiningFrame::addPoint(double x, double y)
{
  m_hX.append(x);
  m_hY.append(y);

  // scroll plot
  if (m_hX.size() > 1000 || m_hY.size() > 1000) {
    m_hX.pop_front();
    m_hY.pop_front();
  }
}

void MiningFrame::plot()
{
  m_maxHr = std::max<double>(m_maxHr, m_hY.at(m_hY.size()-1));
  m_ui->m_hashRateChart->graph(0)->setData(m_hX, m_hY);
  m_ui->m_hashRateChart->xAxis->setRange(m_hX.at(0), m_hX.at(m_hX.size()-1));
  m_ui->m_hashRateChart->yAxis->setRange(0, m_maxHr);
  m_ui->m_hashRateChart->replot();
  m_ui->m_hashRateChart->update();
}

void MiningFrame::timerEvent(QTimerEvent* _event) {
  if (_event->timerId() == m_soloHashRateTimerId) {
    m_miner->merge_hr();
    double hashRate = m_miner->get_speed();
    if (hashRate == 0) {
      return;
    }
    m_ui->m_soloLabel->setText(tr("Mining"));
    m_ui->m_hashratelcdNumber->display(hashRate);
    addPoint(QDateTime::currentDateTime().toTime_t(), hashRate);
    plot();

    return;
  }
  if (_event->timerId() == m_minerRoutineTimerId) {
    QDateTime date = QDateTime::currentDateTime();
    QString formattedTime = date.toString("dd.MM.yyyy hh:mm:ss");
    qDebug() << formattedTime << "Event, requesting block template";

    m_miner->on_block_chain_update();
  }

  QFrame::timerEvent(_event);
}

void MiningFrame::initCpuCoreList() {
  quint16 threads = Settings::instance().getMiningThreads();
  int cpuCoreCount = QThread::idealThreadCount();
  if (cpuCoreCount == -1) {
      cpuCoreCount = 2;
  }

  if (threads > 0) {
    m_ui->m_cpuCoresSpin->setValue(threads);
  } else {
    m_ui->m_cpuCoresSpin->setValue((cpuCoreCount - 1) / 2);
  }

  m_ui->m_cpuCoresSpin->setMaximum(cpuCoreCount);
  m_ui->m_cpuCoresSpin->setMinimum(1);
  m_ui->m_cpuDial->setMaximum(cpuCoreCount);
  m_ui->m_cpuDial->setMinimum(1);
}

void MiningFrame::walletOpened() {
  if(m_solo_mining)
    stopSolo();

  m_wallet_closed = false;

  quint64 difficulty = NodeAdapter::instance().getDifficulty();
  m_ui->m_difficulty->setText(QString(tr("%1")).arg(difficulty));
}

void MiningFrame::walletClosed() {
  //stopSolo();
  m_wallet_closed = true;
  //m_ui->m_startSolo->setEnabled(false);
  //m_ui->m_stopSolo->isChecked();

  // TODO: consider setting all dials to zeroes
}

bool MiningFrame::isSoloRunning() const {
  return m_solo_mining;
}

void MiningFrame::startSolo() {
  quint64 difficulty = NodeAdapter::instance().getDifficulty();
  m_ui->m_difficulty->setText(QString(tr("%1")).arg(difficulty));

  m_miner->start(m_ui->m_cpuCoresSpin->value());
  m_ui->m_soloLabel->setText(tr("Starting..."));
  m_soloHashRateTimerId = startTimer(HASHRATE_TIMER_INTERVAL);
  m_minerRoutineTimerId = startTimer(MINER_ROUTINE_TIMER_INTERVAL);
  addPoint(QDateTime::currentDateTime().toTime_t(), 0);
  m_ui->m_startSolo->setChecked(true);
  m_ui->m_startSolo->setEnabled(false);
  m_ui->m_stopSolo->setEnabled(true);
  m_ui->m_cpuCoresSpin->setEnabled(false);
  m_ui->m_cpuDial->setEnabled(false);
  m_solo_mining = true;
}

void MiningFrame::stopSolo() {
  if(m_solo_mining) {
    killTimer(m_soloHashRateTimerId);
    m_soloHashRateTimerId = -1;
    killTimer(m_minerRoutineTimerId);
    m_minerRoutineTimerId = -1;
    m_miner->stop();
    addPoint(QDateTime::currentDateTime().toTime_t(), 0);
    m_ui->m_soloLabel->setText(tr("Stopped"));
    m_ui->m_hashratelcdNumber->display(0.0);
    if (!m_wallet_closed) {
      m_ui->m_startSolo->setEnabled(true);
      m_ui->m_stopSolo->setEnabled(false);
      m_ui->m_cpuCoresSpin->setEnabled(true);
      m_ui->m_cpuDial->setEnabled(true);
    }
    m_solo_mining = false;
    m_mining_was_stopped = true;
  }
}

void MiningFrame::enableSolo() {
  m_sychronized = true;
  if (!m_solo_mining && !m_miner->is_mining()) {
    m_ui->m_startSolo->setEnabled(true);
    m_ui->m_stopSolo->setEnabled(false);
    if (!m_mining_was_stopped && Settings::instance().isMiningOnLaunchEnabled() && m_sychronized) {
      startSolo();
    }
  }
}

void MiningFrame::startStopSoloClicked(QAbstractButton* _button) {
  if (_button == m_ui->m_startSolo && m_ui->m_startSolo->isChecked() && m_wallet_closed != true) {
    startSolo();
  } else if (m_wallet_closed == true && _button == m_ui->m_stopSolo && m_ui->m_stopSolo->isChecked()) {
    m_ui->m_startSolo->setEnabled(false);
    stopSolo();
  } else if (_button == m_ui->m_stopSolo && m_ui->m_stopSolo->isChecked()) {
    stopSolo();
  }
}

void MiningFrame::setMiningThreads() {
  Settings::instance().setMiningThreads(m_ui->m_cpuCoresSpin->value());
}

void MiningFrame::onBlockHeightUpdated() {
  QDateTime date = QDateTime::currentDateTime();
  QString formattedTime = date.toString("dd.MM.yyyy hh:mm:ss");
  qDebug() << formattedTime << "Blockchain update, requesting block template";

  m_miner->on_block_chain_update();

  quint64 difficulty = NodeAdapter::instance().getDifficulty();
  m_ui->m_difficulty->setText(QString(tr("%1")).arg(difficulty));
}

void MiningFrame::onSynchronizationCompleted() {
  NodeType node = NodeAdapter::instance().getNodeType();
  if (node != NodeType::IN_PROCESS) {
    m_ui->m_startSolo->setEnabled(false);
    return;
  }
  enableSolo();
}

void MiningFrame::updateBalance(quint64 _balance) {
  Q_UNUSED(_balance);
}

void MiningFrame::updatePendingBalance(quint64 _balance) {
  Q_UNUSED(_balance);
}

void MiningFrame::updateMinerLog(const QString& _message) {
  QString message = _message + "\n";
  m_miner_log += message;

  m_ui->m_minerLog->setPlainText(m_miner_log);

  QScrollBar *sb = m_ui->m_minerLog->verticalScrollBar();
  sb->setValue(sb->maximum());
}

void MiningFrame::coreDealTurned(int _cores) {
  QTimer::singleShot(600, [this, _cores]() {
    Settings::instance().setMiningThreads(_cores);
  } );
}

void MiningFrame::poolChanged() {
  QDateTime date = QDateTime::currentDateTime();
  QString formattedTime = date.toString("dd.MM.yyyy hh:mm:ss");
  qDebug() << formattedTime << "Mempool changed, requesting block template";

  m_miner->on_block_chain_update();
}

}
