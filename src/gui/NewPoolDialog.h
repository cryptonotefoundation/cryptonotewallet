// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <QDialog>

namespace Ui {
class NewPoolDialog;
}

namespace WalletGui {

class NewPoolDialog : public QDialog {
  Q_OBJECT

public:
  NewPoolDialog(QWidget* _parent);
  ~NewPoolDialog();

  QString getHost() const;
  quint16 getPort() const;

private:
  QScopedPointer<Ui::NewPoolDialog> m_ui;
};

}
