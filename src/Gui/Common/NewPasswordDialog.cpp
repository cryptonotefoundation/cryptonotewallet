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

#include "NewPasswordDialog.h"

#include "ui_NewPasswordDialog.h"

namespace WalletGui {

NewPasswordDialog::NewPasswordDialog(QWidget* _parent) : QDialog(_parent, static_cast<Qt::WindowFlags>(Qt::WindowCloseButtonHint)), m_ui(new Ui::NewPasswordDialog) {
  m_ui->setupUi(this);
}

NewPasswordDialog::~NewPasswordDialog() {
}

QString NewPasswordDialog::getPassword() const {
  return m_ui->m_passwordEdit->text();
}

void NewPasswordDialog::setConfirmPasswordError(bool _error) {
  m_ui->m_passwordConfirmationEdit->setProperty("errorState", _error);
  m_ui->m_passwordConfirmationTextLabel->setProperty("errorState", _error);
  m_ui->m_passwordConfirmationTextLabel->setText(_error ? tr("PASSWORDS DO NOT MATCH") : tr("CONFIRM"));

  m_ui->m_passwordConfirmationEdit->style()->unpolish(m_ui->m_passwordConfirmationEdit);
  m_ui->m_passwordConfirmationEdit->style()->polish(m_ui->m_passwordConfirmationEdit);
  m_ui->m_passwordConfirmationEdit->update();

  m_ui->m_passwordConfirmationTextLabel->style()->unpolish(m_ui->m_passwordConfirmationTextLabel);
  m_ui->m_passwordConfirmationTextLabel->style()->polish(m_ui->m_passwordConfirmationTextLabel);
  m_ui->m_passwordConfirmationTextLabel->update();
}

void NewPasswordDialog::setEmptyPasswordError(bool _error) {
  m_ui->m_passwordEdit->setProperty("errorState", _error);
  m_ui->m_passwordConfirmationEdit->setProperty("errorState", _error);
  m_ui->m_passwordTextLabel->setProperty("errorState", _error);
  m_ui->m_passwordTextLabel->setText(_error ? tr("EMPTY PASSWORD") : tr("PASSWORD"));

  m_ui->m_passwordEdit->style()->unpolish(m_ui->m_passwordEdit);
  m_ui->m_passwordEdit->style()->polish(m_ui->m_passwordEdit);
  m_ui->m_passwordEdit->update();

  m_ui->m_passwordConfirmationEdit->style()->unpolish(m_ui->m_passwordConfirmationEdit);
  m_ui->m_passwordConfirmationEdit->style()->polish(m_ui->m_passwordConfirmationEdit);
  m_ui->m_passwordConfirmationEdit->update();

  m_ui->m_passwordTextLabel->style()->unpolish(m_ui->m_passwordTextLabel);
  m_ui->m_passwordTextLabel->style()->polish(m_ui->m_passwordTextLabel);
  m_ui->m_passwordTextLabel->update();
}

void NewPasswordDialog::checkPassword(const QString& _password) {
  bool passwordIsEmpty = m_ui->m_passwordEdit->text().trimmed().isEmpty() && m_ui->m_passwordConfirmationEdit->text().trimmed().isEmpty();
  setEmptyPasswordError(passwordIsEmpty);
  bool passwordIsConfirmed = !(m_ui->m_passwordEdit->text().compare(m_ui->m_passwordConfirmationEdit->text()));
  setConfirmPasswordError(!passwordIsConfirmed);
  m_ui->m_okButton->setEnabled(!passwordIsEmpty && passwordIsConfirmed);
}

}
