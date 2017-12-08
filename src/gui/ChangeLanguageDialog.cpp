// Copyright (c) 2016 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "ChangeLanguageDialog.h"
#include "ui_changelanguagedialog.h"
#include "Settings.h"

namespace WalletGui {

ChangeLanguageDialog::ChangeLanguageDialog(QWidget* _parent) : QDialog(_parent), m_ui(new Ui::ChangeLanguageDialog) {
  m_ui->setupUi(this);
}

ChangeLanguageDialog::~ChangeLanguageDialog() {
}

void ChangeLanguageDialog::initLangList() {
 QString currentlang = Settings::instance().getLanguage();

     int index = 0;

         if(currentlang.compare("uk") == 0)
         {
             index = 1;
         }
         else if(currentlang.compare("ru") == 0)
         {
             index = 2;
         }
         else if(currentlang.compare("pl") == 0)
         {
             index = 3;
         }
         else if(currentlang.compare("be") == 0)
         {
             index = 4;
         }
         else if(currentlang.compare("de") == 0)
         {
             index = 5;
         }
         else if(currentlang.compare("es") == 0)
         {
             index = 6;
         }

         m_ui->ChangeLangComboBox->setCurrentIndex(index);

}

QString ChangeLanguageDialog::getLang() const {

    QString lang;
     int n = m_ui->ChangeLangComboBox->currentIndex();
     if (n == 1)
         {
             lang = "uk";
         }
         else if (n == 2)
         {
             lang = "ru";
         }
         else if (n == 3)
         {
             lang = "pl";
         }
         else if (n == 4)
         {
             lang = "be";
         }
         else if (n == 5)
         {
             lang = "de";
         }
         else if (n == 6)
         {
             lang = "es";
         }
         else
         {
             lang = "en";
         }
     return lang;
}

}
