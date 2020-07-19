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
  connect(m_ui->m_enableSSL, SIGNAL(stateChanged(const int &)), this, SLOT(enableSSLstateChanged(const int &)));
}

NewNodeDialog::~NewNodeDialog() {
}

void NewNodeDialog::enableSSLstateChanged(const int state) {
  const quint16 offset = 100;
  quint16 port = m_ui->m_portSpin->value();
  if (state == Qt::Unchecked && port > offset) port -= offset;
  else if (state == Qt::Checked && port <= 65535 - offset) port += offset;
  m_ui->m_portSpin->setValue(port);
}

QString NewNodeDialog::getHost() const {
  return m_ui->m_hostEdit->text().trimmed();
}

quint16 NewNodeDialog::getPort() const {
  return m_ui->m_portSpin->value();
}

QString NewNodeDialog::getPath() const {
  return m_ui->m_pathEdit->text();
}

bool NewNodeDialog::getEnableSSL() const {
  int state = m_ui->m_enableSSL->checkState();
  bool res = false;
  if (state == Qt::Checked) res = true;
  return res;
}

}
