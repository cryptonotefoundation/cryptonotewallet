// Copyright (c) 2016 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "ExportTrackingKeyDialog.h"
#include "ui_exporttrackingkeydialog.h"
#include <QClipboard>
#include <QFileDialog>
#include <QTextStream>
#include <Common/Base58.h>
#include <Common/StringTools.h>
#include "CurrencyAdapter.h"
#include "WalletAdapter.h"
#include "MainWindow.h"
#include <boost/utility/value_init.hpp>

namespace WalletGui {

ExportTrackingKeyDialog::ExportTrackingKeyDialog(QWidget* _parent) : QDialog(_parent), m_ui(new Ui::ExportTrackingKeyDialog) {
  m_ui->setupUi(this);
  connect(&WalletAdapter::instance(), &WalletAdapter::walletInitCompletedSignal, this, &ExportTrackingKeyDialog::walletOpened, Qt::QueuedConnection);
  connect(&WalletAdapter::instance(), &WalletAdapter::walletCloseCompletedSignal, this, &ExportTrackingKeyDialog::walletClosed, Qt::QueuedConnection);
}

ExportTrackingKeyDialog::~ExportTrackingKeyDialog() {
}

void ExportTrackingKeyDialog::walletOpened() {
  CryptoNote::AccountKeys keys;
  WalletAdapter::instance().getAccountKeys(keys);

  keys.spendSecretKey = boost::value_initialized<Crypto::SecretKey>();

  // XDN paperwallet style key
  // trackingWalletKeys = QString::fromStdString(Tools::Base58::encode_addr(CurrencyAdapter::instance().getAddressPrefix(),
  //                     std::string(reinterpret_cast<char*>(&keys), sizeof(keys))));

  // Bytecoin GUI style .trackingkey. Lets use this style to  prevent confusing with import of private key / paperwallet
  trackingWalletKeys = QString::fromStdString(Common::podToHex(keys));

  m_ui->m_trackingKeyEdit->setText(trackingWalletKeys);
//m_ui->m_qrLabel->showQRCode(trackingWalletKeys);
}

void ExportTrackingKeyDialog::walletClosed() {
  m_ui->m_trackingKeyEdit->clear();
//m_ui->m_qrLabel->clear();
}

void ExportTrackingKeyDialog::copyKey() {
  QApplication::clipboard()->setText(m_ui->m_trackingKeyEdit->toPlainText());
}

void ExportTrackingKeyDialog::saveKeyToFile() {
  QString fileName = QFileDialog::getSaveFileName(&MainWindow::instance(), tr("Save tracking key to..."), QDir::homePath(), tr("Tracking key (*.trackingkey)"));
  if (!fileName.isEmpty()) {
      QFile f(fileName);
      if (f.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
          QTextStream outputStream(&f);
          outputStream << trackingWalletKeys;
          f.close();
      }
  }
}

}
