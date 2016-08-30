// Copyright (c) 2016 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma  once

#include <QDialog>

namespace Ui {
class ChangeLanguageDialog;
}

namespace WalletGui {

class ChangeLanguageDialog : public QDialog {
  Q_OBJECT
  Q_DISABLE_COPY(ChangeLanguageDialog)

public:
  ChangeLanguageDialog(QWidget* _parent);
  ~ChangeLanguageDialog();

  QString getLang() const;
  void initLangList();

private:
  QScopedPointer<Ui::ChangeLanguageDialog> m_ui;
};

}
