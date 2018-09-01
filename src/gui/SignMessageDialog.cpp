// Copyright (c) 2016 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "SignMessageDialog.h"
#include "ui_signmessagedialog.h"

#include <QClipboard>
#include <QFileDialog>
#include <QTextStream>

#include "Common/Base58.h"
#include "Common/StringTools.h"
#include "CurrencyAdapter.h"
#include "WalletAdapter.h"
#include "MainWindow.h"
#include <boost/utility/value_init.hpp>
#include "crypto/crypto.h"

namespace WalletGui {

SignMessageDialog::SignMessageDialog(QWidget* _parent) : QDialog(_parent), m_ui(new Ui::SignMessageDialog) {
  m_ui->setupUi(this);
  connect(&WalletAdapter::instance(), &WalletAdapter::walletInitCompletedSignal, this, &SignMessageDialog::walletOpened, Qt::QueuedConnection);
  connect(&WalletAdapter::instance(), &WalletAdapter::walletCloseCompletedSignal, this, &SignMessageDialog::walletClosed, Qt::QueuedConnection);
}

SignMessageDialog::~SignMessageDialog() {
}

void SignMessageDialog::walletOpened() {
  m_address = WalletAdapter::instance().getAddress();
}

void SignMessageDialog::walletClosed() {
  m_ui->m_messageEdit->clear();
}

void SignMessageDialog::messageChanged() {
  std::string message = m_ui->m_messageEdit->toPlainText().toUtf8().constData();
  Crypto::Hash hash;
  Crypto::cn_fast_hash(message.data(), message.size(), hash);
  CryptoNote::AccountKeys keys;
  WalletAdapter::instance().getAccountKeys(keys);
  Crypto::Signature signature;
  Crypto::generate_signature(hash, keys.address.spendPublicKey, keys.spendSecretKey, signature);
  QString sig = QString::fromStdString(std::string("SigV1") + Tools::Base58::encode(std::string((const char *)&signature, sizeof(signature))));
  m_ui->m_signatureEdit->setText(sig);
}

void SignMessageDialog::copySignature() {
  QApplication::clipboard()->setText(m_ui->m_signatureEdit->toPlainText());
}

void SignMessageDialog::saveSignatureToFile() {
  QString fileName = QFileDialog::getSaveFileName(&MainWindow::instance(), tr("Save signature to..."), QDir::homePath(), tr("Plain text (*.txt)"));
  if (!fileName.isEmpty()) {
    QFile f(fileName);
    if (f.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
      QTextStream outputStream(&f);
      outputStream << m_ui->m_signatureEdit->toPlainText();
      f.close();
    }
  }
}

}
