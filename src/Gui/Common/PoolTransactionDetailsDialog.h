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

#include <QDialog>
#include <QPersistentModelIndex>

namespace Ui {
  class PoolTransactionDetailsDialog;
}

namespace WalletGui {

class PoolTransactionDetailsDialog : public QDialog {
  Q_OBJECT
  Q_DISABLE_COPY(PoolTransactionDetailsDialog)

public:
  explicit PoolTransactionDetailsDialog(QAbstractItemModel* _transactionPoolModel, const QModelIndex& _index, QWidget* _parent);
  ~PoolTransactionDetailsDialog();

  virtual bool eventFilter(QObject* _object, QEvent* _event) override;

protected:
  void showEvent(QShowEvent* _event) override;

private:
  QScopedPointer<Ui::PoolTransactionDetailsDialog> m_ui;
  QPersistentModelIndex m_index;
};

}
