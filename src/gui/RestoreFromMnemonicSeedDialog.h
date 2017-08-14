// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Copyright (c) 2016-2017 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <QDialog>

namespace Ui {
class RestoreFromMnemonicSeedDialog;
}

namespace WalletGui {

class RestoreFromMnemonicSeedDialog : public QDialog {
  Q_OBJECT

public:
  RestoreFromMnemonicSeedDialog(QWidget* _parent);
  ~RestoreFromMnemonicSeedDialog();

  QString getSeedString() const;
  QString getFilePath() const;

private:
  QScopedPointer<Ui::RestoreFromMnemonicSeedDialog> m_ui;

  int wordCount = 0;

  Q_SLOT void selectPathClicked();
  Q_SLOT void onTextChanged();
};

}
