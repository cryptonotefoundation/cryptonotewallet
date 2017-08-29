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

#include <QPainter>

#include "LinkLikeColumnDelegate.h"
#include "Settings/Settings.h"
#include "Style/Style.h"

namespace WalletGui {

LinkLikeColumnDelegate::LinkLikeColumnDelegate(QObject* _parent) : QStyledItemDelegate(_parent) {
}

LinkLikeColumnDelegate::~LinkLikeColumnDelegate() {
}

void LinkLikeColumnDelegate::paint(QPainter* _painter, const QStyleOptionViewItem& _option, const QModelIndex& _index) const {
  if (_index.isValid()) {
    QStyleOptionViewItem opt(_option);
     _painter->save();
     initStyleOption(&opt, _index);
    if (opt.state & QStyle::State_MouseOver) {
      QColor textColor(Settings::instance().getCurrentStyle().fontColorBlueHover());
      _painter->setPen(textColor);
    } else {
      QColor textColor(Settings::instance().getCurrentStyle().fontColorBlueNormal());
      _painter->setPen(textColor);
    }

    _painter->setFont(opt.font);
    QFontMetrics fm(opt.font);
    QRect textRect = opt.widget->style()->subElementRect(QStyle::SE_ItemViewItemText, &opt, opt.widget);
    QString elidedText = fm.elidedText(opt.text, opt.textElideMode, textRect.width());
    opt.widget->style()->drawPrimitive(QStyle::PE_PanelItemViewItem, &opt, _painter, opt.widget);
    opt.widget->style()->drawItemText(_painter, textRect, opt.displayAlignment, opt.palette, opt.state & QStyle::State_Enabled, elidedText);
    _painter->restore();
    return;
  }

  QStyledItemDelegate::paint(_painter, _option, _index);
}

}
