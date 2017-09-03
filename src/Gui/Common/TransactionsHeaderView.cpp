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

#include "TransactionsHeaderView.h"
#include "Models/TransactionsModel.h"

namespace WalletGui {

TransactionsHeaderView::TransactionsHeaderView(QWidget* _parent) : QHeaderView(Qt::Horizontal, _parent) {
}

TransactionsHeaderView::~TransactionsHeaderView() {
}

void TransactionsHeaderView::paintSection(QPainter* _painter, const QRect& _rect, int _logicalIndex) const{
  if (_logicalIndex != TransactionsModel::COLUMN_AMOUNT) {
    QHeaderView::paintSection(_painter, _rect, _logicalIndex);
    return;
  }

  QRect rect(_rect);
  rect.setRight(_rect.right() - 24);
  QHeaderView::paintSection(_painter, rect, _logicalIndex);
}

}
