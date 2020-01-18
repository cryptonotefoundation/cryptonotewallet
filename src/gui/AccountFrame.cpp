// Copyright (c) 2011-2015 The Cryptonote developers
// Copyright (c) 2016-2020 The Karbo developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QClipboard>
#include <QTimer>
#include <QFontDatabase>
#include "AccountFrame.h"
#include "WalletAdapter.h"
#include "CurrencyAdapter.h"

#include "ui_accountframe.h"

namespace WalletGui {

QStringList AccountFrame::divideAmount(quint64 _val) {
  QStringList list;
  QString str = CurrencyAdapter::instance().formatAmount(_val).remove(',');

  quint32 offset = str.indexOf(".") + 3; // add two digits .00
  QString before = str.left(offset);
  QString after  = str.mid(offset);

  list << before << after;

  return list;
}

AccountFrame::AccountFrame(QWidget* _parent) : QFrame(_parent), m_ui(new Ui::AccountFrame) {
  m_ui->setupUi(this);
  connect(&WalletAdapter::instance(), &WalletAdapter::updateWalletAddressSignal, this, &AccountFrame::updateWalletAddress);
  connect(&WalletAdapter::instance(), &WalletAdapter::walletActualBalanceUpdatedSignal, this, &AccountFrame::updateActualBalance,
    Qt::QueuedConnection);
  connect(&WalletAdapter::instance(), &WalletAdapter::walletPendingBalanceUpdatedSignal, this, &AccountFrame::updatePendingBalance,
    Qt::QueuedConnection);
  connect(&WalletAdapter::instance(), &WalletAdapter::walletUnmixableBalanceUpdatedSignal, this, &AccountFrame::updateUnmixableBalance,
    Qt::QueuedConnection);
  connect(&WalletAdapter::instance(), &WalletAdapter::walletCloseCompletedSignal, this, &AccountFrame::reset);

  m_ui->m_tickerLabel1->setText(CurrencyAdapter::instance().getCurrencyTicker().toUpper());
  m_ui->m_tickerLabel2->setText(CurrencyAdapter::instance().getCurrencyTicker().toUpper());
  m_ui->m_tickerLabel3->setText(CurrencyAdapter::instance().getCurrencyTicker().toUpper());
  m_ui->m_tickerLabel4->setText(CurrencyAdapter::instance().getCurrencyTicker().toUpper());

  int id = QFontDatabase::addApplicationFont(":/fonts/mplusm");
  QString family = QFontDatabase::applicationFontFamilies(id).at(0);
  QFont monospace(family);
  monospace.setPixelSize(16);
  m_ui->m_addressLabel->setFont(monospace);
}

AccountFrame::~AccountFrame() {
}

void AccountFrame::updateWalletAddress(const QString& _address) {
  m_ui->m_addressLabel->setText(_address);
}

void AccountFrame::copyAddress() {
  QApplication::clipboard()->setText(m_ui->m_addressLabel->text());
}

void AccountFrame::showQR() {
  Q_EMIT showQRcodeSignal();
}

void AccountFrame::updateActualBalance(quint64 _balance) {
  QStringList actualList = divideAmount(_balance);
  m_ui->m_actualBalanceLabel->setText(actualList.first());
  m_ui->m_actualBalanceDust->setText(actualList.last());

  quint64 pendingBalance = WalletAdapter::instance().getPendingBalance();

  QStringList pendingList = divideAmount(_balance + pendingBalance);
  m_ui->m_totalBalanceLabel->setText(pendingList.first());
  m_ui->m_totalBalanceDust->setText(pendingList.last());
}

void AccountFrame::updatePendingBalance(quint64 _balance) {
  QStringList pendingList = divideAmount(_balance);
  m_ui->m_pendingBalanceLabel->setText(pendingList.first());
  m_ui->m_pendingBalanceDust->setText(pendingList.last());

  quint64 actualBalance = WalletAdapter::instance().getActualBalance();

  QStringList totalList = divideAmount(_balance + actualBalance);
  m_ui->m_totalBalanceLabel->setText(totalList.first());
  m_ui->m_totalBalanceDust->setText(totalList.last());
}

void AccountFrame::updateUnmixableBalance(quint64 _balance) {
  QStringList unmixableList = divideAmount(_balance);

  m_ui->m_unmixableBalanceLabel->setText(unmixableList.first());
  m_ui->m_unmixableBalanceDust->setText(unmixableList.last());
}

void AccountFrame::reset() {
  updateActualBalance(0);
  updatePendingBalance(0);
  updateUnmixableBalance(0);
  m_ui->m_addressLabel->clear();
}

}
