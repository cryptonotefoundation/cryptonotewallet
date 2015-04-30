// Copyright (c) 2011-2015 The Cryptonote developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma  once

#include <QDialog>

namespace Ui {
class ChangePasswordDialog;
}

namespace WalletGui {

class ChangePasswordDialog : public QDialog {
  Q_OBJECT
  Q_DISABLE_COPY(ChangePasswordDialog)

public:
  ChangePasswordDialog(QWidget* _parent);
  ~ChangePasswordDialog();

  QString getNewPassword() const;
  QString getOldPassword() const;

private:
  QScopedPointer<Ui::ChangePasswordDialog> m_ui;

  Q_SLOT void checkPassword(const QString& _password);
};

}
