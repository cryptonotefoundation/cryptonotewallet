// Copyright (c) 2016 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "OpenUriDialog.h"

#include "ui_openuridialog.h"

namespace WalletGui {

OpenUriDialog::OpenUriDialog(QWidget* _parent) : QDialog(_parent), m_ui(new Ui::OpenUriDialog) {
  m_ui->setupUi(this);
  m_ui->m_uriEdit->setPlaceholderText("karbowanec:");
}

OpenUriDialog::~OpenUriDialog() {
}

QString OpenUriDialog::getURI() const {
  return m_ui->m_uriEdit->text().trimmed();
}

}
