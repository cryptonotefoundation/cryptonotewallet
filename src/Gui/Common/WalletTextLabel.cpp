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

#include "WalletTextLabel.h"
#include "Settings/Settings.h"
#include "Style/Style.h"

namespace WalletGui {

namespace {

const char WALLET_TEXT_LABEL_STYLE_SHEET_TEMPLATE[] =
  "WalletGui--WalletTextLabel {"
    "border: 1px solid transparent;"
  "}"

  "WalletGui--WalletTextLabel[errorState=\"true\"] {"
      "color: %fontColorRed%;"
  "}"

  "WalletGui--WalletTinyGrayTextLabel {"
    "color: %fontColorGray%;"
    "font-size: %fontSizeTiny%;"
  "}"

  "WalletGui--WalletSmallGrayTextLabel {"
    "color: %fontColorGray%;"
    "font-size: %fontSizeSmall%;"
  "}"

  "WalletGui--WalletSmallGreenBoldTextLabel {"
    "color: %fontColorGreen%;"
    "font-size: %fontSizeSmall%;"
    "font-weight: bold;"
  "}"

  "WalletGui--WalletSmallBlackTextLabel {"
    "color: #000000;"
    "font-size: %fontSizeSmall%;"
  "}"

  "WalletGui--WalletNormalGrayTextLabel {"
    "color: %fontColorGray%;"
    "font-size: %fontSizeNormal%;"
  "}"

  "WalletGui--WalletNormalGreenBoldTextLabel {"
    "color: %fontColorGreen%;"
    "font-size: %fontSizeNormal%;"
    "font-weight: bold;"
  "}"

  "WalletGui--WalletExtraNormalGrayTextLabel {"
    "color: %fontColorGray%;"
    "font-size: %fontSizeExtraNormal%;"
  "}"

  "WalletGui--WalletNormalBlackTextLabel {"
    "color: #000000;"
    "font-size: %fontSizeNormal%;"
  "}"

  "WalletGui--WalletExtraNormalBlackTextLabel {"
    "color: #000000;"
    "font-size: %fontSizeExtraNormal%;"
  "}"

  "WalletGui--WalletLargeBlackTextLabel {"
    "color: #000000;"
    "font-size: %fontSizeLarge%;"
  "}"

  "WalletGui--WalletExtraLargeBlackTextLabel {"
    "color: #000000;"
    "font-size: %fontSizeExtraLarge%;"
  "}"

  "WalletGui--WalletHeaderLabel {"
    "min-height: 17;"
    "font-size: %fontSizeLarge%;"
    "font-weight: bold;"
  "}";

}

WalletTextLabel::WalletTextLabel(QWidget* _parent) : QLabel(_parent) {
  setStyleSheet(Settings::instance().getCurrentStyle().makeStyleSheet(WALLET_TEXT_LABEL_STYLE_SHEET_TEMPLATE));
}

WalletTextLabel::~WalletTextLabel() {
}

WalletTinyGrayTextLabel::WalletTinyGrayTextLabel(QWidget* _parent) : WalletTextLabel(_parent) {
}

WalletTinyGrayTextLabel::~WalletTinyGrayTextLabel() {
}

WalletSmallGrayTextLabel::WalletSmallGrayTextLabel(QWidget* _parent) : WalletTextLabel(_parent) {
}

WalletSmallGrayTextLabel::~WalletSmallGrayTextLabel() {
}

WalletSmallGreenBoldTextLabel::WalletSmallGreenBoldTextLabel(QWidget* _parent) : WalletTextLabel(_parent) {
}

WalletSmallGreenBoldTextLabel::~WalletSmallGreenBoldTextLabel() {
}

WalletSmallBlackTextLabel::WalletSmallBlackTextLabel(QWidget* _parent) : WalletTextLabel(_parent) {
}

WalletSmallBlackTextLabel::~WalletSmallBlackTextLabel() {
}

WalletNormalGrayTextLabel::WalletNormalGrayTextLabel(QWidget* _parent) : WalletTextLabel(_parent) {
}

WalletNormalGrayTextLabel::~WalletNormalGrayTextLabel() {
}

WalletNormalGreenBoldTextLabel::WalletNormalGreenBoldTextLabel(QWidget* _parent) : WalletTextLabel(_parent) {
}

WalletNormalGreenBoldTextLabel::~WalletNormalGreenBoldTextLabel() {
}

WalletExtraNormalGrayTextLabel::WalletExtraNormalGrayTextLabel(QWidget* _parent) : WalletTextLabel(_parent) {
}

WalletExtraNormalGrayTextLabel::~WalletExtraNormalGrayTextLabel() {
}

WalletNormalBlackTextLabel::WalletNormalBlackTextLabel(QWidget* _parent) : WalletTextLabel(_parent) {
}

WalletNormalBlackTextLabel::~WalletNormalBlackTextLabel() {
}

WalletExtraNormalBlackTextLabel::WalletExtraNormalBlackTextLabel(QWidget* _parent) : WalletTextLabel(_parent) {
}

WalletExtraNormalBlackTextLabel::~WalletExtraNormalBlackTextLabel() {
}

WalletLargeBlackTextLabel::WalletLargeBlackTextLabel(QWidget* _parent) : WalletTextLabel(_parent) {
}

WalletLargeBlackTextLabel::~WalletLargeBlackTextLabel() {
}

WalletExtraLargeBlackTextLabel::WalletExtraLargeBlackTextLabel(QWidget* _parent) : WalletTextLabel(_parent) {
}

WalletExtraLargeBlackTextLabel::~WalletExtraLargeBlackTextLabel() {
}

WalletHeaderLabel::WalletHeaderLabel(QWidget* _parent) : WalletTextLabel(_parent) {
}

WalletHeaderLabel::~WalletHeaderLabel() {
}

}
