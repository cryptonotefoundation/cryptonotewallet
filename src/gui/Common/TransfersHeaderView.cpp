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

#include "TransfersHeaderView.h"
#include "Models/TransfersModel.h"

namespace WalletGui {

TransfersHeaderView::TransfersHeaderView(QWidget* _parent) : QHeaderView(Qt::Horizontal, _parent) {
}

TransfersHeaderView::~TransfersHeaderView() {
}

void TransfersHeaderView::paintSection(QPainter* _painter, const QRect& _rect, int _logicalIndex) const {
  QRect rect(_rect);
  switch (_logicalIndex) {
  case TransfersModel::COLUMN_AMOUNT:
    rect.setRight(_rect.right() - 24);
    break;
  case TransfersModel::COLUMN_ADDRESS:
    rect.setRight(_rect.right() - 20);
    break;
  }


  QHeaderView::paintSection(_painter, rect, _logicalIndex);
}

QSize TransfersHeaderView::sectionSizeFromContents(int _logicalIndex) const {
  QSize res = QHeaderView::sectionSizeFromContents(_logicalIndex);
  if (_logicalIndex == TransfersModel::COLUMN_AMOUNT) {
    res.setWidth(res.width() + 20);
  }

  return res;
}

}
