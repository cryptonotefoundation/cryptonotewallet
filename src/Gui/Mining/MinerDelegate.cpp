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

#include <QAbstractItemView>

#include "MinerDelegate.h"
#include "Style/Style.h"
#include "Models/MinerModel.h"

namespace WalletGui {

MinerRemoveDelegate::MinerRemoveDelegate(QAbstractItemView* _view, QObject* _parent) : QStyledItemDelegate(_parent),
  m_view(_view) {
}

MinerRemoveDelegate::~MinerRemoveDelegate() {
}

void MinerRemoveDelegate::paint(QPainter* _painter, const QStyleOptionViewItem& _option, const QModelIndex& _index) const {
  if (_index.column() == MinerModel::COLUMN_REMOVE) {
    QStyleOptionViewItem opt(_option);
    if ((opt.state & QStyle::State_MouseOver) && opt.rect.contains(m_view->viewport()->mapFromGlobal(QCursor::pos()))) {
      initStyleOption(&opt, _index);
      QRect pixmapRect = opt.widget->style()->subElementRect(QStyle::SE_ItemViewItemDecoration, &opt, opt.widget);
      opt.widget->style()->drawPrimitive(QStyle::PE_PanelItemViewItem, &opt, _painter, opt.widget);
      opt.widget->style()->drawItemPixmap(_painter, pixmapRect, opt.displayAlignment, QPixmap(":icons/delete_pool_hover"));
      return;
    }
  }

  QStyledItemDelegate::paint(_painter, _option, _index);
}

}
