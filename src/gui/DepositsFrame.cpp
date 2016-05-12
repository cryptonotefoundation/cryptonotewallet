// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "DepositsFrame.h"
#include "DepositDetailsDialog.h"
#include "DepositListModel.h"
#include "DepositModel.h"
#include "CurrencyAdapter.h"
#include "MainWindow.h"
#include "WalletAdapter.h"
#include "WalletEvents.h"

#include "ui_depositsframe.h"

namespace WalletGui {

namespace {

Q_DECL_CONSTEXPR quint32 SECONDS_IN_MINUTE = 60;
Q_DECL_CONSTEXPR quint32 SECONDS_IN_HOUR = 60 * SECONDS_IN_MINUTE;
Q_DECL_CONSTEXPR quint32 SECONDS_IN_DAY = 24 * SECONDS_IN_HOUR;
Q_DECL_CONSTEXPR quint32 SECONDS_IN_MONTH = 30 * SECONDS_IN_DAY;
Q_DECL_CONSTEXPR quint32 SECONDS_IN_YEAR = 12 * SECONDS_IN_MONTH;

QString secondsToNativeTime(int _seconds) {
  QString resTempate("~ %1 %2");
  if (_seconds < SECONDS_IN_MINUTE) {
    return resTempate.arg(_seconds).arg(QObject::tr("seconds"));
  } else if (_seconds < SECONDS_IN_HOUR) {
    return resTempate.arg(_seconds / SECONDS_IN_MINUTE).arg(QObject::tr("minutes"));
  } else if (_seconds < SECONDS_IN_DAY) {
    return resTempate.arg(_seconds / SECONDS_IN_HOUR).arg(QObject::tr("hours"));
  } else if (_seconds < SECONDS_IN_MONTH) {
    return resTempate.arg(_seconds / SECONDS_IN_DAY).arg(QObject::tr("days"));
  } else if (_seconds < SECONDS_IN_YEAR) {
    return resTempate.arg(_seconds / SECONDS_IN_MONTH).arg(QObject::tr("months"));
  } else {
    return resTempate.arg(_seconds / SECONDS_IN_YEAR).arg(QObject::tr("years"));
  }

  return QString();
}

}

DepositsFrame::DepositsFrame(QWidget* _parent) : QFrame(_parent), m_ui(new Ui::DepositsFrame), m_depositModel(new DepositListModel) {
  m_ui->setupUi(this);
  m_ui->m_timeSpin->setMinimum(CurrencyAdapter::instance().getDepositMinTerm());
  m_ui->m_timeSpin->setMaximum(CurrencyAdapter::instance().getDepositMaxTerm());
  m_ui->m_timeSpin->setSuffix(QString(" %1").arg(tr("Blocks")));
  m_ui->m_amountSpin->setSuffix(" " + CurrencyAdapter::instance().getCurrencyTicker().toUpper());
  m_ui->m_amountSpin->setMinimum(CurrencyAdapter::instance().formatAmount(CurrencyAdapter::instance().getDepositMinAmount()).toDouble());
  m_ui->m_amountSpin->setDecimals(CurrencyAdapter::instance().getNumberOfDecimalPlaces());
  m_ui->m_depositView->setModel(m_depositModel.data());

  m_ui->m_tickerLabel1->setText(CurrencyAdapter::instance().getCurrencyTicker().toUpper());
  m_ui->m_tickerLabel2->setText(CurrencyAdapter::instance().getCurrencyTicker().toUpper());
  m_ui->m_tickerLabel3->setText(CurrencyAdapter::instance().getCurrencyTicker().toUpper());
  m_ui->m_feeLabel->setText(tr("%1 %2").arg(CurrencyAdapter::instance().formatAmount(CurrencyAdapter::instance().getMinimumFee())).arg(CurrencyAdapter::instance().getCurrencyTicker().toUpper()));

  connect(&WalletAdapter::instance(), &WalletAdapter::walletActualDepositBalanceUpdatedSignal,
    this, &DepositsFrame::actualDepositBalanceUpdated, Qt::QueuedConnection);
  connect(&WalletAdapter::instance(), &WalletAdapter::walletPendingDepositBalanceUpdatedSignal,
    this, &DepositsFrame::pendingDepositBalanceUpdated, Qt::QueuedConnection);
  connect(&WalletAdapter::instance(), &WalletAdapter::walletCloseCompletedSignal, this, &DepositsFrame::reset,
    Qt::QueuedConnection);

  reset();
}

DepositsFrame::~DepositsFrame() {
}

void DepositsFrame::actualDepositBalanceUpdated(quint64 _balance) {
  m_ui->m_unlockedDepositLabel->setText(CurrencyAdapter::instance().formatAmount(_balance));
  quint64 pendingDepositBalance = WalletAdapter::instance().getPendingDepositBalance();
  m_ui->m_totalDepositLabel->setText(CurrencyAdapter::instance().formatAmount(_balance + pendingDepositBalance));
}

void DepositsFrame::pendingDepositBalanceUpdated(quint64 _balance) {
  m_ui->m_lockedDepositLabel->setText(CurrencyAdapter::instance().formatAmount(_balance));
  quint64 actualDepositBalance = WalletAdapter::instance().getActualDepositBalance();
  m_ui->m_totalDepositLabel->setText(CurrencyAdapter::instance().formatAmount(_balance + actualDepositBalance));
}

void DepositsFrame::reset() {
  actualDepositBalanceUpdated(0);
  pendingDepositBalanceUpdated(0);
}

void DepositsFrame::depositClicked() {
  quint64 amount = CurrencyAdapter::instance().parseAmount(m_ui->m_amountSpin->cleanText());
  if (amount == 0 || amount + CurrencyAdapter::instance().getMinimumFee() > WalletAdapter::instance().getActualBalance()) {
    QCoreApplication::postEvent(&MainWindow::instance(), new ShowMessageEvent(tr("Wrong amount"), QtCriticalMsg));
    return;
  }

  quint32 term = m_ui->m_timeSpin->value();
  WalletAdapter::instance().deposit(term, amount, CurrencyAdapter::instance().getMinimumFee(), 0);
}

void DepositsFrame::depositParamsChanged() {
  quint64 amount = CurrencyAdapter::instance().parseAmount(m_ui->m_amountSpin->cleanText());
  quint32 term = m_ui->m_timeSpin->value();
  quint64 interest = CurrencyAdapter::instance().calculateInterest(amount, term);
  qreal rate = DepositModel::calculateRate(amount, interest, term);
  m_ui->m_interestLabel->setText(QString("+ %1 %2 (%3 %)").arg(CurrencyAdapter::instance().formatAmount(interest)).
    arg(CurrencyAdapter::instance().getCurrencyTicker().toUpper()).arg(QString::number(rate * 100, 'f', 2)));
}

void DepositsFrame::showDepositDetails(const QModelIndex& _index) {
  if (!_index.isValid()) {
    return;
  }

  DepositDetailsDialog dlg(_index, &MainWindow::instance());
  dlg.exec();
}

void DepositsFrame::timeChanged(int _value) {
  m_ui->m_nativeTimeLabel->setText(secondsToNativeTime(_value * CurrencyAdapter::instance().getDifficultyTarget()));
}

void DepositsFrame::withdrawClicked() {
  QModelIndexList unlockedDepositIndexList = DepositModel::instance().match(DepositModel::instance().index(0, 0),
    DepositModel::ROLE_STATE, DepositModel::STATE_UNLOCKED, -1);
  if (unlockedDepositIndexList.isEmpty()) {
    return;
  }

  QVector<CryptoNote::DepositId> depositIds;
  Q_FOREACH (const QModelIndex& index, unlockedDepositIndexList) {
    depositIds.append(index.row());
  }

  WalletAdapter::instance().withdrawUnlockedDeposits(depositIds, CurrencyAdapter::instance().getMinimumFee());
}

}
