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

#include <QAbstractButton>
#include <QDataWidgetMapper>
#include <QMessageBox>
#include <QMetaMethod>
#include <QThread>
#include <QUrl>

#include "MiningFrame.h"
#include "Settings/Settings.h"
#include "ApplicationEventHandler.h"
#include "Gui/Common/QuestionDialog.h"
#include "Gui/Common/RightAlignmentColumnDelegate.h"
#include "IMiningManager.h"
#include "IPoolMiner.h"
#include "MinerDelegate.h"
#include "Models/AddressBookModel.h"
#include "Models/MinerModel.h"
#include "PoolHeaderView.h"
#include "Style/Style.h"

#include "ui_MiningFrame.h"

namespace WalletGui {

namespace  {

const char MINING_FRAME_STYLE_SHEET_TEMPLATE[] =
  "* {"
    "font-family: %fontFamily%;"
  "}"

  "WalletGui--MiningFrame {"
    "background-color: #ffffff;"
  "}"

  "WalletGui--MiningFrame #m_miningManageFrame {"
    "background-color: %backgroundColorGray%;"
    "border: none;"
    "border-bottom: 1px solid %borderColor%;"
  "}"

  "WalletGui--MiningFrame #m_miningManageFrame #m_cpuCoresCombo {"
    "background-color: #ffffff;"
    "min-width: 40px;"
    "max-width: 40px;"
  "}"

  "WalletGui--MiningFrame #m_addPoolFrame {"
    "border: none;"
    "border-top: 1px solid %borderColor%;"
  "}";


const int NAVIGATION_SECTION_WIDTH = 50;

bool isIpOrHostName(const QString& _string) {
  QRegExp ipRegExp("^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$");
  QRegExp hostNameRegExp("^(([a-zA-Z0-9]|[a-zA-Z0-9][a-zA-Z0-9\\-]*[a-zA-Z0-9])\\.)*([A-Za-z0-9]|[A-Za-z0-9][A-Za-z0-9\\-]*[A-Za-z0-9])$");
  return !_string.isEmpty() && (ipRegExp.exactMatch(_string) || hostNameRegExp.exactMatch(_string));
}

}

MiningFrame::MiningFrame(QWidget* _parent) : QFrame(_parent), m_ui(new Ui::MiningFrame), m_cryptoNoteAdapter(nullptr),
  m_miningManager(nullptr), m_donationManager(nullptr), m_minerModel(nullptr),
  m_dataMapper(new QDataWidgetMapper(this)), m_restorePoolListButton(new WalletLargeBlueButton(this)) {
  m_ui->setupUi(this);
  m_ui->m_hashRateFrame->hide();
  m_ui->m_lockedScreenMiningCheck->hide();
  m_ui->m_poolView->setHeader(new PoolHeaderView(this));
  m_ui->m_poolView->setAcceptDrops(true);

  m_restorePoolListButton->setFixedWidth(200);
  m_restorePoolListButton->setText(tr("Restore default pool list"));
  m_restorePoolListButton->hide();

  connect(m_restorePoolListButton, &WalletLargeBlueButton::clicked, m_restorePoolListButton, &WalletLargeBlueButton::hide);
  connect(m_restorePoolListButton, &WalletLargeBlueButton::clicked, this, &MiningFrame::restoreDefaultPoolsClicked);
  setStyleSheet(Settings::instance().getCurrentStyle().makeStyleSheet(MINING_FRAME_STYLE_SHEET_TEMPLATE));
}

MiningFrame::~MiningFrame() {
}

void MiningFrame::setCryptoNoteAdapter(ICryptoNoteAdapter* _cryptoNoteAdapter) {
  m_cryptoNoteAdapter = _cryptoNoteAdapter;
}

void MiningFrame::setApplicationEventHandler(IApplicationEventHandler* _applicationEventHandler) {
  _applicationEventHandler->addObserver(this);
}

void MiningFrame::setDonationManager(IDonationManager* _donationManager) {
  m_donationManager = _donationManager;
  m_donationManager->addObserver(this);
}

void MiningFrame::setMiningManager(IMiningManager* _miningManager) {
  m_miningManager = _miningManager;
  initCpuCoresCombo();
  if (m_miningManager->getSchedulePolicy() == IMiningManager::POLICY_FAILOVER) {
    m_ui->m_failoverStrategyRadio->setChecked(true);
  } else if (m_miningManager->getSchedulePolicy() == IMiningManager::POLICY_RANDOM) {
    m_ui->m_randomStrategyRadio->setChecked(true);
  }

  m_miningManager->addObserver(this);
}

void MiningFrame::setMainWindow(QWidget* _mainWindow) {
  m_mainWindow = _mainWindow;
}

void MiningFrame::setMinerModel(QAbstractItemModel* _model) {
  m_minerModel = _model;
  m_ui->m_poolView->setModel(m_minerModel);
  RightAlignmentColumnDelegate* delegate = new RightAlignmentColumnDelegate(false, this);
  m_ui->m_poolView->setItemDelegateForColumn(MinerModel::COLUMN_DIFFICULTY, delegate);
  m_ui->m_poolView->setItemDelegateForColumn(MinerModel::COLUMN_GOOD_SHARES, delegate);
  m_ui->m_poolView->setItemDelegateForColumn(MinerModel::COLUMN_BAD_SHARES, delegate);
  m_ui->m_poolView->setItemDelegateForColumn(MinerModel::COLUMN_DONATED_SHARES, delegate);
  m_ui->m_poolView->setItemDelegateForColumn(MinerModel::COLUMN_CONNECTION_ERROR_COUNT, delegate);
  m_ui->m_poolView->setItemDelegateForColumn(MinerModel::COLUMN_LAST_CONNECTION_ERROR_TIME, delegate);
  m_ui->m_poolView->header()->setSectionResizeMode(MinerModel::COLUMN_POOL_URL, QHeaderView::Stretch);
  m_ui->m_poolView->header()->setSectionResizeMode(MinerModel::COLUMN_DIFFICULTY, QHeaderView::Fixed);
  m_ui->m_poolView->header()->setSectionResizeMode(MinerModel::COLUMN_GOOD_SHARES, QHeaderView::Fixed);
  m_ui->m_poolView->header()->setSectionResizeMode(MinerModel::COLUMN_BAD_SHARES, QHeaderView::Fixed);
  m_ui->m_poolView->header()->setSectionResizeMode(MinerModel::COLUMN_DONATED_SHARES, QHeaderView::Fixed);
  m_ui->m_poolView->header()->setSectionResizeMode(MinerModel::COLUMN_CONNECTION_ERROR_COUNT, QHeaderView::Fixed);
  m_ui->m_poolView->header()->setSectionResizeMode(MinerModel::COLUMN_LAST_CONNECTION_ERROR_TIME, QHeaderView::Fixed);
  m_ui->m_poolView->header()->setSectionResizeMode(MinerModel::COLUMN_REMOVE, QHeaderView::Fixed);
  m_ui->m_poolView->header()->resizeSection(MinerModel::COLUMN_DIFFICULTY, 90);
  m_ui->m_poolView->header()->resizeSection(MinerModel::COLUMN_GOOD_SHARES, 120);
  m_ui->m_poolView->header()->resizeSection(MinerModel::COLUMN_BAD_SHARES, 120);
  m_ui->m_poolView->header()->resizeSection(MinerModel::COLUMN_DONATED_SHARES, 130);
  m_ui->m_poolView->header()->resizeSection(MinerModel::COLUMN_CONNECTION_ERROR_COUNT, 160);
  m_ui->m_poolView->header()->resizeSection(MinerModel::COLUMN_LAST_CONNECTION_ERROR_TIME, 160);
  m_ui->m_poolView->header()->resizeSection(MinerModel::COLUMN_REMOVE, 60);
  m_ui->m_poolView->header()->hideSection(MinerModel::COLUMN_HASHRATE);
  m_ui->m_poolView->header()->hideSection(MinerModel::COLUMN_DONATION_HASHRATE);
  m_ui->m_poolView->setLinkLikeColumnSet(QSet<int>() << MinerModel::COLUMN_REMOVE);
  m_ui->m_poolView->setItemDelegateForColumn(MinerModel::COLUMN_REMOVE, new MinerRemoveDelegate(m_ui->m_poolView, this));

  m_dataMapper->setModel(m_minerModel);
  m_dataMapper->addMapping(m_ui->m_hashrateLabel, MinerModel::COLUMN_HASHRATE, "text");
  m_dataMapper->addMapping(m_ui->m_donationHashRateLabel, MinerModel::COLUMN_DONATION_HASHRATE, "text");

  connect(m_minerModel, &MinerModel::rowsRemoved, this, &MiningFrame::showRestoreButton);
}

void MiningFrame::donationManagerOpened() {
  donationMiningEnabled(m_donationManager->isDonationMiningEnabled());
}

void MiningFrame::donationManagerClosed() {
  if (m_ui->m_startMiningButton->isChecked()) {
    m_ui->m_startMiningButton->click();
  }
}

void MiningFrame::donationMiningEnabled(bool _on) {
  if (_on) {
    m_ui->m_poolView->header()->showSection(MinerModel::COLUMN_DONATED_SHARES);
    m_ui->m_donationHashRateTextLabel->show();
    m_ui->m_donationHashRateLabel->show();
  } else {
    m_ui->m_poolView->header()->hideSection(MinerModel::COLUMN_DONATED_SHARES);
    m_ui->m_donationHashRateTextLabel->hide();
    m_ui->m_donationHashRateLabel->hide();
  }
}

void MiningFrame::donationMiningAddressChanged(const QString& _address) {
  // Do nothing
}

void MiningFrame::donationMiningAmountChanged(int _amount) {
  // Do nothing
}

void MiningFrame::donationChangeEnabled(bool _on) {
  // Do nothing
}

void MiningFrame::donationChangeAddressChanged(const QString& _address) {
  // Do nothing
}

void MiningFrame::donationChangeAmountChanged(int _amount) {
  // Do nothing
}

void MiningFrame::minersLoaded() {
  if (m_miningManager->getMinerCount() == 0) {
    showRestoreButton();
  }
}

void MiningFrame::minersUnloaded() {
  if (m_restorePoolListButton->isVisible()) {
    m_restorePoolListButton->hide();
  }
}

void MiningFrame::miningStarted() {
  m_ui->m_cpuCoresCombo->setEnabled(false);
  m_ui->m_failoverStrategyRadio->setEnabled(false);
  m_ui->m_randomStrategyRadio->setEnabled(false);
  if (!m_ui->m_startMiningButton->isChecked()) {
    m_ui->m_startMiningButton->setChecked(true);
    m_ui->m_startMiningButton->setText(tr("Stop"));
    m_ui->m_startMiningButton->setFixedWidth(50);
    m_ui->m_hashrateLabel->setText("0 H/s");
    m_ui->m_donationHashRateLabel->setText("0 H/s");
    m_ui->m_hashRateFrame->setVisible(true);
  }
}

void MiningFrame::miningStopped() {
  m_ui->m_cpuCoresCombo->setEnabled(true);
  m_ui->m_failoverStrategyRadio->setEnabled(true);
  m_ui->m_randomStrategyRadio->setEnabled(true);
  if (m_ui->m_startMiningButton->isChecked()) {
    m_ui->m_startMiningButton->setChecked(false);
    m_ui->m_startMiningButton->setText(tr("Start mining"));
    m_ui->m_startMiningButton->setFixedWidth(190);
    m_ui->m_hashRateFrame->setVisible(false);
  }
}

void MiningFrame::activeMinerChanged(quintptr _minerIndex) {
  m_ui->m_hashrateLabel->clear();
  m_ui->m_donationHashRateLabel->clear();
  m_dataMapper->setCurrentIndex(_minerIndex);
}

void MiningFrame::schedulePolicyChanged(int _schedulePolicy) {
  // Do nothing
}

void MiningFrame::cpuCoreCountChanged(quint32 _cpuCoreCount) {
  // Do nothing
}

void MiningFrame::minerAdded(quintptr _minerIndex) {
  if (m_restorePoolListButton->isVisible()) {
    m_restorePoolListButton->hide();
  }
}

void MiningFrame::minerRemoved(quintptr _minerIndex) {
  if (m_miningManager->getMinerCount() == 0) {
    showRestoreButton();
  }
}

void MiningFrame::stateChanged(quintptr _minerIndex, int _newState) {
  // Do nothing
}

void MiningFrame::hashRateChanged(quintptr _minerIndex, quint32 _hashRate) {
  // Do nothing
}

void MiningFrame::alternateHashRateChanged(quintptr _minerIndex, quint32 _hashRate) {
  // Do nothing
}

void MiningFrame::difficultyChanged(quintptr _minerIndex, quint32 _difficulty) {
  // Do nothing
}

void MiningFrame::goodShareCountChanged(quintptr _minerIndex, quint32 _goodShareCount) {
  // Do nothing
}

void MiningFrame::goodAlternateShareCountChanged(quintptr _minerIndex, quint32 _goodShareCount) {
  // Do nothing
}

void MiningFrame::badShareCountChanged(quintptr _minerIndex, quint32 _badShareCount) {
  // Do nothing
}

void MiningFrame::connectionErrorCountChanged(quintptr _minerIndex, quint32 _connectionErrorCount) {
  // Do nothing
}

void MiningFrame::lastConnectionErrorTimeChanged(quintptr _minerIndex, const QDateTime& _lastConnectionErrorTime) {
  // Do nothing
}

void MiningFrame::urlReceived(const QUrl& _url) {
  // Do nothing
}

void MiningFrame::screenLocked() {
  if (m_ui->m_lockedScreenMiningCheck->isChecked() && !m_ui->m_startMiningButton->isChecked()) {
    m_ui->m_startMiningButton->click();
  }
}

void MiningFrame::screenUnlocked() {
  if (m_ui->m_lockedScreenMiningCheck->isChecked() && m_ui->m_startMiningButton->isChecked()) {
    m_ui->m_startMiningButton->click();
  }
}

void MiningFrame::resizeEvent(QResizeEvent* _event) {
  QFrame::resizeEvent(_event);
  if (m_restorePoolListButton->isVisible()) {
    showRestoreButton();
  }
}

void MiningFrame::initCpuCoresCombo() {
  int maxCpuCoreCount = QThread::idealThreadCount();
  if (maxCpuCoreCount == -1) {
    maxCpuCoreCount = 2;
  }

  for (int i = 0; i < maxCpuCoreCount; ++i) {
    m_ui->m_cpuCoresCombo->addItem(QString::number(i + 1), i + 1);
  }

  m_ui->m_cpuCoresCombo->setCurrentIndex(m_miningManager->getCpuCoreCount() - 1);
}

void MiningFrame::restoreDefaultPoolsClicked() {
  m_miningManager->restoreDefaultMinerList();
}

void MiningFrame::startMining() {
  m_miningManager->startMining();
}

void MiningFrame::stopMining() {
  m_miningManager->stopMining();
}

void MiningFrame::showRestoreButton() {
  if (m_minerModel->rowCount() == 0) {
    QPoint poolViewCenter = m_ui->m_poolView->geometry().center();
    poolViewCenter.setY(m_ui->m_poolView->y() + 60);

    QRect restoreButtonRect = m_restorePoolListButton->rect();
    restoreButtonRect.moveCenter(poolViewCenter);
    m_restorePoolListButton->show();
    m_restorePoolListButton->setGeometry(restoreButtonRect);
    m_restorePoolListButton->raise();
  }
}

void MiningFrame::addPoolClicked() {
  m_miningManager->addMiner(m_ui->m_newPoolHostEdit->text().trimmed(), m_ui->m_newPoolPortSpin->value(), m_ui->m_difficultySpin->value());
  m_ui->m_newPoolHostEdit->clear();
  m_ui->m_newPoolPortSpin->setValue(m_ui->m_newPoolPortSpin->minimum());
}

void MiningFrame::cpuCoreCountChangedByUser(int _cpuCoreCount) {
  m_miningManager->setCpuCoreCount(_cpuCoreCount + 1);
}

void MiningFrame::deleteClicked(const QModelIndex& _index) {
  if (!_index.isValid() || _index.column() != MinerModel::COLUMN_REMOVE) {
    return;
  }

  if (_index.data(MinerModel::ROLE_STATE).toInt() == IPoolMiner::STATE_RUNNING) {
    QMessageBox::warning(m_mainWindow, tr("Warning"), tr("Can't remove active pool."));
    return;
  }

  QuestionDialog dlg(tr("Delete pool"), tr("Pool will be removed from list. Are you sure?"), m_mainWindow);
  if (dlg.exec() == QDialog::Accepted) {
    m_miningManager->removeMiner(_index.row());
  }
}

void MiningFrame::miningOnLockedScreenChecked(bool _enabled) {
  Settings::instance().setMiningOnLockedScreenEnabled(_enabled);
}

void MiningFrame::newPoolUrlChanged() {
  m_ui->m_addPoolButton->setEnabled(isIpOrHostName(m_ui->m_newPoolHostEdit->text().trimmed()));
}

void MiningFrame::schedulePolicyChanged(QAbstractButton* _button, bool _checked) {
  if (!_checked) {
    return;
  }

  if (_button == m_ui->m_failoverStrategyRadio) {
    m_miningManager->setSchedulePolicy(IMiningManager::POLICY_FAILOVER);
  } else if (_button == m_ui->m_randomStrategyRadio) {
    m_miningManager->setSchedulePolicy(IMiningManager::POLICY_RANDOM);
  }
}

void MiningFrame::startMiningClicked(bool _on) {
  if (_on) {
    m_ui->m_startMiningButton->setText(tr("Stop"));
    m_ui->m_startMiningButton->setFixedWidth(50);
    m_ui->m_hashrateLabel->setText("0 H/s");
    m_ui->m_donationHashRateLabel->setText("0 H/s");
    startMining();
  } else {
    m_ui->m_startMiningButton->setText(tr("Start mining"));
    m_ui->m_startMiningButton->setFixedWidth(190);
    stopMining();
  }

  m_ui->m_hashRateFrame->setVisible(_on);
}

}
