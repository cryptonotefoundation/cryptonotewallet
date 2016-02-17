// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QThread>
#include <QUrl>

#include "MiningFrame.h"
#include "MainWindow.h"
#include "Miner.h"
#include "NewPoolDialog.h"
#include "PoolModel.h"
#include "Settings.h"
#include "WalletAdapter.h"

#include "ui_miningframe.h"

namespace WalletGui {

const quint32 HASHRATE_TIMER_INTERVAL = 1000;

MiningFrame::MiningFrame(QWidget* _parent) : QFrame(_parent), m_ui(new Ui::MiningFrame), m_miner(nullptr),
  m_poolModel(new PoolModel(this)), m_hashRateTimerId(-1) {
  m_ui->setupUi(this);
  m_ui->m_poolCombo->setModel(m_poolModel);
  m_ui->m_poolCombo->setCurrentIndex(0);
  initCpuCoreList();

  connect(&WalletAdapter::instance(), &WalletAdapter::walletCloseCompletedSignal, this, &MiningFrame::walletClosed, Qt::QueuedConnection);
}

MiningFrame::~MiningFrame() {
  stopMining();
}

void MiningFrame::timerEvent(QTimerEvent* _event) {
  if (_event->timerId() == m_hashRateTimerId) {
    quint32 hashRate = m_miner->getHashRate();
    if (hashRate == 0) {
      return;
    }

    m_ui->m_stateLabel->setText(tr("Mining. Hashrate: %1 H/s").arg(hashRate));
    return;
  }

  QFrame::timerEvent(_event);
}

void MiningFrame::initCpuCoreList() {
  int cpuCoreCount = QThread::idealThreadCount();
    if (cpuCoreCount == -1) {
      cpuCoreCount = 2;
    }

    for (int i = 0; i < cpuCoreCount; ++i) {
      m_ui->m_cpuCombo->addItem(QString::number(i + 1), i + 1);
    }

    m_ui->m_cpuCombo->setCurrentIndex((cpuCoreCount - 1) / 2);
}

void MiningFrame::startMining() {
  Q_ASSERT(m_miner == nullptr);
  QUrl poolUrl = QUrl::fromUserInput(m_ui->m_poolCombo->currentText());
  if (!poolUrl.isValid()) {
    m_ui->m_stopButton->setChecked(true);
  }

  m_miner = new Miner(this, poolUrl.host(), poolUrl.port(), WalletAdapter::instance().getAddress());
  connect(m_miner, &Miner::socketErrorSignal, this, [this](const QString& _errorString) {
    m_ui->m_stateLabel->setText(tr("Error: %1").arg(_errorString));
  });

  m_ui->m_stateLabel->setText(tr("Starting..."));
  m_miner->start(m_ui->m_cpuCombo->currentData().toUInt());
  m_hashRateTimerId = startTimer(HASHRATE_TIMER_INTERVAL);
  m_ui->m_poolCombo->setEnabled(false);
  m_ui->m_cpuCombo->setEnabled(false);
}

void MiningFrame::stopMining() {
  if (m_miner == nullptr) {
    return;
  }

  killTimer(m_hashRateTimerId);
  m_hashRateTimerId = -1;
  m_miner->stop();
  m_miner->deleteLater();
  m_miner = nullptr;
  m_ui->m_stateLabel->setText(tr("Stopped"));
  m_ui->m_poolCombo->setEnabled(true);
  m_ui->m_cpuCombo->setEnabled(true);
}

void MiningFrame::walletClosed() {
  stopMining();
}

void MiningFrame::addPoolClicked() {
  NewPoolDialog dlg(&MainWindow::instance());
  if (dlg.exec() == QDialog::Accepted) {
    QString host = dlg.getHost();
    quint16 port = dlg.getPort();
    if (host.isEmpty()) {
      return;
    }

    m_poolModel->addPool(host, port);
  }
}

void MiningFrame::startStopClicked(QAbstractButton* _button) {
  if (_button == m_ui->m_startButton && m_ui->m_startButton->isChecked()) {
    startMining();
  } else if (_button == m_ui->m_stopButton && m_ui->m_stopButton->isChecked()) {
    stopMining();
  }
}

}
