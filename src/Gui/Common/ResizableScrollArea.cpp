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

#include "ResizableScrollArea.h"

namespace WalletGui {

ResizableScrollArea::ResizableScrollArea(QWidget* _parent) : QScrollArea(_parent) {
}

ResizableScrollArea::~ResizableScrollArea() {
}

QSize ResizableScrollArea::sizeHint() const {
  if (widget() == nullptr || widget()->height() == 0) {
    return QScrollArea::sizeHint();
  }

  QList<QWidget*> childList = widget()->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly);
  int height = 0;
  for (QWidget* widgetItem : childList) {
    height += widgetItem->height();
  }

  return QSize(width(), height + 2);
}

}
