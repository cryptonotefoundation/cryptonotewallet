

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

#include <QMouseEvent>

#include "OverviewTransactionPoolTreeView.h"
#include "Models/TransactionPoolModel.h"

namespace WalletGui {

OverviewTransactionPoolTreeView::OverviewTransactionPoolTreeView(QWidget* _parent) : QTreeView(_parent) {
  setMouseTracking(true);
  connect(this, &OverviewTransactionPoolTreeView::clicked, this, &OverviewTransactionPoolTreeView::itemClicked);
}

OverviewTransactionPoolTreeView::~OverviewTransactionPoolTreeView() {
}

void OverviewTransactionPoolTreeView::mouseMoveEvent(QMouseEvent* _event) {
  QPoint pos = _event->pos();
  QModelIndex index = indexAt(pos);
  if (index.isValid() && index.data(TransactionPoolModel::ROLE_COLUMN).toInt() == TransactionPoolModel::COLUMN_HASH) {
    setCursor(Qt::PointingHandCursor);
  } else {
    setCursor(Qt::ArrowCursor);
  }

  QTreeView::mouseMoveEvent(_event);
}

void OverviewTransactionPoolTreeView::itemClicked(const QModelIndex& _index) {
  if (_index.isValid() && _index.data(TransactionPoolModel::ROLE_COLUMN).toInt() == TransactionPoolModel::COLUMN_HASH) {
    Q_EMIT doubleClicked(_index);
  }
}

}
