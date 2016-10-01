// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Copyright (c) 2016 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "NewNodeDialog.h"

#include "ui_newnodedialog.h"

namespace WalletGui {

NewNodeDialog::NewNodeDialog(QWidget* _parent) : QDialog(_parent), m_ui(new Ui::NewNodeDialog) {
  m_ui->setupUi(this);
}

NewNodeDialog::~NewNodeDialog() {
}

QString NewNodeDialog::getHost() const {
  return m_ui->m_hostEdit->text().trimmed();
}

quint16 NewNodeDialog::getPort() const {
  return m_ui->m_portSpin->value();
}

}
