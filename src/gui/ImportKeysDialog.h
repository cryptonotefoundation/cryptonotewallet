// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Copyright (c) 2016-2019 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <QDialog>

namespace Ui {
class ImportKeysDialog;
}

namespace WalletGui {

class ImportKeysDialog : public QDialog {
  Q_OBJECT

public:
  ImportKeysDialog(QWidget* _parent);
  ~ImportKeysDialog();

  QString getViewKeyString() const;
  QString getSpendKeyString() const;
  QString getFilePath() const;

private:
  QScopedPointer<Ui::ImportKeysDialog> m_ui;

  Q_SLOT void selectPathClicked();
};

}
