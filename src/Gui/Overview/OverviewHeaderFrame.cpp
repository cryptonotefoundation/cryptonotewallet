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

#include <QApplication>
#include <QClipboard>
#include <QDataWidgetMapper>
#include <QMetaMethod>
#include <QMovie>
#include <QtMath>

#include "OverviewHeaderFrame.h"
#include "Settings/Settings.h"
#include "OverviewHeaderGlassFrame.h"
#include "Gui/Common/LinkLikeColumnDelegate.h"
#include "Gui/Common/PoolTransactionDetailsDialog.h"
#include "ICryptoNoteAdapter.h"
#include "Models/MinerModel.h"
#include "Models/NodeStateModel.h"
#include "Models/TransactionPoolModel.h"
#include "Models/WalletStateModel.h"
#include "OverviewTransactionPoolModel.h"
#include "Style/Style.h"
#include "ui_OverviewHeaderFrame.h"

namespace WalletGui {

namespace {

const char OVERVIEW_HEADER_STYLE_SHEET_TEMPLATE[] =
  "* {"
    "font-family: %fontFamily%;"
  "}"

  "WalletGui--OverviewHeaderFrame {"
    "border: none;"
    "border-bottom: 1px solid %borderColor%;"
    "background-color: %backgroundColorGray%;"
    "min-height: 130px;"
    "max-height: 130px;"
  "}"

  "WalletGui--OverviewHeaderFrame #m_overviewBalanceFrame {"
    "border: none;"
    "border-right: 1px solid %borderColor%;"
  "}"

  "WalletGui--OverviewHeaderFrame #m_overviewMiningFrame {"
    "border: none;"
    "border-right: 1px solid %borderColor%;"
  "}"

  "WalletGui--OverviewHeaderFrame #m_overviewTransactionPoolView {"
    "background: transparent;"
    "margin-top: -4px;"
  "}"

  "WalletGui--OverviewHeaderFrame #m_overviewTransactionPoolView::item {"
    "min-height: 19px;"
    "border-right: 10px solid transparent;"
    "border-bottom: 2px solid transparent;"
  "}"

  "WalletGui--OverviewHeaderFrame WalletGui--WalletHeaderLabel {"
    "margin-top: 2px;"
  "}"

  "WalletGui--OverviewHeaderFrame #m_overviewPoolSizeLabel {"
    "margin-top: 3px;"
  "}"

  "WalletGui--OverviewHeaderFrame #m_startMiningButton {"
    "min-width: 97px;"
    "max-width: 97px;"
    "min-height: 23px;"
    "max-height: 23px;"
    "border: 1px solid %backgroundColorGreen%;"
    "background-color: %backgroundColorGreen%;"
    "color: #ffffff;"
    "image: url(:icons/start-mining);"
    "image-position: left;"
    "padding-left: 7px;"
  "}"

  "WalletGui--OverviewHeaderFrame #m_startMiningButton:hover {"
    "background-color: %backgroundColorGreenHover%;"
  "}"

  "WalletGui--OverviewHeaderFrame #m_startMiningButton:checked {"
    "border: 1px solid %borderColor%;"
    "background-color: %backgroundColorGray%;"
    "color: %fontColorGray%;"
    "image: url(:icons/stop-mining);"
  "}"

  "WalletGui--OverviewHeaderFrame #m_startMiningButton:checked:hover {"
    "background-color: %backgroundButtonColorGrayHover%;"
  "}"

  "WalletGui--OverviewHeaderFrame #m_miningStateLabel {"
    "font-size: %fontSizeTiny%;"
    "color: %fontColorRed%;"
  "}"

  "WalletGui--OverviewHeaderFrame #m_miningStateLabel[miningRunning=\"true\"] {"
    "color: %fontColorGreen%;"
  "}";

}

OverviewHeaderFrame::OverviewHeaderFrame(QWidget* _parent) : QFrame(_parent), m_ui(new Ui::OverviewHeaderFrame),
  m_cryptoNoteAdapter(nullptr), m_miningManager(nullptr), m_mainWindow(nullptr), m_nodeStateModel(nullptr),
  m_walletStateModel(nullptr), m_transactionPoolModel(nullptr), m_overViewTransactionPoolModel(nullptr),
  m_miningMapper(new QDataWidgetMapper(this)), m_syncMovie(new QMovie(":icons/light/wallet-sync", QByteArray(), this)),
  m_balancesGlassFrame(new OverviewHeaderGlassFrame(m_syncMovie, nullptr)),
  m_miningStatsGlassFrame(new OverviewHeaderGlassFrame(m_syncMovie, nullptr)),
  m_transactionPoolGlassFrame(new OverviewHeaderGlassFrame(m_syncMovie, nullptr)) {
  m_ui->setupUi(this);
  setStyleSheet(Settings::instance().getCurrentStyle().makeStyleSheet(OVERVIEW_HEADER_STYLE_SHEET_TEMPLATE));
  m_ui->m_overviewAvailableBalanceLabel->installEventFilter(this);
  m_ui->m_overviewLockedBalanceLabel->installEventFilter(this);
  m_ui->m_overviewTotalBalanceLabel->installEventFilter(this);
}

OverviewHeaderFrame::~OverviewHeaderFrame() {
}

bool OverviewHeaderFrame::eventFilter(QObject* _object, QEvent* _event) {
  if (_event ->type() == QEvent::MouseButtonRelease) {
    if (_object == m_ui->m_overviewAvailableBalanceLabel) {
      copyAvailableBalance();
    } else if (_object == m_ui->m_overviewLockedBalanceLabel) {
      copyLockedBalance();
    } else if (_object == m_ui->m_overviewTotalBalanceLabel) {
      copyTotalBalance();
    }
  }

  return QFrame::eventFilter(_object, _event);
}

void OverviewHeaderFrame::setCryptoNoteAdapter(ICryptoNoteAdapter* _cryptoNoteAdapter) {
  m_cryptoNoteAdapter = _cryptoNoteAdapter;
  m_ui->m_overviewAvailableBalanceTickerLabel->setText(m_cryptoNoteAdapter->getCurrencyTicker().toUpper());
  m_ui->m_overviewLockedBalanceTickerLabel->setText(m_cryptoNoteAdapter->getCurrencyTicker().toUpper());
  m_ui->m_overviewTotalBalanceTickerLabel->setText(m_cryptoNoteAdapter->getCurrencyTicker().toUpper());
  m_cryptoNoteAdapter->addObserver(this);
}

void OverviewHeaderFrame::setMiningManager(IMiningManager* _miningManager) {
  m_miningManager = _miningManager;
  m_miningManager->addObserver(this);
}

void OverviewHeaderFrame::setMainWindow(QWidget* _mainWindow) {
  m_mainWindow = _mainWindow;
}

void OverviewHeaderFrame::setNodeStateModel(QAbstractItemModel* _model) {
  m_nodeStateModel = _model;
  QDataWidgetMapper* stateMapper = new QDataWidgetMapper(this);
  stateMapper->setModel(m_nodeStateModel);
  stateMapper->addMapping(m_ui->m_overviewNetworkHashrateLabel, NodeStateModel::COLUMN_NETWORK_HASHRATE, "text");
  stateMapper->addMapping(m_ui->m_overviewNetworkDifficultyLabel, NodeStateModel::COLUMN_LAST_LOCAL_BLOCK_DIFFICULTY, "text");
  stateMapper->setCurrentIndex(0);
}

void OverviewHeaderFrame::setWalletStateModel(QAbstractItemModel* _model) {
  m_walletStateModel = _model;
  QDataWidgetMapper* stateMapper = new QDataWidgetMapper(this);
  stateMapper->setModel(m_walletStateModel);
  stateMapper->addMapping(m_ui->m_overviewAvailableBalanceLabel, WalletStateModel::COLUMN_ACTUAL_BALANCE, "text");
  stateMapper->addMapping(m_ui->m_overviewLockedBalanceLabel, WalletStateModel::COLUMN_PENDING_BALANCE, "text");
  stateMapper->addMapping(m_ui->m_overviewTotalBalanceLabel, WalletStateModel::COLUMN_TOTAL_BALANCE, "text");
  stateMapper->setCurrentIndex(0);
  connect(m_walletStateModel, &QAbstractItemModel::dataChanged, this, &OverviewHeaderFrame::walletStateModelDataChanged);
}

void OverviewHeaderFrame::setTransactionPoolModel(QAbstractItemModel* _model) {
  m_transactionPoolModel = _model;
  connect(m_transactionPoolModel, &QAbstractItemModel::modelReset, this, &OverviewHeaderFrame::transactionPoolChanged);
  m_overViewTransactionPoolModel = new OverviewTransactionPoolModel(m_cryptoNoteAdapter, this);
  m_overViewTransactionPoolModel->setSourceModel(m_transactionPoolModel);
  m_ui->m_overviewTransactionPoolView->setModel(m_overViewTransactionPoolModel);
  int hashColumn = TransactionPoolModel::findProxyColumn(m_overViewTransactionPoolModel, TransactionPoolModel::COLUMN_HASH);
  int amountColumn = TransactionPoolModel::findProxyColumn(m_overViewTransactionPoolModel, TransactionPoolModel::COLUMN_AMOUNT);
  m_ui->m_overviewTransactionPoolView->header()->setSectionResizeMode(hashColumn, QHeaderView::Stretch);
  m_ui->m_overviewTransactionPoolView->header()->setSectionResizeMode(amountColumn, QHeaderView::ResizeToContents);
  m_ui->m_overviewTransactionPoolView->setItemDelegateForColumn(hashColumn, new LinkLikeColumnDelegate(this));
}

void OverviewHeaderFrame::setMinerModel(QAbstractItemModel *_model) {
  m_miningMapper->setModel(_model);
  m_miningMapper->addMapping(m_ui->m_overviewHashRateLabel, MinerModel::COLUMN_HASHRATE, "text");
  if (_model->rowCount() > 0) {
    m_miningMapper->setCurrentIndex(0);
  } else {
    m_ui->m_overviewHashRateLabel->setText("0 H/s");
  }
}

void OverviewHeaderFrame::cryptoNoteAdapterInitCompleted(int _status) {
  // Do nothing
}

void OverviewHeaderFrame::cryptoNoteAdapterDeinitCompleted() {
  // Do nothing
}

void OverviewHeaderFrame::minersLoaded() {
  // Do nothing
}

void OverviewHeaderFrame::minersUnloaded() {
  // Do nothing
}

void OverviewHeaderFrame::miningStarted() {
  if (!m_ui->m_startMiningButton->isChecked()) {
    m_ui->m_startMiningButton->setChecked(true);
    m_ui->m_startMiningButton->setText(tr("Stop mining"));
    m_ui->m_miningStateLabel->setText(tr("mining on"));
    m_ui->m_miningStateLabel->setProperty("miningRunning", true);
    m_ui->m_miningStateLabel->style()->unpolish(m_ui->m_miningStateLabel);
    m_ui->m_miningStateLabel->style()->polish(m_ui->m_miningStateLabel);
  }
}

void OverviewHeaderFrame::miningStopped() {
  if (m_ui->m_startMiningButton->isChecked()) {
    m_ui->m_startMiningButton->setChecked(false);
    m_ui->m_startMiningButton->setText(tr("Start mining"));
    m_ui->m_miningStateLabel->setText(tr("mining off"));
    m_ui->m_miningStateLabel->setProperty("miningRunning", false);
    m_ui->m_miningStateLabel->style()->unpolish(m_ui->m_miningStateLabel);
    m_ui->m_miningStateLabel->style()->polish(m_ui->m_miningStateLabel);
  }
}

void OverviewHeaderFrame::activeMinerChanged(quintptr _minerIndex) {
  m_miningMapper->setCurrentIndex(_minerIndex);
}

void OverviewHeaderFrame::schedulePolicyChanged(int _schedulePolicy) {
  // Do nothing
}

void OverviewHeaderFrame::cpuCoreCountChanged(quint32 _cpuCoreCount) {
  // Do nothing
}

void OverviewHeaderFrame::minerAdded(quintptr _minerIndex) {
  // Do nothing
}

void OverviewHeaderFrame::minerRemoved(quintptr _minerIndex) {
  // Do nothing
}

void OverviewHeaderFrame::stateChanged(quintptr _minerIndex, int _newState) {
  // Do nothing
}

void OverviewHeaderFrame::hashRateChanged(quintptr _minerIndex, quint32 _hashRate) {
  // Do nothing
}

void OverviewHeaderFrame::alternateHashRateChanged(quintptr _minerIndex, quint32 _hashRate) {
  // Do nothing
}

void OverviewHeaderFrame::difficultyChanged(quintptr _minerIndex, quint32 _difficulty) {
  // Do nothing
}

void OverviewHeaderFrame::goodShareCountChanged(quintptr _minerIndex, quint32 _goodShareCount) {
  // Do nothing
}

void OverviewHeaderFrame::goodAlternateShareCountChanged(quintptr _minerIndex, quint32 _goodShareCount) {
  // Do nothing
}

void OverviewHeaderFrame::badShareCountChanged(quintptr _minerIndex, quint32 _badShareCount) {
  // Do nothing
}

void OverviewHeaderFrame::connectionErrorCountChanged(quintptr _minerIndex, quint32 _connectionErrorCount) {
  // Do nothing
}

void OverviewHeaderFrame::lastConnectionErrorTimeChanged(quintptr _minerIndex, const QDateTime& _lastConnectionErrorTime) {
  // Do nothing
}

void OverviewHeaderFrame::copyAvailableBalance() {
  QString balanceString = m_walletStateModel->index(0, WalletStateModel::COLUMN_TOTAL_BALANCE).data().toString();
  balanceString.remove(',');
  QApplication::clipboard()->setText(balanceString);
  m_ui->m_availableBalanceCopyLabel->start();
}

void OverviewHeaderFrame::copyLockedBalance() {
  QString balanceString = m_walletStateModel->index(0, WalletStateModel::COLUMN_PENDING_BALANCE).data().toString();
  balanceString.remove(',');
  QApplication::clipboard()->setText(balanceString);
  m_ui->m_lockedBalanceCopyLabel->start();
}

void OverviewHeaderFrame::copyTotalBalance() {
  QString balanceString = m_walletStateModel->index(0, WalletStateModel::COLUMN_TOTAL_BALANCE).data().toString();
  balanceString.remove(',');
  QApplication::clipboard()->setText(balanceString);
  m_ui->m_totalBalanceCopyLabel->start();
}

void OverviewHeaderFrame::transactionPoolChanged() {
  m_ui->m_overviewPoolSizeLabel->setText(tr("%1 transactions").arg(m_transactionPoolModel->rowCount()));
}

void OverviewHeaderFrame::walletStateModelDataChanged(const QModelIndex& _topLeft, const QModelIndex& _bottomRight,
  const QVector<int>& _roles) {
  if (_topLeft.column() == WalletStateModel::COLUMN_ABOUT_TO_BE_SYNCHRONIZED) {
    bool walletAboutToBeSynchronized = _topLeft.data().toBool();
    if (!walletAboutToBeSynchronized) {
      m_balancesGlassFrame->install(m_ui->m_overviewBalanceFrame);
      m_miningStatsGlassFrame->install(m_ui->m_overviewMiningFrame);
      m_transactionPoolGlassFrame->install(m_ui->m_overviewPoolFrame);
      m_syncMovie->start();
    } else {
      m_balancesGlassFrame->remove();
      m_miningStatsGlassFrame->remove();
      m_transactionPoolGlassFrame->remove();
      m_syncMovie->stop();
    }
  }
}

void OverviewHeaderFrame::poolTransactionClicked(const QModelIndex& _index) {
  QModelIndex sourceIndex = m_overViewTransactionPoolModel->mapToSource(_index);
  if (!sourceIndex.isValid()) {
    return;
  }

  PoolTransactionDetailsDialog dlg(m_transactionPoolModel, sourceIndex, m_mainWindow);
  dlg.exec();
}

void OverviewHeaderFrame::startMiningClicked(bool _on) {
  if (_on) {
    m_ui->m_startMiningButton->setText(tr("Stop mining"));
    m_ui->m_miningStateLabel->setText(tr("mining on"));
    m_ui->m_miningStateLabel->setProperty("miningRunning", true);
    m_miningManager->startMining();
  } else {
    m_ui->m_startMiningButton->setText(tr("Start mining"));
    m_ui->m_miningStateLabel->setText(tr("mining off"));
    m_ui->m_miningStateLabel->setProperty("miningRunning", false);
    m_miningManager->stopMining();
  }

  m_ui->m_miningStateLabel->style()->unpolish(m_ui->m_miningStateLabel);
  m_ui->m_miningStateLabel->style()->polish(m_ui->m_miningStateLabel);
}

}
