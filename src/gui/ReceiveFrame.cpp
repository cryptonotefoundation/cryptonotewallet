// Copyright (c) 2011-2015 The Cryptonote developers
// Copyright (c) 2016 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QClipboard>
#include <QTimer>
#include <QFontDatabase>
#include "ReceiveFrame.h"
#include "WalletAdapter.h"
#include "CurrencyAdapter.h"

#include "ui_receiveframe.h"

namespace WalletGui {

ReceiveFrame::ReceiveFrame(QWidget* _parent) : QFrame(_parent), m_ui(new Ui::ReceiveFrame) {
  m_ui->setupUi(this);
  connect(&WalletAdapter::instance(), &WalletAdapter::updateWalletAddressSignal, this, &ReceiveFrame::updateWalletAddress);
  connect(&WalletAdapter::instance(), &WalletAdapter::walletActualBalanceUpdatedSignal, this, &ReceiveFrame::updateWalletBalance,
    Qt::QueuedConnection);
  connect(&WalletAdapter::instance(), &WalletAdapter::walletCloseCompletedSignal, this, &ReceiveFrame::reset);


  QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
  font.setPixelSize(14);
  font.setWeight(75);
  m_ui->m_addressEdit->setFont(font);
}

ReceiveFrame::~ReceiveFrame() {
}

void ReceiveFrame::updateWalletAddress(const QString& _address) {
  m_ui->m_addressEdit->setText(_address);
}

void ReceiveFrame::copyAddress() {
  QApplication::clipboard()->setText(m_ui->m_addressEdit->text());
  m_ui->addressStatusLabel->setText(tr("Copied to clipboard"));
  QTimer::singleShot(1500,this,SLOT(clearLabel()));
}

void ReceiveFrame::clearLabel() {
m_ui->addressStatusLabel->setText(tr(""));
}

void ReceiveFrame::updateWalletBalance(quint64 _balance) {
  quint64 actualBalance = WalletAdapter::instance().getActualBalance();
  quint64 pendingBalance = WalletAdapter::instance().getPendingBalance();
  m_ui->totalBalance->setText(CurrencyAdapter::instance().formatAmount(actualBalance + pendingBalance));
}

void ReceiveFrame::reset() {
  m_ui->m_addressEdit->clear();
  updateWalletBalance(0);
}


}
