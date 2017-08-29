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

#include <QApplication>
#include <QClipboard>
#include <QMenu>
#include <QPushButton>

#include "AddressBookDelegate.h"
#include "Models/AddressBookModel.h"

namespace WalletGui {

AddressBookDelegate::AddressBookDelegate(QObject* _parent) : QStyledItemDelegate(_parent) {
}

AddressBookDelegate::~AddressBookDelegate() {
}

QWidget* AddressBookDelegate::createEditor(QWidget* _parent, const QStyleOptionViewItem& _option, const QModelIndex& _index) const {
  if (!_index.isValid() || _index.column() != AddressBookModel::COLUMN_ACTION) {
    return QStyledItemDelegate::createEditor(_parent, _option, _index);
  }

  QPushButton* editor = new QPushButton(_parent);
  editor->setObjectName("m_addressBookMenuButton");
  editor->setFocusPolicy(Qt::NoFocus);
  QMenu* menu = new QMenu(editor);
  menu->setObjectName("m_addressBookMenu");
  QAction* sendAction = new QAction(tr("Send"), menu);
  QAction* editAction = new QAction(tr("Edit"), menu);
  QAction* copyAction = new QAction(tr("Copy to clipboard"), menu);
  QAction* delAction = new QAction(tr("Delete"), menu);
  menu->addAction(sendAction);
  menu->addAction(editAction);
  menu->addAction(copyAction);
  menu->addAction(delAction);

  editor->setMenu(menu);
  editor->setCursor(Qt::PointingHandCursor);
  QPersistentModelIndex index(_index);
  connect(sendAction, &QAction::triggered, [this, index]() {
      Q_EMIT sendToSignal(index.data(AddressBookModel::ROLE_ADDRESS).toString());
    });
  connect(editAction, &QAction::triggered, [this, index]() {
      Q_EMIT editSignal(index);
    });
  connect(copyAction, &QAction::triggered, [index]() {
      QApplication::clipboard()->setText(index.data(AddressBookModel::ROLE_ADDRESS).toString());
    });
  connect(delAction, &QAction::triggered, [this, index]() {
      Q_EMIT deleteSignal(index);
    });

  return editor;
}

void AddressBookDelegate::updateEditorGeometry(QWidget* _editor, const QStyleOptionViewItem& _option, const QModelIndex& _index) const {
  _editor->setGeometry(_option.rect);
}

}
