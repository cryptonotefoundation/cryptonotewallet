// Copyright (c) 2016-2022 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma  once

#include <QDialog>

namespace Ui {
class WalletRpcSettingsDialog;
}

namespace WalletGui {

class WalletRpcSettingsDialog : public QDialog {
  Q_OBJECT
  Q_DISABLE_COPY(WalletRpcSettingsDialog)

public:
  WalletRpcSettingsDialog(QWidget* _parent);
  ~WalletRpcSettingsDialog();

private:
  QScopedPointer<Ui::WalletRpcSettingsDialog> m_ui;

  Q_SLOT void saveSettingClicked();
  Q_SLOT void showPwdToggled(bool _pressed);

};

}
