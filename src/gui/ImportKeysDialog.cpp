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

#include "ImportKeysDialog.h"

#include "ui_importkeysdialog.h"

namespace WalletGui {

ImportKeysDialog::ImportKeysDialog(QWidget* _parent) : QDialog(_parent), m_ui(new Ui::ImportKeysDialog) {
  m_ui->setupUi(this);
  m_ui->m_okButton->setEnabled(false);
}

ImportKeysDialog::~ImportKeysDialog() {
}

QString ImportKeysDialog::getViewKeyString() const {
  return m_ui->m_viewKeyEdit->text().trimmed();
}

QString ImportKeysDialog::getSpendKeyString() const {
  return m_ui->m_spendKeyEdit->text().trimmed();
}

QString ImportKeysDialog::getFilePath() const {
  return m_ui->m_pathEdit->text().trimmed();
}

quint32 ImportKeysDialog::getSyncHeight() const {
  return m_ui->m_syncHeight->value();
}

CryptoNote::AccountKeys ImportKeysDialog::getAccountKeys() const {
  return m_keys;
}

void ImportKeysDialog::selectPathClicked() {
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

void ImportKeysDialog::onTextChanged(QString _text) {
  Q_UNUSED(_text);
  if (getViewKeyString().isEmpty() || getSpendKeyString().isEmpty() || getViewKeyString().size() != 64 || getSpendKeyString().size() != 64) {
    m_ui->m_okButton->setEnabled(false);
  } else {
    m_ui->m_okButton->setEnabled(true);
  }
}

void ImportKeysDialog::onAccept() {
  QString viewKeyString = getViewKeyString().trimmed();
  QString spendKeyString = getSpendKeyString().trimmed();

  if (viewKeyString.isEmpty() || spendKeyString.isEmpty()) {
    return; // no error message, it's obvious
  }

  uint64_t addressPrefix;
  std::string data;

  std::string private_spend_key_string = spendKeyString.toStdString();
  std::string private_view_key_string = viewKeyString.toStdString();

  Crypto::Hash private_spend_key_hash;
  Crypto::Hash private_view_key_hash;

  size_t size;
  if (!Common::fromHex(private_spend_key_string, &private_spend_key_hash, sizeof(private_spend_key_hash), size) || size != sizeof(private_spend_key_hash)) {
    QMessageBox::warning(this, tr("Key is not valid"), tr("The private spend key you entered is not valid."), QMessageBox::Ok);
    return;
  }
  if (!Common::fromHex(private_view_key_string, &private_view_key_hash, sizeof(private_view_key_hash), size) || size != sizeof(private_view_key_hash)) {
    QMessageBox::warning(this, tr("Key is not valid"), tr("The private view key you entered is not valid."), QMessageBox::Ok);
    return;
  }

  m_keys.spendSecretKey = *(struct Crypto::SecretKey *) &private_spend_key_hash;
  m_keys.viewSecretKey = *(struct Crypto::SecretKey *) &private_view_key_hash;

  Crypto::secret_key_to_public_key(m_keys.spendSecretKey, m_keys.address.spendPublicKey);
  Crypto::secret_key_to_public_key(m_keys.viewSecretKey, m_keys.address.viewPublicKey);

  if (getFilePath().isEmpty()) {
    QMessageBox::critical(nullptr, tr("File path is empty"), tr("Please enter the path where to save the wallet file and its name."), QMessageBox::Ok);
    return;
  }

  accept();
}

}
