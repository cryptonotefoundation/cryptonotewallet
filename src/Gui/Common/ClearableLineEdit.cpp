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
#include <QPushButton>
#include <QSizePolicy>

#include "ClearableLineEdit.h"
#include "Settings/Settings.h"
#include "Style/Style.h"

namespace WalletGui {

const char CLEAR_BUTTON_STYLE_SHEET_TEMPLATE[] =
  "* {"
    "font-family: %fontFamily%;"
  "}"

  "QPushButton {"
    "border: none;"
    "font-size: %fontSizeTiny%;"
    "min-width: 5px;"
    "min-height: 5px;"
    "color: %fontColorGray%;"
  "}"

  "QPushButton:hover {"
    "color: %fontColorBlueHover%;"
  "}";

ClearableLineEdit::ClearableLineEdit(QWidget* _parent) : QLineEdit(_parent) {
  QHBoxLayout* layout = new QHBoxLayout(this);
  QPushButton* clearButton = new QPushButton(this);
  clearButton->setStyleSheet(Settings::instance().getCurrentStyle().makeStyleSheet(CLEAR_BUTTON_STYLE_SHEET_TEMPLATE));
  clearButton->setText(tr("clear"));
  clearButton->setCursor(Qt::PointingHandCursor);
  clearButton->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
  clearButton->adjustSize();
  setLayout(layout);
  layout->addWidget(clearButton, 0, Qt::AlignVCenter | Qt::AlignRight);
  QMargins margings = textMargins();
  margings.setRight(margings.right() + clearButton->width() + 10);
  setTextMargins(margings);
  connect(clearButton, &QPushButton::clicked, this, &ClearableLineEdit::clear);
}

ClearableLineEdit::~ClearableLineEdit() {
}

}
