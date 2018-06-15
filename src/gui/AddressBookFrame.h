// Copyright (c) 2011-2015 The Cryptonote developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <QWidget>
#include <QFrame>
#include <QMenu>
#include <QStyledItemDelegate>

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
  QModelIndex index;

public slots:
  void onCustomContextMenu(const QPoint &point);

public Q_SLOTS:
  void addClicked();
  void editClicked();
  void copyClicked();
  void copyPaymentIdClicked();
  void copyLabelClicked();
  void deleteClicked();
  void payToClicked();

private:
  QScopedPointer<Ui::AddressBookFrame> m_ui;

  static bool isValidPaymentId(const QByteArray& _paymentIdString);
  QMenu* contextMenu;

  Q_SLOT void currentAddressChanged(const QModelIndex& _index);
  Q_SLOT void addressDoubleClicked(const QModelIndex& _index);

Q_SIGNALS:
  void payToSignal(const QModelIndex& _index);

};

}
