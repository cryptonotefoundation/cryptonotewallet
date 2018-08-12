// Copyright (c) 2016 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "PrivateKeysDialog.h"
#include "ui_privatekeysdialog.h"
#include <QClipboard>
#include <Common/Base58.h>
#include <Common/StringTools.h>
#include "CurrencyAdapter.h"
#include "WalletAdapter.h"

namespace WalletGui {

PrivateKeysDialog::PrivateKeysDialog(QWidget* _parent) : QDialog(_parent), m_ui(new Ui::PrivateKeysDialog) {
  m_ui->setupUi(this);
  connect(&WalletAdapter::instance(), &WalletAdapter::walletInitCompletedSignal, this, &PrivateKeysDialog::walletOpened, Qt::QueuedConnection);
  connect(&WalletAdapter::instance(), &WalletAdapter::walletCloseCompletedSignal, this, &PrivateKeysDialog::walletClosed, Qt::QueuedConnection);
}

PrivateKeysDialog::~PrivateKeysDialog() {
}

void PrivateKeysDialog::walletOpened() {
  CryptoNote::AccountKeys keys;
  WalletAdapter::instance().getAccountKeys(keys);

  QString privateKeys = QString::fromStdString(Tools::Base58::encode_addr(CurrencyAdapter::instance().getAddressPrefix(),
    std::string(reinterpret_cast<char*>(&keys), sizeof(keys))));

  m_ui->m_privateKeyEdit->setText(privateKeys);
//m_ui->m_qrLabel->showQRCode(privateKeys);

  QString spendSecretKey = QString::fromStdString(Common::podToHex(keys.spendSecretKey));
  QString viewSecretKey = QString::fromStdString(Common::podToHex(keys.viewSecretKey));

  m_ui->m_spendSecretKeyEdit->setText(spendSecretKey);
  m_ui->m_viewSecretKeyEdit->setText(viewSecretKey);

}

void PrivateKeysDialog::walletClosed() {
  m_ui->m_privateKeyEdit->clear();
//m_ui->m_qrLabel->clear();
}

void PrivateKeysDialog::copyKey() {
  QApplication::clipboard()->setText(m_ui->m_privateKeyEdit->toPlainText());
}

}
