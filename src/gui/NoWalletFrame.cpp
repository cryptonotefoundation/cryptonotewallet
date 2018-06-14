// Copyright (c) 2017-2018, The karbo developers
//
// This file is part of karbo.
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

#include "MainWindow.h"
#include "NoWalletFrame.h"

#include "ui_nowalletframe.h"

namespace WalletGui {

NoWalletFrame::NoWalletFrame(QWidget* _parent) : QFrame(_parent), m_ui(new Ui::NoWalletFrame) {
  m_ui->setupUi(this);
}

NoWalletFrame::~NoWalletFrame() {
}

void NoWalletFrame::createWallet() {
  Q_EMIT createWalletClickedSignal();
}

void NoWalletFrame::openWallet() {
  Q_EMIT openWalletClickedSignal();
}

}
