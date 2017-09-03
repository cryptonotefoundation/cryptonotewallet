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

#include <QRegExp>
#include <QUrl>

#include "OptionsDialog.h"
#include "Application/IWalletUiItem.h"
#include "Settings/Settings.h"
#include "IOptionsPage.h"
#include "Style/Style.h"

#include "ui_OptionsDialog.h"

namespace WalletGui {

namespace {

const char OPTIONS_DIALOG_STYLE_SHEET_TEMPLATE[] =
  "WalletGui--OptionsDialog {"
    "min-height: 563px;"
    "max-height: 563px;"
    "background-color: %backgroundColorGray%;"
  "}"

  "WalletGui--OptionsDialog > #m_buttonFrame {"
    "background: #ffffff;"
  "}"

  "WalletGui--OptionsDialog > #m_buttonFrame > #m_warningLabel {"
    "font-size: %fontSizeLarge%;"
    "font-weight: bold;"
  "}"

  "WalletGui--OptionsDialog > #m_optionsTabWidget {"
    "border: none;"
  "}"

  "WalletGui--OptionsDialog > #m_optionsTabWidget::pane {"
    "border: none;"
    "border-top: 1px solid %borderColorDark%;"
    "top: -1px;"
  "}"

  "WalletGui--OptionsDialog > #m_optionsTabWidget::tab-bar {"
    "alignment: center;"
  "}"

  "WalletGui--OptionsDialog > #m_optionsTabWidget QTabBar::tab {"
    "font-size: %fontSizeLarge%;"
    "font-weight: bold;"
    "min-width: 165px;"
    "max-width: 165px;"
    "min-height: 30px;"
    "max-height: 30px;"
    "border: 1px solid %borderColorDark%;"
    "margin-left: 2px;"
    "margin-right: 2px;"
    "border-top-left-radius: 2px;"
    "border-top-right-radius: 2px;"
  "}"

  "WalletGui--OptionsDialog > #m_optionsTabWidget QTabBar::tab:selected {"
    "background: #ffffff;"
    "border: 1px solid %borderColorDark%;"
    "border-bottom: 1px solid #ffffff;"
  "}"

  "WalletGui--OptionsDialog > #m_optionsTabWidget QTabBar::tab:!selected {"
    "background: %backgroundColorGray%;"
    "border: 1px solid %borderColorDark%;"
  "}";

}

OptionsDialog::OptionsDialog(ICryptoNoteAdapter* _cryptoNoteAdapter, IDonationManager* _donationManager,
  IOptimizationManager* _optimizationManager, QAbstractItemModel* _addressBookModel, QWidget* _parent) :
  QDialog(_parent, static_cast<Qt::WindowFlags>(Qt::WindowCloseButtonHint)), m_ui(new Ui::OptionsDialog), m_needToRestart(false) {
  m_ui->setupUi(this);
  m_ui->m_warningLabel->hide();
  for (int i = 0; i < m_ui->m_optionsTabWidget->count(); ++i) {
    IOptionsPage* page = dynamic_cast<IOptionsPage*>(m_ui->m_optionsTabWidget->widget(i));
    IWalletUiItem* uiItem = dynamic_cast<IWalletUiItem*>(m_ui->m_optionsTabWidget->widget(i));
    QObject* pageObject = qobject_cast<QObject*>(m_ui->m_optionsTabWidget->widget(i));
    connect(pageObject, SIGNAL(disableAcceptButtonSignal(bool)), this, SLOT(disableAcceptButton(bool)));
    connect(pageObject, SIGNAL(showRestartWarningSignal(bool)), this, SLOT(showRestartWarning(bool)));
    uiItem->setCryptoNoteAdapter(_cryptoNoteAdapter);
    uiItem->setDonationManager(_donationManager);
    uiItem->setOptimizationManager(_optimizationManager);
    uiItem->setAddressBookModel(_addressBookModel);
    page->load();
    m_ui->m_optionsTabWidget->setTabEnabled(i, page->isEnabled());
  }

  setStyleSheet(Settings::instance().getCurrentStyle().makeStyleSheet(OPTIONS_DIALOG_STYLE_SHEET_TEMPLATE));
}

OptionsDialog::~OptionsDialog() {
}

bool OptionsDialog::needToRestartApplication() const {
  return m_needToRestart;
}

void OptionsDialog::accept() {
  for (int i = 0; i < m_ui->m_optionsTabWidget->count(); ++i) {
    IOptionsPage* page = dynamic_cast<IOptionsPage*>(m_ui->m_optionsTabWidget->widget(i));
    page->save();
  }

  QDialog::accept();
}

void OptionsDialog::showRestartWarning(bool _show) {
  m_needToRestart = false;
  for (int i = 0; i < m_ui->m_optionsTabWidget->count(); ++i) {
    IOptionsPage* page = dynamic_cast<IOptionsPage*>(m_ui->m_optionsTabWidget->widget(i));
    m_needToRestart |= page->needToRestartApplication();
  }

  m_ui->m_warningLabel->setVisible(m_needToRestart);
}

void OptionsDialog::disableAcceptButton(bool _disable) {
  bool canAccept = true;
  for (int i = 0; i < m_ui->m_optionsTabWidget->count(); ++i) {
    IOptionsPage* page = dynamic_cast<IOptionsPage*>(m_ui->m_optionsTabWidget->widget(i));
    canAccept &= page->canAccept();
  }

  m_ui->m_okButton->setEnabled(canAccept);
}

void WalletGui::OptionsDialog::setDonationAddress(const QString& _label, const QString& _address) {
  QVariantMap params;
  params.insert("label", _label);
  params.insert("address", _address);
  m_ui->m_donationTab->setData(params);
  int donationTabIndex = m_ui->m_optionsTabWidget->indexOf(m_ui->m_donationTab);
  m_ui->m_optionsTabWidget->setCurrentIndex(donationTabIndex);
}

}
