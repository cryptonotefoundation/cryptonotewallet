// Copyright (c) 2017 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "MnemonicSeedDialog.h"
#include "ui_mnemonicseeddialog.h"
#include "CurrencyAdapter.h"
#include "WalletAdapter.h"

namespace WalletGui {

MnemonicSeedDialog::MnemonicSeedDialog(QWidget* _parent) : QDialog(_parent), m_ui(new Ui::MnemonicSeedDialog) {
  m_ui->setupUi(this);
  connect(&WalletAdapter::instance(), &WalletAdapter::walletInitCompletedSignal, this, &MnemonicSeedDialog::walletOpened, Qt::QueuedConnection);
  connect(&WalletAdapter::instance(), &WalletAdapter::walletCloseCompletedSignal, this, &MnemonicSeedDialog::walletClosed, Qt::QueuedConnection);
}

MnemonicSeedDialog::~MnemonicSeedDialog() {
}

void MnemonicSeedDialog::walletOpened() {
  CryptoNote::AccountKeys keys;
  WalletAdapter::instance().getAccountKeys(keys);
  QString mnemonicSeed = WalletAdapter::instance().getMnemonicSeed();
  m_ui->m_mnemonicSeedEdit->setText(mnemonicSeed);
}

void MnemonicSeedDialog::walletClosed() {
  m_ui->m_mnemonicSeedEdit->clear();
}

}
