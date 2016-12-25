// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Copyright (c) 2016 The Karbovanets developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QClipboard>
#include <QFileDialog>
#include <QBuffer>
#include <QUrl>
#include <QTime>

#include <Common/Base58.h>

#include "MainWindow.h"
#include "ReceiveFrame.h"
#include "CurrencyAdapter.h"
#include "WalletAdapter.h"
#include "QRLabel.h"
#include "ShowPaymentRequestDialog.h"

#include "ui_receiveframe.h"

namespace WalletGui {

ReceiveFrame::ReceiveFrame(QWidget* _parent) : QFrame(_parent), m_ui(new Ui::ReceiveFrame) {
  m_ui->setupUi(this);
  m_ui->m_addressQRFrame->hide();
  m_ui->m_requestPaymentFrame->hide();
  m_ui->m_requestAmountSpin->setSuffix(" " + CurrencyAdapter::instance().getCurrencyTicker().toUpper());
  connect(&WalletAdapter::instance(), &WalletAdapter::updateWalletAddressSignal, this, &ReceiveFrame::updateWalletAddress);
  connect(&WalletAdapter::instance(), &WalletAdapter::walletCloseCompletedSignal, this, &ReceiveFrame::walletClosed, Qt::QueuedConnection);
}

ReceiveFrame::~ReceiveFrame() {
}

void ReceiveFrame::updateWalletAddress(const QString& _address) {
  wallet_address = _address;
  m_ui->m_qrLabel->showQRCode(_address);
  m_ui->m_addressQRFrame->show();
}

void ReceiveFrame::walletClosed() {
  m_ui->m_qrLabel->clear();
}

void ReceiveFrame::copyAddress() {
  QApplication::clipboard()->setText(wallet_address);
}

void ReceiveFrame::saveQRcodeToFile() {
  QString fileName = QFileDialog::getSaveFileName(&MainWindow::instance(), tr("Save QR Code"), QDir::homePath(), "PNG (*.png)");
  if (!fileName.isEmpty()) {
    QPixmap qrcode = QPixmap::grabWidget(m_ui->m_qrLabel);
    QFile f(fileName);
    if (f.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
      QByteArray ba;
      QBuffer buffer(&ba);
      buffer.open(QIODevice::WriteOnly);
      qrcode.save(&buffer, "PNG");
      f.write(ba);
      f.close();
    }
  }
}

void ReceiveFrame::requestPaymentClicked() {
  m_ui->m_addressQRFrame->hide();
  m_ui->m_bottomButtonsFrame->hide();
  m_ui->m_requestPaymentFrame->show();
}

void ReceiveFrame::closePaymentRequestForm() {
  m_ui->m_requestPaymentFrame->hide();
  m_ui->m_addressQRFrame->show();
  m_ui->m_bottomButtonsFrame->show();
}

void ReceiveFrame::generatePaymentIdClicked() {
  QTime time = QTime::currentTime();
  qsrand((uint)time.msec());
  const QString possibleCharacters("ABCDEF0123456789");
  const int randomStringLength = 64;
  QString randomString;
  for(int i=0; i<randomStringLength; ++i)
  {
    int index = qrand() % possibleCharacters.length();
    QChar nextChar = possibleCharacters.at(index);
    randomString.append(nextChar);
  }
  m_ui->m_requestPaymentIdEdit->setText(randomString);
}

void ReceiveFrame::createRequestPaymentClicked() {
  requestUri = "karbowanec:" + wallet_address;
  if(CurrencyAdapter::instance().parseAmount(m_ui->m_requestAmountSpin->cleanText()) != 0){
    requestUri.append("?amount=" + m_ui->m_requestAmountSpin->cleanText());
  }

  if(CurrencyAdapter::instance().parseAmount(m_ui->m_requestAmountSpin->cleanText()) != 0 && !m_ui->m_requestPaymentIdEdit->text().isEmpty()) {
    requestUri.append("&payment_id=" + m_ui->m_requestPaymentIdEdit->text());
  } else if(!m_ui->m_requestPaymentIdEdit->text().isEmpty()) {
    requestUri.append("?payment_id=" + m_ui->m_requestPaymentIdEdit->text());
  }

  if((CurrencyAdapter::instance().parseAmount(m_ui->m_requestAmountSpin->cleanText()) != 0 || !m_ui->m_requestPaymentIdEdit->text().isEmpty()) && !m_ui->m_payerLabel->text().isEmpty()) {
    requestUri.append("&label=" + QUrl::toPercentEncoding(m_ui->m_payerLabel->text()));
  } else if(!m_ui->m_payerLabel->text().isEmpty()){
    requestUri.append("?label=" + QUrl::toPercentEncoding(m_ui->m_payerLabel->text()));
  }

  ShowPaymentRequestDialog dlg(&MainWindow::instance());
  dlg.setData(requestUri);
  dlg.exec();
}

}
