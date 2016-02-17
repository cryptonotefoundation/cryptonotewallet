// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QClipboard>

#include <Common/Base58.h>

#include "ReceiveFrame.h"
#include "CurrencyAdapter.h"
#include "WalletAdapter.h"

#include "ui_receiveframe.h"

namespace WalletGui {

ReceiveFrame::ReceiveFrame(QWidget* _parent) : QFrame(_parent), m_ui(new Ui::ReceiveFrame) {
  m_ui->setupUi(this);
  m_ui->m_keyFrame->hide();
  connect(&WalletAdapter::instance(), &WalletAdapter::updateWalletAddressSignal, this, &ReceiveFrame::updateWalletAddress);
  connect(&WalletAdapter::instance(), &WalletAdapter::walletInitCompletedSignal, this, &ReceiveFrame::walletOpened, Qt::QueuedConnection);
  connect(&WalletAdapter::instance(), &WalletAdapter::walletCloseCompletedSignal, this, &ReceiveFrame::walletClosed, Qt::QueuedConnection);
}

ReceiveFrame::~ReceiveFrame() {
}

void ReceiveFrame::updateWalletAddress(const QString& _address) {
  m_ui->m_addressEdit->setText(_address);
}

void ReceiveFrame::walletOpened(int _error) {
  if (_error != 0) {
    return;
  }

  CryptoNote::AccountKeys keys;
  WalletAdapter::instance().getAccountKeys(keys);
  QString privateKeys = QString::fromStdString(Tools::Base58::encode_addr(CurrencyAdapter::instance().getAddressPrefix(),
    std::string(reinterpret_cast<char*>(&keys), sizeof(keys))));

  m_ui->m_keyEdit->setText(privateKeys);
  m_ui->m_qrLabel->showQRCode(privateKeys);
}

void ReceiveFrame::walletClosed() {
  m_ui->m_addressEdit->clear();
  m_ui->m_keyEdit->clear();
  m_ui->m_qrLabel->clear();
}

void ReceiveFrame::copyAddress() {
  QApplication::clipboard()->setText(m_ui->m_addressEdit->text());
}

void ReceiveFrame::copyKey() {
  QApplication::clipboard()->setText(m_ui->m_keyEdit->text());
}

void ReceiveFrame::showKeyClicked() {
  if (!WalletAdapter::instance().isOpen()) {
    m_ui->m_showKeyButton->setChecked(false);
    return;
  }

  m_ui->m_showKeyButton->setText(m_ui->m_showKeyButton->isChecked() ? tr("Hide private keys") : tr("Show private keys"));
  m_ui->m_keyFrame->setVisible(m_ui->m_showKeyButton->isChecked());
}

}
