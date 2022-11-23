// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Copyright (c) 2016-2022 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>
#include "RestoreFromMnemonicSeedDialog.h"
#include "Mnemonics/electrum-words.h"

extern "C"
{
#include "crypto/keccak.h"
#include "crypto/crypto-ops.h"
}

#include "ui_restorefrommnemonicseeddialog.h"

namespace WalletGui {

RestoreFromMnemonicSeedDialog::RestoreFromMnemonicSeedDialog(QWidget* _parent) : QDialog(_parent), m_ui(new Ui::RestoreFromMnemonicSeedDialog) {
  m_ui->setupUi(this);
  m_ui->m_okButton->setEnabled(false);
}

RestoreFromMnemonicSeedDialog::~RestoreFromMnemonicSeedDialog() {
}

QString RestoreFromMnemonicSeedDialog::getSeedString() const {
  return m_ui->m_seedEdit->toPlainText().trimmed();
}

QString RestoreFromMnemonicSeedDialog::getFilePath() const {
  return m_ui->m_pathEdit->text().trimmed();
}

quint32 RestoreFromMnemonicSeedDialog::getSyncHeight() const {
  return m_ui->m_syncHeight->value();
}

CryptoNote::AccountKeys RestoreFromMnemonicSeedDialog::getAccountKeys() const {
  return m_keys;
}

void RestoreFromMnemonicSeedDialog::selectPathClicked() {
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

void RestoreFromMnemonicSeedDialog::onTextChanged() {
  wordCount = m_ui->m_seedEdit->toPlainText().split(QRegExp("(\\s|\\n|\\r)+")
                                                  , QString::SkipEmptyParts).count();
  if(wordCount != 25) {
    m_ui->m_okButton->setEnabled(false);
    m_ui->m_errorLabel->setText(QString::number(wordCount));
    m_ui->m_errorLabel->setStyleSheet("QLabel { color : red; }");
  }
  if(wordCount == 25) {
    m_ui->m_okButton->setEnabled(true);
    m_ui->m_errorLabel->setText("OK");
    m_ui->m_errorLabel->setStyleSheet("QLabel { color : green; }");
  }
}

void RestoreFromMnemonicSeedDialog::onAccept() {
  std::string seed_language = "";
  QString seedString = getSeedString();
  if (!Crypto::ElectrumWords::words_to_bytes(seedString.toStdString(), m_keys.spendSecretKey, seed_language)) {
    QMessageBox::critical(nullptr, tr("Mnemonic seed is not correct"), tr("There must be an error in mnemonic seed. Make sure you entered it correctly."), QMessageBox::Ok);
    return;
  } else {
    Crypto::secret_key_to_public_key(m_keys.spendSecretKey,m_keys.address.spendPublicKey);
    Crypto::SecretKey second;
    keccak((uint8_t *)&m_keys.spendSecretKey, sizeof(Crypto::SecretKey), (uint8_t *)&second, sizeof(Crypto::SecretKey));
    Crypto::generate_deterministic_keys(m_keys.address.viewPublicKey,m_keys.viewSecretKey,second);
  }

  if (getFilePath().isEmpty()) {
    QMessageBox::critical(nullptr, tr("File path is empty"), tr("Please enter the path where to save the wallet file and its name."), QMessageBox::Ok);
    return;
  }

  accept();
}

}
