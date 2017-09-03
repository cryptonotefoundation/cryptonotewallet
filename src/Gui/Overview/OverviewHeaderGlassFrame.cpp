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

#include <QHBoxLayout>
#include <QLabel>
#include <QMovie>

#include "OverviewHeaderGlassFrame.h"
#include "Settings/Settings.h"
#include "Style/Style.h"

namespace WalletGui {

namespace {

const char BALANCES_GLASS_FRAME_STYLE_SHEET_TEMPLATE[] =
  "WalletGui--OverviewHeaderGlassFrame {"
    "border: none;"
    "background-color: #c8f4f4f4;"
    "border-right: 1px solid %borderColor%;"
  "}";

}

OverviewHeaderGlassFrame::OverviewHeaderGlassFrame(QMovie* _syncMovie, QWidget* _parent) : GlassFrame(_parent),
  m_syncLabel(new QLabel(this)) {
  setStyleSheet(Settings::instance().getCurrentStyle().makeStyleSheet(BALANCES_GLASS_FRAME_STYLE_SHEET_TEMPLATE));
  m_syncLabel->setMovie(_syncMovie);
  QHBoxLayout* layout = new QHBoxLayout(this);
  setLayout(layout);
  layout->addWidget(m_syncLabel, 0, Qt::AlignCenter);
}

OverviewHeaderGlassFrame::~OverviewHeaderGlassFrame() {
}

void OverviewHeaderGlassFrame::install(QWidget* _parent) {
  GlassFrame::install(_parent);
}

void OverviewHeaderGlassFrame::remove() {
  GlassFrame::remove();
}

}
