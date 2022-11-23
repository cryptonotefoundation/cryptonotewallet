// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Copyright (c) 2016-2022 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <QDialog>
#include <CryptoNote.h>

namespace Ui {
class ImportTrackingKeyDialog;
}

namespace WalletGui {

class ImportTrackingKeyDialog : public QDialog {
  Q_OBJECT

public:
  ImportTrackingKeyDialog(QWidget* _parent);
  ~ImportTrackingKeyDialog();

  QString getKeyString() const;
  QString getFilePath() const;
  quint32 getSyncHeight() const;
  CryptoNote::AccountKeys getAccountKeys() const;

private:
  QScopedPointer<Ui::ImportTrackingKeyDialog> m_ui;

  CryptoNote::AccountKeys m_keys;

  Q_SLOT void selectPathClicked();
  Q_SLOT void onTextChanged();
  Q_SLOT void onAccept();
};

}
