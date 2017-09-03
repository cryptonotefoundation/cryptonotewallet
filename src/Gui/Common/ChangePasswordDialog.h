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

#pragma once

#include <QDialog>

namespace Ui {
class ChangePasswordDialog;
}

namespace WalletGui {

class ChangePasswordDialog : public QDialog {
  Q_OBJECT
  Q_DISABLE_COPY(ChangePasswordDialog)

public:
  ChangePasswordDialog(bool _old_password_error, QWidget* _parent);
  ~ChangePasswordDialog();

  QString getNewPassword() const;
  QString getOldPassword() const;

private:
  QScopedPointer<Ui::ChangePasswordDialog> m_ui;

  void setConfirmPasswordError(bool _error);
  void setEmptyPasswordError(bool _error);
  void setOldPasswordError(bool _error);

  Q_SLOT void checkPassword(const QString& _password);
  Q_SLOT void passwordChanged(const QString& _password);
};

}
