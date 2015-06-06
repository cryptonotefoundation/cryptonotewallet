// Copyright (c) 2011-2015 The Cryptonote developers
// Copyright (c) 2015 XDN developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QClipboard>

#include <common/base58.h>

#include "ReceiveFrame.h"
#include "CurrencyAdapter.h"
#include "WalletAdapter.h"

#include "ui_receiveframe.h"

namespace WalletGui {

ReceiveFrame::ReceiveFrame(QWidget* _parent) : QFrame(_parent), m_ui(new Ui::ReceiveFrame) {
  m_ui->setupUi(this);
  connect(&WalletAdapter::instance(), &WalletAdapter::updateWalletAddressSignal, this, &ReceiveFrame::updateWalletAddress);
  connect(&WalletAdapter::instance(), &WalletAdapter::walletCloseCompletedSignal, this, &ReceiveFrame::reset);
}

ReceiveFrame::~ReceiveFrame() {
}

void ReceiveFrame::updateWalletAddress(const QString& _address) {
  m_ui->m_addressEdit->setText(_address);
}

void ReceiveFrame::reset() {
  m_ui->m_addressEdit->clear();
  m_ui->m_keyEdit->clear();
}

void ReceiveFrame::copyAddress() {
  QApplication::clipboard()->setText(m_ui->m_addressEdit->text());
}

void ReceiveFrame::copyKey() {
  QApplication::clipboard()->setText(m_ui->m_keyEdit->text());
}

void ReceiveFrame::showKeyClicked() {
  if (!WalletAdapter::instance().isOpen()) {
    return;
  }

  CryptoNote::WalletAccountKeys keys;
  WalletAdapter::instance().getAccountKeys(keys);
  QString privateKeys = QString::fromStdString(tools::base58::encode_addr(CurrencyAdapter::instance().getAddressPrefix(),
    std::string(reinterpret_cast<char*>(&keys), sizeof(keys))));

  m_ui->m_keyEdit->setText(privateKeys);
}

}
