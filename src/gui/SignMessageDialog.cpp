// Copyright (c) 2016 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "SignMessageDialog.h"
#include "ui_signmessagedialog.h"

#include <QClipboard>
#include <QFileDialog>
#include <QTextStream>
#include <QTabWidget>

#include "CurrencyAdapter.h"
#include "WalletAdapter.h"
#include "MainWindow.h"
#include <boost/utility/value_init.hpp>

namespace WalletGui {

SignMessageDialog::SignMessageDialog(QWidget* _parent) : QDialog(_parent), m_ui(new Ui::SignMessageDialog) {
  m_ui->setupUi(this);
  connect(&WalletAdapter::instance(), &WalletAdapter::walletInitCompletedSignal, this, &SignMessageDialog::walletOpened, Qt::QueuedConnection);
  connect(&WalletAdapter::instance(), &WalletAdapter::walletCloseCompletedSignal, this, &SignMessageDialog::walletClosed, Qt::QueuedConnection);
  m_ui->m_verificationResult->setText("");
}

SignMessageDialog::~SignMessageDialog() {
}

void SignMessageDialog::sign() {
  m_ui->m_tabWidget->setCurrentIndex(0);
  changeTitle(0);
}

void SignMessageDialog::verify() {
  m_ui->m_tabWidget->setCurrentIndex(1);
  changeTitle(1);
}

void SignMessageDialog::changeTitle(int _variant) {
  this->setWindowTitle(_variant == 0 ? tr("Sign message") : tr("Verify signed message"));
}

void SignMessageDialog::walletOpened() {
  m_address = WalletAdapter::instance().getAddress();
}

void SignMessageDialog::walletClosed() {
  m_ui->m_messageEdit->clear();
}

void SignMessageDialog::messageChanged() {
  if (m_ui->m_tabWidget->currentIndex() != 0) { return; }
  QString message = m_ui->m_messageEdit->toPlainText().toUtf8();
  QString sig = WalletAdapter::instance().signMessage(message);
  m_ui->m_signatureEdit->setText(sig);
}

void SignMessageDialog::verifyMessage() {
  m_ui->m_verificationResult->setText("");
  CryptoNote::AccountPublicAddress acc = boost::value_initialized<CryptoNote::AccountPublicAddress>();
  std::string addr_str = m_ui->m_addressEdit->text().trimmed().toStdString();
  QString message = m_ui->m_verifyMessageEdit->toPlainText().toUtf8();
  QString signature = m_ui->m_verifySignatureEdit->toPlainText();
  if(addr_str.empty() || message.isEmpty() || signature.isEmpty())
    return;
  if(CurrencyAdapter::instance().getCurrency().parseAccountAddressString(addr_str, acc)) {
    if (WalletAdapter::instance().verifyMessage(message, acc, signature)) {
      m_ui->m_verificationResult->setText(tr("Signature is valid"));
      m_ui->m_verificationResult->setStyleSheet("QLabel { color : green; }");
    } else {
      m_ui->m_verificationResult->setText(tr("Signature is invalid!"));
      m_ui->m_verificationResult->setStyleSheet("QLabel { color : red; }");
    }
  } else {
    m_ui->m_verificationResult->setText(tr("Address is invalid!"));
    m_ui->m_verificationResult->setStyleSheet("QLabel { color : red; }");
  }
}

}
