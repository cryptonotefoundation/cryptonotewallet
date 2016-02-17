// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <QDialog>

namespace Ui {
class AddressBookDialog;
}

namespace WalletGui {

class AddressBookDialog : public QDialog {
  Q_OBJECT
  Q_DISABLE_COPY(AddressBookDialog)

public:
  AddressBookDialog(QWidget* _parent);
  ~AddressBookDialog();

  QString getAddress() const;

private:
  QScopedPointer<Ui::AddressBookDialog> m_ui;
};

}
