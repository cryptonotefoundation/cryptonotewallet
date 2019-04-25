// Copyright (c) 2017 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "MnemonicSeedDialog.h"
#include "ui_mnemonicseeddialog.h"
#include "CurrencyAdapter.h"
#include "WalletAdapter.h"
#include "Mnemonics/electrum-words.h"
#include "Settings.h"

namespace WalletGui {

MnemonicSeedDialog::MnemonicSeedDialog(QWidget* _parent) : QDialog(_parent), m_ui(new Ui::MnemonicSeedDialog) {
  m_ui->setupUi(this);
  connect(&WalletAdapter::instance(), &WalletAdapter::walletInitCompletedSignal, this, &MnemonicSeedDialog::walletOpened, Qt::QueuedConnection);
  connect(&WalletAdapter::instance(), &WalletAdapter::walletCloseCompletedSignal, this, &MnemonicSeedDialog::walletClosed, Qt::QueuedConnection);
  initLanguages();
  m_ui->m_languageCombo->setCurrentIndex(m_ui->m_languageCombo->findData(getLanguageName(), Qt::DisplayRole));
}

MnemonicSeedDialog::~MnemonicSeedDialog() {
}

void MnemonicSeedDialog::walletOpened() {
  CryptoNote::AccountKeys keys;
  WalletAdapter::instance().getAccountKeys(keys);
  QString mnemonicSeed = WalletAdapter::instance().getMnemonicSeed(getLanguageName());
  m_ui->m_mnemonicSeedEdit->setText(mnemonicSeed);
}

void MnemonicSeedDialog::walletClosed() {
  m_ui->m_mnemonicSeedEdit->clear();
}

void MnemonicSeedDialog::initLanguages() {
  std::vector<std::string> languages;
  Crypto::ElectrumWords::get_language_list(languages);
  for (size_t i = 0; i < languages.size(); ++i)
  {
    m_ui->m_languageCombo->addItem(QString::fromStdString(languages[i]));
  }
}

void MnemonicSeedDialog::languageChanged() {
  QString mnemonicSeed = WalletAdapter::instance().getMnemonicSeed(m_ui->m_languageCombo->currentText());
  m_ui->m_mnemonicSeedEdit->setText(mnemonicSeed);
}

QString MnemonicSeedDialog::getLanguageName() {
  QString lng = Settings::instance().getLanguage();
  QString lang;
  if (lng == "en") {
      lang = "English";
    } else if(lng == "nl") {
      lang = "Nederlands";
    } else if(lng == "fr") {
      lang = "Français";
    } else if(lng == "es") {
      lang = "Español";
    } else if(lng == "pt") {
      lang = "Português";
    } else if(lng == "jp") {
      lang = "日本語";
    } else if(lng == "it") {
      lang = "Italiano";
    } else if(lng == "de") {
      lang = "Deutsch";
    } else if(lng == "ru") {
      lang = "русский язык";
    } else if(lng == "cn") {
      lang = "简体中文 (中国)";
    } else if(lng == "uk") {
      lang = "українська мова";
    } else if(lng == "pl") {
      lang = "język polski";
    } else if(lng == "be") {
      lang = "русский язык";
    } else {
      lang = "English";
    }
  return lang;
}

}
