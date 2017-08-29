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

#include "Style.h"

namespace WalletGui {

class LightStyle : public Style {
public:
  LightStyle();

  virtual QString statusBarBackgroundColor() const override;
  virtual QString statusBarFontColor() const override;
  virtual QString headerBackgroundColor() const override;
  virtual QString headerBorderColor() const override;
  virtual QString addressFontColor() const override;
  virtual QString balanceFontColor() const override;
  virtual QString toolButtonBackgroundColorNormal() const override;
  virtual QString toolButtonBackgroundColorHover() const override;
  virtual QString toolButtonBackgroundColorPressed() const override;
  virtual QString toolButtonFontColorNormal() const override;
  virtual QString toolButtonFontColorDisabled() const override;
  virtual QString toolBarBorderColor() const override;

  virtual QString getWalletSyncGifFile() const override;
  virtual QPixmap getLogoPixmap() const override;
  virtual QPixmap getBalanceIcon() const override;
  virtual QPixmap getConnectedIcon() const override;
  virtual QPixmap getDisconnectedIcon() const override;
  virtual QPixmap getEncryptedIcon() const override;
  virtual QPixmap getNotEncryptedIcon() const override;
  virtual QPixmap getSyncedIcon() const override;
};

}
