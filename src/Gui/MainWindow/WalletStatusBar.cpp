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

#include <QDataWidgetMapper>
#include <QDateTime>
#include <QLabel>
#include <QLocale>
#include <QMovie>
#include <QTimerEvent>

#include "WalletStatusBar.h"
#include "Models/NodeStateModel.h"
#include "Settings/Settings.h"
#include "ICryptoNoteAdapter.h"
#include "INodeAdapter.h"
#include "Style/Style.h"

namespace WalletGui {

namespace {

const QDateTime EPOCH_DATE_TIME = QDateTime::fromMSecsSinceEpoch(0);
const int TEMP_MESSAGE_DURATION = 3000;
const int MSECS_IN_MINUTE = 60 * 1000;
const int MSECS_IN_HOUR = 60 * MSECS_IN_MINUTE;

const char STATUS_BAR_STYLE_SHEET_TEMPLATE[] =
  "WalletGui--WalletStatusBar, "
  "WalletGui--WalletStatusBar QLabel {"
    "background-color: %statusBarBackgroundColor%;"
    "color: %statusBarFontColor%;"
    "font-size: %fontSizeTiny%;"
  "}";

QString formatTimeDiff(quint64 _timeDiff) {

  const int cseconds_in_day = 86400;
  const int cseconds_in_hour = 3600;
  const int cseconds_in_minute = 60;
  const int cseconds = 1;

  long long day = _timeDiff > cseconds_in_day ? _timeDiff / cseconds_in_day : 0;
  long hour = _timeDiff > cseconds_in_hour ? (_timeDiff % cseconds_in_day) / cseconds_in_hour : 0;
  long minute = _timeDiff > cseconds_in_minute ? ((_timeDiff % cseconds_in_day) % cseconds_in_hour) / cseconds_in_minute : 0;
  long seconds = (((_timeDiff % cseconds_in_day) % cseconds_in_hour) % cseconds_in_minute) / cseconds;

  QString firstPart;
  QString secondPart;

   if (day > 0) {
    firstPart = QStringLiteral("%1 %2").arg(day).arg(day == 1 ? QObject::tr("day") : QObject::tr("days"));
    secondPart = QStringLiteral("%1 %2").arg(hour).arg(hour == 1 ? QObject::tr("hour") : QObject::tr("hours"));
  } else if (hour > 0) {
    firstPart = QStringLiteral("%1 %2").arg(hour).arg(hour == 1 ? QObject::tr("hour") : QObject::tr("hours"));
    secondPart = QStringLiteral("%1 %2").arg(minute).arg(minute == 1 ? QObject::tr("minute") : QObject::tr("minutes"));
  } else if (minute > 0) {
    firstPart = QStringLiteral("%1 %2").arg(minute).arg(minute == 1 ? QObject::tr("minute") : QObject::tr("minutes"));
  } else {
    firstPart = QStringLiteral("Less than 1 minute");
  }

  if (secondPart.isNull()) {
    return firstPart;
  }

  return QStringLiteral("%1 %2").arg(firstPart).arg(secondPart);
}

}

WalletStatusBar::WalletStatusBar(QWidget* _parent) : QStatusBar(_parent), m_cryptoNoteAdapter(nullptr), m_nodeStateModel(nullptr), m_syncStatusLabel(new QLabel(this)),
  m_syncStatusIconLabel(new QLabel(this)), m_encryptionStatusIconLabel(new QLabel(this)),
  m_peerCountLabel(new QLabel(this)), m_syncMovie(new QMovie(this)), m_walletIsSynchronized(false), m_checkSyncStateTimerId(-1) {
  m_syncStatusLabel->setObjectName("m_syncStatusLabel");
  m_syncStatusIconLabel->setObjectName("m_syncStatusIconLabel");
  m_encryptionStatusIconLabel->setObjectName("m_encryptionStatusIconLabel");
  m_peerCountLabel->setObjectName("m_peerCountLabel");
  m_syncMovie->setFileName(Settings::instance().getCurrentStyle().getWalletSyncGifFile());
  m_syncMovie->setScaledSize(QSize(16, 16));
  addWidget(m_syncStatusLabel);
  addPermanentWidget(m_peerCountLabel);
  addPermanentWidget(m_encryptionStatusIconLabel);
  addPermanentWidget(m_syncStatusIconLabel);

  setStyleSheet(Settings::instance().getCurrentStyle().makeStyleSheet(STATUS_BAR_STYLE_SHEET_TEMPLATE));
}

WalletStatusBar::~WalletStatusBar() {
}

void WalletStatusBar::setCryptoNoteAdapter(ICryptoNoteAdapter* _cryptoNoteAdapter) {
  m_cryptoNoteAdapter = _cryptoNoteAdapter;
  m_cryptoNoteAdapter->addObserver(this);
  m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter()->addObserver(this);
  quintptr peerCount = m_cryptoNoteAdapter->getNodeAdapter()->getPeerCount();
}

void WalletStatusBar::setNodeStateModel(QAbstractItemModel* _model) {
  m_nodeStateModel = _model;
  QDataWidgetMapper* stateMapper = new QDataWidgetMapper(this);
  stateMapper->setModel(m_nodeStateModel);
  stateMapper->addMapping(m_peerCountLabel, NodeStateModel::COLUMN_PEER_COUNT, "text");
  stateMapper->setCurrentIndex(0);
  connect(m_nodeStateModel, &QAbstractItemModel::dataChanged, this, &WalletStatusBar::nodeStateChanged);
}

void WalletStatusBar::updateStyle() {
  IWalletAdapter* walletAdapter = m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter();
  if (m_syncMovie->state() == QMovie::Running) {
    m_syncMovie->stop();
  }

  m_syncMovie->setFileName(Settings::instance().getCurrentStyle().getWalletSyncGifFile());
  if (walletAdapter->isOpen()) {
    updateEncryptedState(walletAdapter->isEncrypted());
    updateSyncState(m_walletIsSynchronized);
  }

  setStyleSheet(Settings::instance().getCurrentStyle().makeStyleSheet(STATUS_BAR_STYLE_SHEET_TEMPLATE));
}

void WalletStatusBar::walletOpened() {
  showMessage(tr("Wallet opened"), TEMP_MESSAGE_DURATION);
  m_encryptionStatusIconLabel->show();
  m_syncStatusIconLabel->show();
  updateEncryptedState(m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter()->isEncrypted());
  updateSyncState(true);
}

void WalletStatusBar::walletOpenError(int _initStatus) {
  m_walletIsSynchronized = false;
}

void WalletStatusBar::walletClosed() {
  if (m_checkSyncStateTimerId != -1) {
    killTimer(m_checkSyncStateTimerId);
    m_checkSyncStateTimerId = -1;
  }

  m_syncMovie->stop();
  m_encryptionStatusIconLabel->hide();
  m_syncStatusIconLabel->hide();
  showMessage(tr("Wallet closed"));
}

void WalletStatusBar::passwordChanged() {
  showMessage(tr("Password changed"), TEMP_MESSAGE_DURATION);
  updateEncryptedState(m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter()->isEncrypted());
}

void WalletStatusBar::synchronizationProgressUpdated(quint32 _current, quint32 _total) {
  if (m_checkSyncStateTimerId != -1) {
    killTimer(m_checkSyncStateTimerId);
    m_checkSyncStateTimerId = -1;
  }

  quint64 currentDateTime = QDateTime::currentDateTimeUtc().toMSecsSinceEpoch() / 1000;
  quint64 lastBlockTimestamp = m_nodeStateModel->index(0, NodeStateModel::COLUMN_LAST_LOCAL_BLOCK_TIMESTAMP).data(NodeStateModel::ROLE_LAST_LOCAL_BLOCK_TIMESTAMP).value<quint64>();
  quint64 timeDiff = currentDateTime - lastBlockTimestamp;
  QString formattedTimeDiff = lastBlockTimestamp > 0 ? formatTimeDiff(timeDiff) : tr("unknown");
  QString blockchainAge = lastBlockTimestamp > 0 ? QStringLiteral("%1 ago").arg(formattedTimeDiff) : QStringLiteral("%1").arg(formattedTimeDiff);

  m_walletIsSynchronized = false;
  m_syncStatusLabel->setText(tr("Synchronization: %1/%2 (%3)").arg(_current).arg(_total).arg(blockchainAge));
  updateSyncState(false);
}

void WalletStatusBar::synchronizationCompleted() {
  if (m_checkSyncStateTimerId == -1) {
    m_checkSyncStateTimerId = startTimer(MSECS_IN_MINUTE);
  }

  m_walletIsSynchronized = true;
  updateStatusDescription();
  updateSyncState(true);
}

void WalletStatusBar::balanceUpdated(quint64 _actualBalance, quint64 _pendingBalance) {
  // Do nothing
}

void WalletStatusBar::externalTransactionCreated(quintptr _transactionId, const FullTransactionInfo& _transaction) {
  // Do nothing
}

void WalletStatusBar::transactionUpdated(quintptr _transactionId, const FullTransactionInfo& _transaction) {
  // Do nothing
}

void WalletStatusBar::cryptoNoteAdapterInitCompleted(int _status) {
  if (_status == 0) {
    m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter()->addObserver(this);
    if (m_cryptoNoteAdapter->getNodeAdapter()->getWalletAdapter()->isOpen()) {
      walletOpened();
    }
  }
}

void WalletStatusBar::cryptoNoteAdapterDeinitCompleted() {
  // Do nothing
}

void WalletStatusBar::timerEvent(QTimerEvent* _event) {
  if (_event->timerId() == m_checkSyncStateTimerId) {
    updateStatusDescription();
    return;
  }

  QStatusBar::timerEvent(_event);
}

void WalletStatusBar::nodeStateChanged(const QModelIndex& _topLeft, const QModelIndex& _bottomRight, const QVector<int>& _roles) {
  if (_roles.contains(NodeStateModel::ROLE_LOCAL_BLOCK_COUNT)) {
    updateStatusDescription();
  }
}

void WalletStatusBar::updateStatusDescription() {
  if (!m_walletIsSynchronized) {
    return;
  }

  quint32 localBlockCount = m_nodeStateModel->index(0, NodeStateModel::COLUMN_LOCAL_BLOCK_COUNT).data(NodeStateModel::ROLE_LOCAL_BLOCK_COUNT).value<quint32>();
  QDateTime lastLocalBlockTimestamp = QDateTime::fromTime_t(m_nodeStateModel->index(0, NodeStateModel::COLUMN_LAST_LOCAL_BLOCK_TIMESTAMP).data(NodeStateModel::ROLE_LAST_LOCAL_BLOCK_TIMESTAMP).value<quint64>()).toUTC();
  quintptr peerCount = m_nodeStateModel->index(0, NodeStateModel::COLUMN_PEER_COUNT).data(NodeStateModel::ROLE_PEER_COUNT).value<quintptr>();
  QDateTime currentDateTime = QDateTime::currentDateTimeUtc();
  quint64 timeDiff = lastLocalBlockTimestamp.msecsTo(currentDateTime);
  QString warningString;
  if (timeDiff > MSECS_IN_HOUR) {
    warningString.append(QString(" Warning: last block was received %1 hours %2 minutes ago.").
      arg(timeDiff / MSECS_IN_HOUR).arg((timeDiff % MSECS_IN_HOUR) / MSECS_IN_MINUTE));
  }

  if (peerCount == 0) {
    warningString.append(tr(" No network connection."));
  }

  QString statusText = tr("Wallet synchronized. Top block height: %1  /  Time (UTC): %2%3");
  m_syncStatusLabel->setText(statusText.arg(localBlockCount - 1).
    arg(QLocale(QLocale::English).toString(lastLocalBlockTimestamp, "dd MMM yyyy, HH:mm:ss")).
    arg(warningString));
}

void WalletStatusBar::updateSyncState(bool _isSynchronized) {
  if (_isSynchronized) {
    m_syncMovie->stop();
    m_syncStatusIconLabel->setPixmap(Settings::instance().getCurrentStyle().getSyncedIcon());
  } else {
    if (m_syncMovie->state() == QMovie::NotRunning) {
      m_syncStatusIconLabel->setMovie(m_syncMovie);
      m_syncMovie->start();
    }
  }
}

void WalletStatusBar::updateEncryptedState(bool _isEncrypted) {
  if (_isEncrypted) {
    m_encryptionStatusIconLabel->setPixmap(Settings::instance().getCurrentStyle().getEncryptedIcon());
  } else {
    m_encryptionStatusIconLabel->setPixmap(Settings::instance().getCurrentStyle().getNotEncryptedIcon());
  }
}

}
