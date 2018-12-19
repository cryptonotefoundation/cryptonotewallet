// Copyright (c) 2016 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "GetBalanceProofDialog.h"
#include "ui_getbalanceproofdialog.h"

#include <QClipboard>
#include <QFileDialog>
#include <QBuffer>
#include <QTextStream>

#include "Common/Base58.h"
#include "Common/StringTools.h"
#include "CurrencyAdapter.h"
#include "WalletAdapter.h"
#include "MainWindow.h"

namespace WalletGui {

GetBalanceProofDialog::GetBalanceProofDialog(QWidget* _parent) : QDialog(_parent), m_ui(new Ui::GetBalanceProofDialog) {
  m_ui->setupUi(this);
  connect(&WalletAdapter::instance(), &WalletAdapter::walletActualBalanceUpdatedSignal, this, &GetBalanceProofDialog::walletBalanceUpdated, Qt::QueuedConnection);
  m_ui->m_amountSpin->setSuffix(" " + CurrencyAdapter::instance().getCurrencyTicker().toUpper());
  m_amount = WalletAdapter::instance().getActualBalance();
  m_ui->m_amountSpin->setValue(CurrencyAdapter::instance().formatAmount(m_amount).toDouble());
  genProof();
}

GetBalanceProofDialog::~GetBalanceProofDialog() {
}

void GetBalanceProofDialog::walletBalanceUpdated() {
  if (this->isVisible()) {
    m_amount = WalletAdapter::instance().getActualBalance();
    m_ui->m_amountSpin->setValue(CurrencyAdapter::instance().formatAmount(m_amount).toDouble());
  }
}

void GetBalanceProofDialog::genProof() {
  m_amount = static_cast<quint64>(CurrencyAdapter::instance().parseAmount(m_ui->m_amountSpin->cleanText()));
  quint64 balance = WalletAdapter::instance().getActualBalance();
  if (m_amount > balance) {
      m_amount = balance;
      m_ui->m_amountSpin->setValue(CurrencyAdapter::instance().formatAmount(m_amount).toDouble());
  }
  m_message = m_ui->m_messageEdit->toPlainText().toUtf8().constData();
  m_proof = WalletAdapter::instance().getReserveProof(m_amount, m_message);
  m_ui->m_signatureEdit->setText(m_proof);
}

void GetBalanceProofDialog::copyProof() {
  QApplication::clipboard()->setText(m_proof);
}

void GetBalanceProofDialog::saveProof() {
 QString file = QFileDialog::getSaveFileName(&MainWindow::instance(), tr("Save as"), QDir::homePath(), "TXT (*.txt)");
   if (!file.isEmpty()) {
     QFile f(file);
      if (f.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QTextStream outputStream(&f);
        outputStream << m_proof;
        f.close();
      }
   }
}

}
