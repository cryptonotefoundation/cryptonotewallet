// Copyright (c) 2011-2015 The Cryptonote developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <QFrame>

namespace Ui {
class AddressBookFrame;
}

namespace WalletGui {

class AddressBookFrame : public QFrame {
  Q_OBJECT
  Q_DISABLE_COPY(AddressBookFrame)

public:
  AddressBookFrame(QWidget* _parent);
  ~AddressBookFrame();

private:
  QScopedPointer<Ui::AddressBookFrame> m_ui;

  Q_SLOT void addClicked();
  Q_SLOT void copyClicked();
  Q_SLOT void deleteClicked();
  Q_SLOT void currentAddressChanged(const QModelIndex& _index);
};

}
