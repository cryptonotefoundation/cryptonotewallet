// Copyright (c) 2016-2022 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "WalletRpcSettings.h"
#include "Settings.h"
#include "MainWindow.h"

#include "ui_walletrpcsettingsdialog.h"

namespace Ui {
class WalletRpcSettingsDialog;
}

namespace WalletGui {

WalletRpcSettingsDialog::WalletRpcSettingsDialog(QWidget *_parent) : QDialog(_parent),
    m_ui(new Ui::WalletRpcSettingsDialog)
{
  m_ui->setupUi(this);
  m_ui->m_enableWalletRpcCheck->setChecked(Settings::instance().runWalletRpc());
  m_ui->m_walletRpcIp->setText(Settings::instance().getWalletRpcBindIp());
  m_ui->m_walletRpcPort->setValue(Settings::instance().getWalletRpcBindPort());
  m_ui->m_walletRpcUser->setText(Settings::instance().getWalletRpcUser());
  m_ui->m_walletRpcPassword->setText(Settings::instance().getWalletRpcPassword());
}

WalletRpcSettingsDialog::~WalletRpcSettingsDialog() {
}

void WalletRpcSettingsDialog::saveSettingClicked() {
  Settings::instance().setRunWalletRpc(m_ui->m_enableWalletRpcCheck->isChecked());
  Settings::instance().setWalletRpcBindIp(m_ui->m_walletRpcIp->text().trimmed());
  Settings::instance().setWalletRpcBindPort(m_ui->m_walletRpcPort->value());
  Settings::instance().setWalletRpcUser(m_ui->m_walletRpcUser->text().trimmed());
  Settings::instance().setWalletRpcPassword(m_ui->m_walletRpcPassword->text().trimmed());

  accept();
}

void WalletRpcSettingsDialog::showPwdToggled(bool _pressed) {
  if(_pressed)
    m_ui->m_walletRpcPassword->setEchoMode(QLineEdit::Normal);
  else
    m_ui->m_walletRpcPassword->setEchoMode(QLineEdit::Password);
}

}
