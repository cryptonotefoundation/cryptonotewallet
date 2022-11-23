// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Copyright (c) 2016-2022 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>

#include "Common/Base58.h"
#include "Common/StringTools.h"
#include "CryptoNoteCore/CryptoNoteTools.h"
#include "CurrencyAdapter.h"

#include "ImportKeyDialog.h"

#include "ui_importkeydialog.h"

namespace WalletGui {

ImportKeyDialog::ImportKeyDialog(QWidget* _parent) : QDialog(_parent), m_ui(new Ui::ImportKeyDialog) {
  m_ui->setupUi(this);
  m_ui->m_okButton->setEnabled(false);
}

ImportKeyDialog::~ImportKeyDialog() {
}

QString ImportKeyDialog::getKeyString() const {
  return m_ui->m_keyEdit->toPlainText().trimmed();
}

QString ImportKeyDialog::getFilePath() const {
  return m_ui->m_pathEdit->text().trimmed();
}

quint32 ImportKeyDialog::getSyncHeight() const {
  return m_ui->m_syncHeight->value();
}

CryptoNote::AccountKeys ImportKeyDialog::getAccountKeys() const {
  return m_keys;
}

void ImportKeyDialog::selectPathClicked() {
  QString filePath = QFileDialog::getSaveFileName(this, tr("Wallet file"),
#ifdef Q_OS_WIN
    //QApplication::applicationDirPath(),
      QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
#else
    QDir::homePath(),
#endif
    tr("Wallets (*.wallet)")
    );

  if (!filePath.isEmpty() && !filePath.endsWith(".wallet")) {
    filePath.append(".wallet");
  }

  m_ui->m_pathEdit->setText(filePath);
}

void ImportKeyDialog::onTextChanged() {
  if (getKeyString().isEmpty() || getKeyString().size() != 183) {
    m_ui->m_okButton->setEnabled(false);
  } else {
    m_ui->m_okButton->setEnabled(true);
  }
}

void ImportKeyDialog::onAccept() {
  uint64_t addressPrefix;
  std::string data;
  QString keyString = getKeyString().trimmed();
  if (!keyString.isEmpty() && Tools::Base58::decode_addr(keyString.toStdString(), addressPrefix, data) && addressPrefix == CurrencyAdapter::instance().getAddressPrefix() && data.size() == sizeof(m_keys)) {
    if (!CryptoNote::fromBinaryArray(m_keys, Common::asBinaryArray(data))) {
      QMessageBox::warning(nullptr, tr("Wallet keys are not valid"), tr("Failed to parse account keys"), QMessageBox::Ok);
      return;
    }
  } else {
    QMessageBox::warning(nullptr, tr("Wallet keys are not valid"), tr("The private keys you entered are not valid."), QMessageBox::Ok);
    return;
  }

  QString filePath = getFilePath();
  if (filePath.isEmpty()) {
    QMessageBox::critical(nullptr, tr("File path is empty"), tr("Please enter the path where to save the wallet file and its name."), QMessageBox::Ok);
    return;
  }

  accept();
}

}
