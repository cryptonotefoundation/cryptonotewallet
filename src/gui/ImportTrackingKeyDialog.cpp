// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Copyright (c) 2016-2022 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>

#include "Common/StringTools.h"
#include "CryptoNoteCore/CryptoNoteTools.h"
#include "CurrencyAdapter.h"

#include "ImportTrackingKeyDialog.h"

#include "ui_importtrackingkeydialog.h"

namespace WalletGui {

ImportTrackingKeyDialog::ImportTrackingKeyDialog(QWidget* _parent) : QDialog(_parent), m_ui(new Ui::ImportTrackingKeyDialog) {
  m_ui->setupUi(this);
  m_ui->m_okButton->setEnabled(false);
}

ImportTrackingKeyDialog::~ImportTrackingKeyDialog() {
}

QString ImportTrackingKeyDialog::getKeyString() const {
  return m_ui->m_keyEdit->toPlainText().trimmed();
}

QString ImportTrackingKeyDialog::getFilePath() const {
  return m_ui->m_pathEdit->text().trimmed();
}

quint32 ImportTrackingKeyDialog::getSyncHeight() const {
  return m_ui->m_syncHeight->value();
}

CryptoNote::AccountKeys ImportTrackingKeyDialog::getAccountKeys() const {
  return m_keys;
}

void ImportTrackingKeyDialog::selectPathClicked() {
  QString filePath = QFileDialog::getSaveFileName(this, tr("Tracking wallet file"),
#ifdef Q_OS_WIN
    //QApplication::applicationDirPath(),
      QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
#else
    QDir::homePath(),
#endif
    tr("Tracking wallets (*.wallet)")
    );

  if (!filePath.isEmpty() && !filePath.endsWith(".wallet")) {
    filePath.append(".wallet");
  }

  m_ui->m_pathEdit->setText(filePath);
}

void ImportTrackingKeyDialog::onTextChanged() {
  if (getKeyString().isEmpty()  || getKeyString().size() != 256) {
    m_ui->m_okButton->setEnabled(false);
  } else {
    m_ui->m_okButton->setEnabled(true);
  }
}

void ImportTrackingKeyDialog::onAccept() {
  QString keyString = getKeyString().trimmed();
  if (keyString.isEmpty() || keyString.size() != 256) {
    QMessageBox::warning(this, tr("Tracking key is not valid"), tr("The tracking key you entered is not valid."), QMessageBox::Ok);
    return;
  }

  //  XDN style tracking key import
  //  uint64_t addressPrefix;
  //  std::string data;

  //  if (Tools::Base58::decode_addr(keyString.toStdString(), addressPrefix, data) && addressPrefix == CurrencyAdapter::instance().getAddressPrefix() &&
  //    data.size() == sizeof(keys)) {
  //    std::memcpy(&keys, data.data(), sizeof(keys));

  // To prevent confusing with import of private key / paperwallet lets use Bytecoin style tracking keys, they look different
  std::string public_spend_key_string = keyString.mid(0,64).toStdString();
  std::string public_view_key_string = keyString.mid(64,64).toStdString();
  std::string private_spend_key_string = keyString.mid(128,64).toStdString();
  std::string private_view_key_string = keyString.mid(192,64).toStdString();

  Crypto::Hash public_spend_key_hash;
  Crypto::Hash public_view_key_hash;
  Crypto::Hash private_spend_key_hash;
  Crypto::Hash private_view_key_hash;

  size_t size;
  if (!Common::fromHex(public_spend_key_string, &public_spend_key_hash, sizeof(public_spend_key_hash), size) || size != sizeof(public_spend_key_hash)) {
    QMessageBox::warning(this, tr("Key is not valid"), tr("The public spend key you entered is not valid."), QMessageBox::Ok);
    return;
  }
  if (!Common::fromHex(public_view_key_string, &public_view_key_hash, sizeof(public_view_key_hash), size) || size != sizeof(public_view_key_hash)) {
    QMessageBox::warning(this, tr("Key is not valid"), tr("The public view key you entered is not valid."), QMessageBox::Ok);
    return;
  }
  if (!Common::fromHex(private_spend_key_string, &private_spend_key_hash, sizeof(private_spend_key_hash), size) || size != sizeof(private_spend_key_hash)) {
    QMessageBox::warning(this, tr("Key is not valid"), tr("The private spend key you entered is not valid."), QMessageBox::Ok);
    return;
  }
  if (!Common::fromHex(private_view_key_string, &private_view_key_hash, sizeof(private_view_key_hash), size) || size != sizeof(private_view_key_hash)) {
    QMessageBox::warning(this, tr("Key is not valid"), tr("The private view key you entered is not valid."), QMessageBox::Ok);
    return;
  }

  Crypto::PublicKey public_spend_key = *(struct Crypto::PublicKey *) &public_spend_key_hash;
  Crypto::PublicKey public_view_key = *(struct Crypto::PublicKey *) &public_view_key_hash;
  Crypto::SecretKey private_spend_key = *(struct Crypto::SecretKey *) &private_spend_key_hash;
  Crypto::SecretKey private_view_key = *(struct Crypto::SecretKey *) &private_view_key_hash;

  m_keys.address.spendPublicKey = public_spend_key;
  m_keys.address.viewPublicKey = public_view_key;
  m_keys.spendSecretKey = private_spend_key;
  m_keys.viewSecretKey = private_view_key;

  if (getFilePath().isEmpty()) {
    QMessageBox::critical(nullptr, tr("File path is empty"), tr("Please enter the path where to save the wallet file and its name."), QMessageBox::Ok);
    return;
  }

  accept();
}

}
