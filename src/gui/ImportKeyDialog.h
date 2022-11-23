// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Copyright (c) 2016-2022 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <QDialog>
#include <CryptoNote.h>

namespace Ui {
class ImportKeyDialog;
}

namespace WalletGui {

class ImportKeyDialog : public QDialog {
  Q_OBJECT

public:
  ImportKeyDialog(QWidget* _parent);
  ~ImportKeyDialog();

  QString getKeyString() const;
  QString getFilePath() const;
  quint32 getSyncHeight() const;
  CryptoNote::AccountKeys getAccountKeys() const;

private:
  QScopedPointer<Ui::ImportKeyDialog> m_ui;

  CryptoNote::AccountKeys m_keys;

  Q_SLOT void selectPathClicked();
  Q_SLOT void onTextChanged();
  Q_SLOT void onAccept();
};

}
