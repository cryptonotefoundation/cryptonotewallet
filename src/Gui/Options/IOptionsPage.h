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

#include <QObject>

namespace WalletGui {

class IOptionsPage {

public:
  virtual void load() = 0;
  virtual void save() = 0;
  virtual void setData(const QVariantMap& _data) = 0;
  virtual bool isEnabled() const = 0;
  virtual bool needToRestartApplication() const = 0;
  virtual bool canAccept() const = 0;

Q_SIGNALS:
  virtual void showRestartWarningSignal(bool _show) = 0;
  virtual void disableAcceptButtonSignal(bool _disable) = 0;
};

}

Q_DECLARE_INTERFACE(WalletGui::IOptionsPage, "WalletGui::IOptionsPage")
