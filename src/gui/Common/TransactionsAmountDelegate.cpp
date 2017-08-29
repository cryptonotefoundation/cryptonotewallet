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

#include <QLinearGradient>
#include <QPainter>

#include "TransactionsAmountDelegate.h"
#include "Settings/Settings.h"
#include "Models/TransactionsModel.h"
#include "Style/Style.h"

namespace WalletGui {

namespace {

const int AMOUNT_DECIMAL_COUNT = 3;

}

TransactionsAmountDelegate::TransactionsAmountDelegate(bool _hideLongAmounts, QObject* _parent) : QStyledItemDelegate(_parent),
  m_hideLongAmounts(_hideLongAmounts), m_amountGradient(0, 0, 1, 0) {
  m_amountGradient.setCoordinateMode(QLinearGradient::ObjectBoundingMode);
  m_amountGradient.setColorAt(0, Qt::black);
  m_amountGradient.setColorAt(0.7, Qt::black);
  m_amountGradient.setColorAt(1, Settings::instance().getCurrentStyle().backgroundColorAlternate());
}

TransactionsAmountDelegate::~TransactionsAmountDelegate() {
}

void TransactionsAmountDelegate::paint(QPainter* _painter, const QStyleOptionViewItem& _option, const QModelIndex& _index) const {
  if (!_index.isValid()) {
    QStyledItemDelegate::paint(_painter, _option, _index);
    return;
  }

  _painter->save();
  _painter->setRenderHints(QPainter::Antialiasing);
  QStyleOptionViewItem opt(_option);
  initStyleOption(&opt, _index);
  opt.decorationPosition = QStyleOptionViewItem::Right;

  opt.widget->style()->drawPrimitive(QStyle::PE_PanelItemViewItem, &opt, _painter, opt.widget);
  if (_index.data(Qt::DecorationRole).isValid()) {
    QRect iconRect = opt.widget->style()->subElementRect(QStyle::SE_ItemViewItemDecoration, &opt, opt.widget);
    opt.widget->style()->drawItemPixmap(_painter, iconRect, _option.decorationAlignment, _index.data(Qt::DecorationRole).value<QPixmap>());
  }

  QString amountString = _index.data().toString();
  if (m_hideLongAmounts) {
    int dotPos = amountString.indexOf('.');
    int decimalDigitNumber = amountString.length() - 1 - dotPos;
    if (decimalDigitNumber < AMOUNT_DECIMAL_COUNT) {
      amountString.append(QString().fill('0', AMOUNT_DECIMAL_COUNT - decimalDigitNumber));
    }

    if (decimalDigitNumber > AMOUNT_DECIMAL_COUNT && ((opt.state & QStyle::State_MouseOver) == 0)) {
      amountString = amountString.left(amountString.length() - decimalDigitNumber + AMOUNT_DECIMAL_COUNT);
      QPen pen(m_amountGradient, 0);
      _painter->setPen(pen);
    }
  }

  QRect textRect = opt.widget->style()->subElementRect(QStyle::SE_ItemViewItemText, &opt, opt.widget);
  _painter->setFont(opt.font);
  opt.widget->style()->drawItemText(_painter, textRect, opt.displayAlignment, opt.palette, opt.state & QStyle::State_Enabled, amountString);
  _painter->restore();
}

}
