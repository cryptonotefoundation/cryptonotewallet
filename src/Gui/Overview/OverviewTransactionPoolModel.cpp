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

#include <QFont>

#include "OverviewTransactionPoolModel.h"
#include "Settings/Settings.h"
#include "ICryptoNoteAdapter.h"
#include "Models/TransactionPoolModel.h"
#include "Style/Style.h"

namespace WalletGui {

namespace {

const int AMOUNT_DECIMAL_COUNT = 3;

}

OverviewTransactionPoolModel::OverviewTransactionPoolModel(ICryptoNoteAdapter* _cryptoNoteAdapter, QObject* _parent) :
  QSortFilterProxyModel(_parent), m_cryptoNoteAdapter(_cryptoNoteAdapter) {
}

OverviewTransactionPoolModel::~OverviewTransactionPoolModel() {
}

QVariant OverviewTransactionPoolModel::data(const QModelIndex& _index, int _role) const {
  int originColumn = QSortFilterProxyModel::data(_index, TransactionPoolModel::ROLE_COLUMN).toInt();
  switch (_role) {
  case Qt::DisplayRole: {
    if (originColumn == TransactionPoolModel::COLUMN_AMOUNT) {
      QString amountString = QSortFilterProxyModel::data(_index, _role).toString();
      int dotPos = amountString.indexOf('.');
      int decimalDigitNumber = amountString.length() - 1 - dotPos;
      if (decimalDigitNumber < AMOUNT_DECIMAL_COUNT) {
        amountString.append(QString().fill('0', AMOUNT_DECIMAL_COUNT - decimalDigitNumber));
      } else if (decimalDigitNumber > AMOUNT_DECIMAL_COUNT) {
        amountString = amountString.left(amountString.length() - decimalDigitNumber + AMOUNT_DECIMAL_COUNT);
      }

      amountString.append(QString(" %1").arg(m_cryptoNoteAdapter->getCurrencyTicker().toUpper()));
      return amountString;
    }

    break;
  }

  case Qt::FontRole: {
    QFont font;
    font.setPixelSize(Style::FONT_NORMAL);
    return font;
  }

  case Qt::TextColorRole: {
    if (originColumn == TransactionPoolModel::COLUMN_HASH) {
      return QColor(Settings::instance().getCurrentStyle().fontColorBlueNormal());
    } else if (originColumn == TransactionPoolModel::COLUMN_AMOUNT) {
      return QColor(Settings::instance().getCurrentStyle().fontColorGray());
    }

    break;
  }

  case Qt::TextAlignmentRole: {
    if (originColumn == TransactionPoolModel::COLUMN_HASH) {
      return static_cast<int>(Qt::AlignVCenter | Qt::AlignLeft);
    } if (originColumn == TransactionPoolModel::COLUMN_AMOUNT) {
      return static_cast<int>(Qt::AlignVCenter | Qt::AlignRight);
    }

    break;
  }


  default:
    break;
  }

  return QSortFilterProxyModel::data(_index, _role);
}

int OverviewTransactionPoolModel::rowCount(const QModelIndex& _parent) const {
  if (sourceModel() == nullptr) {
    return 0;
  } else if (sourceModel()->rowCount() > 3) {
    return 3;
  }

  return sourceModel()->rowCount();
}

bool OverviewTransactionPoolModel::filterAcceptsColumn(int _sourceColumn, const QModelIndex& _sourceParent) const {
  return _sourceColumn == TransactionPoolModel::COLUMN_HASH || _sourceColumn == TransactionPoolModel::COLUMN_AMOUNT;
}

}
