// Copyright (c) 2015-2017, The Bytecoin developers
//
// This file is part of Bytecoin.
//
// Intensecoin is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Intensecoin is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Intensecoin.  If not, see <http://www.gnu.org/licenses/>.

#include "P2pBindPortErrorDialog.h"

#include "ui_P2pBindPortErrorDialog.h"

namespace WalletGui {

P2pBindPortErrorDialog::P2pBindPortErrorDialog(quint16 _oldPort, quint16 _newPort, QWidget* _parent) : QDialog(_parent, static_cast<Qt::WindowFlags>(Qt::WindowCloseButtonHint)),
  m_ui(new Ui::P2pBindPortErrorDialog) {
  m_ui->setupUi(this);

  QString errorText = m_ui->m_portErrorLabel->text();
  m_ui->m_portErrorLabel->setText(errorText.arg(_oldPort));
  m_ui->m_newPortSpin->setValue(_newPort);
}

P2pBindPortErrorDialog::~P2pBindPortErrorDialog() {
}

quint16 P2pBindPortErrorDialog::getNewPort() const {
  return m_ui->m_newPortSpin->value();
}

void P2pBindPortErrorDialog::accept() {
  if (m_ui->m_acceptRadio->isChecked()) {
    QDialog::accept();
  } else {
    QDialog::reject();
  }
}

}
