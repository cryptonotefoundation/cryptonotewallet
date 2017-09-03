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

#include "WalletBlueButton.h"
#include "Settings/Settings.h"
#include "Style/Style.h"

namespace WalletGui {

namespace {

const char WALLET_BLUE_BUTTON_STYLE_SHEET_TEMPLATE[] =
  "* {"
    "font-family: %fontFamily%;"
  "}"

  "WalletGui--WalletBlueButton {"
    "font-size: %fontSizeNormal%;"
    "color: #ffffff;"
    "background-color: %backgroundColorBlueNormal%;"
    "border: 1px solid %backgroundColorBlueNormal%;"
  "}"

  "WalletGui--WalletBlueButton:disabled {"
    "background-color: %backgroundColorBlueDisabled%;"
    "border: 1px solid %backgroundColorBlueDisabled%;"
  "}"

  "WalletGui--WalletBlueButton:hover {"
    "background-color: %backgroundColorBlueHover%;"
    "border: 1px solid %backgroundColorBlueHover%;"
  "}"

  "WalletGui--WalletBlueButton:pressed {"
    "background-color: %backgroundColorBluePressed%;"
    "border: 1px solid %backgroundColorBluePressed%;"
  "}"

  "WalletGui--WalletNormalBlueButton {"
    "min-height: 30px;"
    "max-height: 30px;"
    "font-size: %fontSizeNormal%;"
  "}"

  "WalletGui--WalletLargeBlueButton {"
    "min-height: 40px;"
    "max-height: 40px;"
    "font-size: %fontSizeLarge%;"
  "}";

}

WalletBlueButton::WalletBlueButton(QWidget* _parent) : QPushButton(_parent) {
  setStyleSheet(Settings::instance().getCurrentStyle().makeStyleSheet(WALLET_BLUE_BUTTON_STYLE_SHEET_TEMPLATE));
}

WalletBlueButton::~WalletBlueButton() {
}

WalletNormalBlueButton::WalletNormalBlueButton(QWidget* _parent) : WalletBlueButton(_parent) {
}

WalletNormalBlueButton::~WalletNormalBlueButton() {
}

WalletLargeBlueButton::WalletLargeBlueButton(QWidget* _parent) : WalletBlueButton(_parent) {
}

WalletLargeBlueButton::~WalletLargeBlueButton() {
}

}
