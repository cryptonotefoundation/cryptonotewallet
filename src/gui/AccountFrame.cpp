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
  m_ui->m_actualBalanceLabel->setText(CurrencyAdapter::instance().formatAmount(_balance).remove(','));
  quint64 pendingBalance = WalletAdapter::instance().getPendingBalance();
  m_ui->m_totalBalanceLabel->setText(CurrencyAdapter::instance().formatAmount(_balance + pendingBalance).remove(','));
}

void AccountFrame::updatePendingBalance(quint64 _balance) {
  m_ui->m_pendingBalanceLabel->setText(CurrencyAdapter::instance().formatAmount(_balance).remove(','));
  quint64 actualBalance = WalletAdapter::instance().getActualBalance();
  m_ui->m_totalBalanceLabel->setText(CurrencyAdapter::instance().formatAmount(_balance + actualBalance).remove(','));
}

void AccountFrame::updateUnmixableBalance(quint64 _balance) {
  m_ui->m_unmixableBalanceLabel->setText(CurrencyAdapter::instance().formatAmount(_balance).remove(','));
}

void AccountFrame::reset() {
  updateActualBalance(0);
  updatePendingBalance(0);
  updateUnmixableBalance(0);
  m_ui->m_addressLabel->clear();
}

}
