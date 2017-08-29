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

#include <QPixmap>

namespace WalletGui {

class Style {
public:
  enum FontSize {
    FONT_TINY = 10, FONT_SMALL = 11, FONT_NORMAL = 12, FONT_EXTRA_NORMAL = 13, FONT_LARGE = 14, FONT_EXTRA_LARGE = 16, FONT_HUGE = 20
  };

  virtual ~Style();
  QString getStyleId() const;
  QString getStyleName() const;

  virtual QString fontFamily() const;
  virtual int viewItemHeight() const;
  virtual QString fontColorGray() const;
  virtual QString fontColorGreen() const;
  virtual QString fontColorRed() const;
  virtual QString fontColorBlueNormal() const;
  virtual QString fontColorBlueHover() const;
  virtual QString fontColorBluePressed() const;
  virtual QString backgroundColorGray() const;
  virtual QString backgroundButtonColorGrayHover() const;
  virtual QString backgroundColorBlueNormal() const;
  virtual QString backgroundColorBlueHover() const;
  virtual QString backgroundColorBluePressed() const;
  virtual QString backgroundColorBlueDisabled() const;
  virtual QString backgroundColorGrayNormal() const;
  virtual QString backgroundColorGrayHover() const;
  virtual QString backgroundColorGrayPressed() const;
  virtual QString backgroundColorGreen() const;
  virtual QString backgroundColorGreenHover() const;
  virtual QString backgroundColorAlternate() const;
  virtual QString borderColor() const;
  virtual QString borderColorDark() const;
  virtual QString selectionColor() const;
  virtual QString glassColor() const;

  virtual QString getWalletSyncGifFile() const = 0;
  virtual QIcon getSystemTrayIcon() const;
  virtual QPixmap getLogoPixmap() const = 0;
  virtual QPixmap getBalanceIcon() const = 0;
  virtual QPixmap getConnectedIcon() const = 0;
  virtual QPixmap getDisconnectedIcon() const = 0;
  virtual QPixmap getEncryptedIcon() const = 0;
  virtual QPixmap getNotEncryptedIcon() const = 0;
  virtual QPixmap getSyncedIcon() const = 0;
  virtual QString makeStyleSheet(const QString& _styleSheetTemplate) const;

  virtual QString statusBarBackgroundColor() const = 0;
  virtual QString statusBarFontColor() const = 0;
  virtual QString headerBackgroundColor() const = 0;
  virtual QString headerBorderColor() const = 0;
  virtual QString addressFontColor() const = 0;
  virtual QString balanceFontColor() const = 0;
  virtual QString toolButtonBackgroundColorNormal() const = 0;
  virtual QString toolButtonBackgroundColorHover() const = 0;
  virtual QString toolButtonBackgroundColorPressed() const = 0;
  virtual QString toolButtonFontColorNormal() const = 0;
  virtual QString toolButtonFontColorDisabled() const = 0;
  virtual QString toolBarBorderColor() const = 0;

protected:
  Style(const QString& _styleId, const QString& _styleName);

private:
  const QString m_styleId;
  const QString m_styleName;
};

}
