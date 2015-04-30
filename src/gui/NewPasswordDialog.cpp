// Copyright (c) 2011-2015 The Cryptonote developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "NewPasswordDialog.h"

#include "ui_newpassworddialog.h"

namespace WalletGui {

NewPasswordDialog::NewPasswordDialog(QWidget* _parent) : QDialog(_parent), m_ui(new Ui::NewPasswordDialog) {
  m_ui->setupUi(this);
  m_ui->m_errorLabel->setText("");
}

NewPasswordDialog::~NewPasswordDialog() {
}

QString NewPasswordDialog::getPassword() const {
  return m_ui->m_passwordEdit->text();
}

void NewPasswordDialog::checkPassword(const QString& _password) {
  bool passwordIsConfirmed = !(m_ui->m_passwordEdit->text().trimmed().isEmpty() ||
    m_ui->m_passwordConfirmationEdit->text().trimmed().isEmpty() ||
    m_ui->m_passwordEdit->text().compare(m_ui->m_passwordConfirmationEdit->text()));
  m_ui->m_errorLabel->setText(passwordIsConfirmed ? "" : tr("Password not confirmed"));
  m_ui->m_okButton->setEnabled(passwordIsConfirmed);
}

}
