// Copyright (c) 2011-2015 The Cryptonote developers
// Copyright (c) 2016-2017 The Karbowanec developers
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
  connect(&WalletAdapter::instance(), &WalletAdapter::walletActualBalanceUpdatedSignal, this, &AccountFrame::updateWalletBalance,
    Qt::QueuedConnection);
  connect(&WalletAdapter::instance(), &WalletAdapter::walletCloseCompletedSignal, this, &AccountFrame::reset);

  int id = QFontDatabase::addApplicationFont(":/fonts/mplusm");
  QString family = QFontDatabase::applicationFontFamilies(id).at(0);
  QFont monospace(family);
  monospace.setPixelSize(15);
  m_ui->m_addressLabel->setFont(monospace);
}

AccountFrame::~AccountFrame() {
}

void AccountFrame::updateWalletAddress(const QString& _address) {
  m_ui->m_addressLabel->setText(_address);
}

void AccountFrame::copyAddress() {
  QApplication::clipboard()->setText(m_ui->m_addressLabel->text());
  m_ui->addressStatusLabel->setText(tr("Copied to clipboard"));
  QTimer::singleShot(1500,this,SLOT(clearLabel()));
}

void AccountFrame::showQR() {
  Q_EMIT showQRcodeSignal();
}

void AccountFrame::clearLabel() {
m_ui->addressStatusLabel->setText(tr(""));
}

void AccountFrame::updateWalletBalance(quint64 _balance) {
  quint64 actualBalance = WalletAdapter::instance().getActualBalance();
  quint64 pendingBalance = WalletAdapter::instance().getPendingBalance();
  m_ui->totalBalance->setText(CurrencyAdapter::instance().formatAmount(actualBalance + pendingBalance).remove(','));
}

void AccountFrame::reset() {
  m_ui->m_addressLabel->clear();
  updateWalletBalance(0);
}

}
