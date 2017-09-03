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

#include "WalletNavigationButton.h"
#include "Settings/Settings.h"
#include "Style/Style.h"

namespace WalletGui {

namespace {

const char WALLET_NAVIGATION_BUTTON_STYLE_SHEET_TEMPLATE[] =
  "* {"
    "font-family: %fontFamily%;"
  "}"

  "WalletGui--WalletNavigationButton {"
    "min-width: 136px;"
    "max-width: 136px;"
    "min-height: 30px;"
    "max-height: 30px;"
    "font-size: %fontSizeSmall%;"
    "border-radius: 2px;"
    "border: 1px solid %borderColor%;"
    "background-color: %backgroundColorGray%;"
    "color: %fontColorBlueNormal%;"
  "}"

  "WalletGui--WalletNavigationButton:hover {"
    "background-color: #ffffff;"
    "color: %fontColorBlueHover%;"
  "}"

  "WalletGui--WalletNavigationButton:pressed {"
    "background-color: %backgroundColorGray%;"
    "color: %fontColorBluePressed%;"
  "}";

}

WalletNavigationButton::WalletNavigationButton(QWidget* _parent) : QPushButton(_parent) {
  setStyleSheet(Settings::instance().getCurrentStyle().makeStyleSheet(WALLET_NAVIGATION_BUTTON_STYLE_SHEET_TEMPLATE));
}

WalletNavigationButton::~WalletNavigationButton() {
}

}
