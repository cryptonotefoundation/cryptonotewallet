// Copyright (c) 2015-2017, The Bytecoin developers
//
// This file is part of Bytecoin.
//
// Intensecoin is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Intensecoin is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Intensecoin.  If not, see <http://www.gnu.org/licenses/>.

#include <QFileDialog>
#include <QMessageBox>

#include "KeyDialog.h"
#include "IWalletAdapter.h"
#include "Settings/Settings.h"
#include "Style/Style.h"
#include "ui_KeyDialog.h"

namespace WalletGui {

namespace {

const char KEY_DIALOG_STYLE_SHEET_TEMPLATE[] =
  "* {"
    "font-family: %fontFamily%;"
  "}"

  "WalletGui--KeyDialog #m_keyEdit {"
    "font-size: %fontSizeNormal%;"
    "border-radius: 2px;"
    "border: 1px solid %borderColorDark%;"
  "}";

bool isTrackingKeys(const QByteArray& _array) {
  if (_array.size() < sizeof(AccountKeys)) {
    return false;
  }

  AccountKeys accountKeys;
  QDataStream trackingKeysDataStream(_array);
  trackingKeysDataStream.readRawData(reinterpret_cast<char*>(&accountKeys.spendKeys.publicKey), sizeof(Crypto::PublicKey));
  trackingKeysDataStream.readRawData(reinterpret_cast<char*>(&accountKeys.viewKeys.publicKey), sizeof(Crypto::PublicKey));
  trackingKeysDataStream.readRawData(reinterpret_cast<char*>(&accountKeys.spendKeys.secretKey), sizeof(Crypto::SecretKey));
  trackingKeysDataStream.readRawData(reinterpret_cast<char*>(&accountKeys.viewKeys.secretKey), sizeof(Crypto::SecretKey));
  return (std::memcmp(&accountKeys.spendKeys.secretKey, &CryptoNote::NULL_SECRET_KEY, sizeof(Crypto::SecretKey)) == 0);
}

}

KeyDialog::KeyDialog(const QByteArray& _key, bool _isTracking, QWidget *_parent)
  : QDialog(_parent, static_cast<Qt::WindowFlags>(Qt::WindowCloseButtonHint))
  , m_ui(new Ui::KeyDialog)
  , m_isTracking(_isTracking)
  , m_isExport(true)
  , m_key(_key) {
  m_ui->setupUi(this);
  setWindowTitle(m_isTracking ? tr("Export tracking key") : tr("Export key"));
  m_ui->m_fileButton->setText(tr("Save to file"));
  m_ui->m_okButton->setText(tr("Close"));
  m_ui->m_keyEdit->setReadOnly(true);
  m_ui->m_keyEdit->setPlainText(m_key.toHex().toUpper());
  if (m_isTracking) {
    m_ui->m_descriptionLabel->setText(tr("Tracking key allows other people to see all incoming transactions of this wallet.\n"
      "It doesn't allow spending your funds."));
  }

  m_ui->m_cancelButton->hide();
  setFixedHeight(195);
  setStyleSheet(Settings::instance().getCurrentStyle().makeStyleSheet(KEY_DIALOG_STYLE_SHEET_TEMPLATE));
}

KeyDialog::KeyDialog(QWidget* _parent)
  : QDialog(_parent, static_cast<Qt::WindowFlags>(Qt::WindowCloseButtonHint))
  , m_ui(new Ui::KeyDialog)
  , m_isTracking(false)
  , m_isExport(false) {
  m_ui->setupUi(this);
  setWindowTitle(m_isTracking ? tr("Import tracking key") : tr("Import key"));
  m_ui->m_fileButton->setText(tr("Load from file"));
  if (m_isTracking) {
    m_ui->m_descriptionLabel->setText(tr("Import a tracking key of a wallet to see all its incoming transactions.\n"
      "It doesn't allow spending funds."));
  }

  setFixedHeight(195);
}

KeyDialog::~KeyDialog() {
}

QByteArray KeyDialog::getKey() const {
  return QByteArray::fromHex(m_ui->m_keyEdit->toPlainText().toLatin1());
}

void KeyDialog::saveKey() {
  QString filePath = QFileDialog::getSaveFileName(this, m_isTracking ? tr("Save tracking key to...") : tr("Save key to..."),
#ifdef Q_OS_WIN
    QApplication::applicationDirPath(),
#else
    QDir::homePath(),
#endif
    m_isTracking ? tr("Tracking key (*.trackingkey)") : tr("Wallet key (*.walletkey)"));
  if (filePath.isEmpty()) {
    return;
  }

  if (m_isTracking && !filePath.endsWith(".trackingkey")) {
    filePath.append(".trackingkey");
  } else if (!m_isTracking && !filePath.endsWith(".walletkey")) {
    filePath.append(".walletkey");
  }

  QFile keyFile(filePath);
  if (!keyFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
    return;
  }

  keyFile.write(m_key);
  keyFile.close();
}

void KeyDialog::loadKey() {
  QString filePath = QFileDialog::getOpenFileName(this, m_isTracking ? tr("Load tracking key from...") : tr("Load key from..."),
#ifdef Q_OS_WIN
    QApplication::applicationDirPath(),
#else
    QDir::homePath(),
#endif
    m_isTracking ? tr("Tracking key (*.trackingkey)") : tr("Wallet key (*.walletkey)"));
  if (filePath.isEmpty()) {
    return;
  }

  QFile keyFile(filePath);
  if (!keyFile.open(QIODevice::ReadOnly)) {
    return;
  }

  m_key = keyFile.readAll();
  keyFile.close();
  m_ui->m_keyEdit->setPlainText(m_key.toHex().toUpper());
}

void KeyDialog::fileClicked() {
  if (m_isExport) {
    saveKey();
    accept();
  } else {
    loadKey();
  }
}

void KeyDialog::keyChanged() {
  m_isTracking = isTrackingKeys(getKey());
  setWindowTitle(m_isTracking ? tr("Import tracking key") : tr("Import key"));
  if (m_isTracking) {
    m_ui->m_descriptionLabel->setText(tr("Import a tracking key of a wallet to see all its incoming transactions.\n"
      "It doesn't allow spending funds."));
  } else {
    m_ui->m_descriptionLabel->clear();
  }
}

}
