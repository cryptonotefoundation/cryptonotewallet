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

#include "WalletLinkLikeButton.h"
#include "Settings/Settings.h"
#include "Style/Style.h"

namespace WalletGui {

namespace {

const char WALLET_LINK_LIKE_BUTTON_STYLE_SHEET_TEMPLATE[] =
  "* {"
    "font-family: %fontFamily%;"
  "}"

  "WalletGui--WalletLinkLikeButton {"
    "border: 1px solid transparent;"
    "background-color: transparent;"
    "color: %fontColorBlueNormal%;"
  "}"

  "WalletGui--WalletLinkLikeButton:hover {"
    "color: %fontColorBlueHover%;"
  "}"

  "WalletGui--WalletLinkLikeButton:pressed {"
    "color: %fontColorBluePressed%;"
  "}"

  "WalletGui--WalletTinyLinkLikeButton {"
    "font-size: %fontSizeTiny%;"
    "min-height: 12px;"
  "}"

  "WalletGui--WalletNormalLinkLikeButton {"
    "font-size: %fontSizeNormal%;"
    "min-height: %fontSizeNormal%;"
  "}";

}

WalletLinkLikeButton::WalletLinkLikeButton(QWidget* _parent) : QPushButton(_parent) {
  setStyleSheet(Settings::instance().getCurrentStyle().makeStyleSheet(WALLET_LINK_LIKE_BUTTON_STYLE_SHEET_TEMPLATE));
}

WalletLinkLikeButton::~WalletLinkLikeButton() {
}

WalletTinyLinkLikeButton::WalletTinyLinkLikeButton(QWidget* _parent) : WalletLinkLikeButton(_parent) {
}

WalletTinyLinkLikeButton::~WalletTinyLinkLikeButton() {
}

WalletNormalLinkLikeButton::WalletNormalLinkLikeButton(QWidget* _parent) : WalletLinkLikeButton(_parent) {
}

WalletNormalLinkLikeButton::~WalletNormalLinkLikeButton() {
}

}
