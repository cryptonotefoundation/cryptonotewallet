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

#include "NewTransactionDelegate.h"

#include "Gui/Common/MagicLabel.h"

#include "Models/TransactionsModel.h"

namespace WalletGui {

NewTransactionDelegate::NewTransactionDelegate(QObject* _parent) : QStyledItemDelegate(_parent) {
}

NewTransactionDelegate::~NewTransactionDelegate() {
}

QWidget* NewTransactionDelegate::createEditor(QWidget* _parent, const QStyleOptionViewItem& _option, const QModelIndex& _index) const {
  MagicLabel* newTransactionLabel = new MagicLabel(1000, 7000, _parent);
  connect(newTransactionLabel, &MagicLabel::completedSignal, newTransactionLabel, &MagicLabel::deleteLater);
  newTransactionLabel->setObjectName("newTransactionLabel");
  return newTransactionLabel;
}

void NewTransactionDelegate::updateEditorGeometry(QWidget* _editor, const QStyleOptionViewItem& _option, const QModelIndex& _index) const {
  _editor->setGeometry(_option.rect);
}

void NewTransactionDelegate::setEditorData(QWidget* _editor, const QModelIndex& _index) const {
  static_cast<MagicLabel*>(_editor)->start();
}

}
