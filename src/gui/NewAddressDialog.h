// Copyright (c) 2011-2015 The Cryptonote developers
// Copyright (c) 2016 The befrank developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <QDialog>

namespace Ui {
class NewAddressDialog;
}

namespace WalletGui {

class NewAddressDialog : public QDialog {
  Q_OBJECT
  Q_DISABLE_COPY(NewAddressDialog)

public:
  NewAddressDialog(QWidget* _parent);
  ~NewAddressDialog();

  QString getAddress() const;
  QString getLabel() const;
  QString getPaymentID() const;

  void setEditLabel(QString label);
  void setEditAddress(QString address);
  void setEditPaymentId(QString paymentid);

private:
  QScopedPointer<Ui::NewAddressDialog> m_ui;
};

}
