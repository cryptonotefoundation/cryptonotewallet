// Copyright (c) 2017 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "VerifyMnemonicSeedDialog.h"
#include "ui_verifymnemonicseeddialog.h"
#include "CurrencyAdapter.h"
#include "WalletAdapter.h"
#include "mnemonics/electrum-words.h"

namespace WalletGui {

VerifyMnemonicSeedDialog::VerifyMnemonicSeedDialog(QWidget* _parent) : QDialog(_parent), m_ui(new Ui::VerifyMnemonicSeedDialog) {
  setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
  m_ui->setupUi(this);
  connect(&WalletAdapter::instance(), &WalletAdapter::walletCloseCompletedSignal, this, &VerifyMnemonicSeedDialog::walletClosed, Qt::QueuedConnection);
  initLanguages();
  m_ui->m_languageCombo->setCurrentIndex(m_ui->m_languageCombo->findData("English", Qt::DisplayRole));
  QString lang = "English";
  QString mnemonicSeed = WalletAdapter::instance().getMnemonicSeed(lang);
  m_ui->m_seedEdit->setText(mnemonicSeed);
}

VerifyMnemonicSeedDialog::~VerifyMnemonicSeedDialog() {
}

void VerifyMnemonicSeedDialog::walletClosed() {
  m_ui->m_seedEdit->clear();
  m_ui->m_seedRepeat->clear();
}

void VerifyMnemonicSeedDialog::onTextChanged() {
  if (QString::compare(m_ui->m_seedEdit->toPlainText().trimmed(), m_ui->m_seedRepeat->toPlainText().trimmed(), Qt::CaseInsensitive) == 0) {
    m_ui->m_okButton->setEnabled(true);
    m_seedsMatch = true;
  } else {
    m_ui->m_okButton->setEnabled(false);
    m_seedsMatch = false;
  }
}

void VerifyMnemonicSeedDialog::reject() {
  if(m_seedsMatch == true) done(0);
}

void VerifyMnemonicSeedDialog::initLanguages() {
  std::vector<std::string> languages;
  Crypto::ElectrumWords::get_language_list(languages);
  for (size_t i = 0; i < languages.size(); ++i)
  {
    m_ui->m_languageCombo->addItem(QString::fromStdString(languages[i]));
  }
}

void VerifyMnemonicSeedDialog::languageChanged() {
  QString mnemonicSeed = WalletAdapter::instance().getMnemonicSeed(m_ui->m_languageCombo->currentText());
  m_ui->m_seedEdit->setText(mnemonicSeed);
  if (QString::compare(m_ui->m_seedEdit->toPlainText().trimmed(), m_ui->m_seedRepeat->toPlainText().trimmed(), Qt::CaseInsensitive) == 0) {
    m_ui->m_okButton->setEnabled(true);
    m_seedsMatch = true;
  } else {
    m_ui->m_okButton->setEnabled(false);
    m_seedsMatch = false;
  }
}

}
