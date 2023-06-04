// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Copyright (c) 2016 The befrank developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <QDialog>

namespace Ui {
class NewNodeDialog;
}

namespace WalletGui {

class NewNodeDialog : public QDialog {
  Q_OBJECT

public:
  NewNodeDialog(QWidget* _parent);
  ~NewNodeDialog();

  QString getHost() const;
  quint16 getPort() const;

private:
  QScopedPointer<Ui::NewNodeDialog> m_ui;
};

}
