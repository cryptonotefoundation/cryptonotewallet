// Copyright (c) 2017-2018, The Karbo developers
//
// This file is part of Karbo.
//
// Karbovanets is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Karbovanets is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Karbovanets. If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <QFrame>

namespace Ui {
  class NoWalletFrame;
}

namespace WalletGui {

class NoWalletFrame : public QFrame {
  Q_OBJECT
  Q_DISABLE_COPY(NoWalletFrame)

public:
  NoWalletFrame(QWidget* _parent);
  ~NoWalletFrame();

 Q_SLOT void createWallet();
 Q_SLOT void openWallet();
  
private:
  QScopedPointer<Ui::NoWalletFrame> m_ui;

Q_SIGNALS:
  void createWalletClickedSignal();
  void openWalletClickedSignal();
};

}
