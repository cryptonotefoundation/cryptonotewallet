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

#pragma once

#include <QStyledItemDelegate>

namespace WalletGui {

class NewTransactionDelegate : public QStyledItemDelegate {
  Q_OBJECT
  Q_DISABLE_COPY(NewTransactionDelegate)

public:
  explicit NewTransactionDelegate(QObject* _parent);
  ~NewTransactionDelegate();
  QWidget* createEditor(QWidget* _parent, const QStyleOptionViewItem& _option, const QModelIndex& _index) const override;
  void updateEditorGeometry(QWidget* _editor, const QStyleOptionViewItem& _option, const QModelIndex& _index) const override;
  void setEditorData(QWidget* _editor, const QModelIndex& _index) const override;
};

}
