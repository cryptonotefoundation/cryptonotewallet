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

#include "PrivacyFrame.h"
#include "Settings/Settings.h"
#include "Style/Style.h"
#include "ui_PrivacyFrame.h"

namespace WalletGui {

namespace {

const char PRIVACY_FRAME_STYLE_SHEET_TEMPLATE[] =
  "WalletGui--PrivacyFrame {"
    "border: none;"
    "background-color: #ffffff;"
  "}"

  "WalletGui--PrivacyFrame QCheckBox {"
    "spacing: 20px;"
    "min-height: 25px;"
    "max-height: 25px;"
  "}";

}

PrivacyFrame::PrivacyFrame(QWidget* _parent) : QFrame(_parent), m_ui(new Ui::PrivacyFrame) {
  m_ui->setupUi(this);
  setStyleSheet(Settings::instance().getCurrentStyle().makeStyleSheet(PRIVACY_FRAME_STYLE_SHEET_TEMPLATE));
}

PrivacyFrame::~PrivacyFrame() {
}

void PrivacyFrame::load() {
  m_ui->m_newsCheck->setChecked(Settings::instance().isNewsEnabled());
}

void PrivacyFrame::save() {
  Settings::instance().setNewsEnabled(m_ui->m_newsCheck->isChecked());
}

void PrivacyFrame::setData(const QVariantMap& _data) {

}

bool PrivacyFrame::isEnabled() const {
  return true;
}

bool PrivacyFrame::needToRestartApplication() const {
  return false;
}

bool PrivacyFrame::canAccept() const {
  return true;
}

}
