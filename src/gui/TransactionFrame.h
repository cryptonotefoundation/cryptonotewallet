// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <QDataWidgetMapper>
#include <QFrame>
#include <QModelIndex>
#include <QStyledItemDelegate>

namespace Ui {
class TransactionFrame;
}

namespace WalletGui {

class TransactionFrame : public QFrame {
  Q_OBJECT
  Q_DISABLE_COPY(TransactionFrame)

public:
  TransactionFrame(const QModelIndex &_index, QWidget* _parent);
  ~TransactionFrame();

protected:
  void mousePressEvent(QMouseEvent* _event) Q_DECL_OVERRIDE;

private:
  QScopedPointer<Ui::TransactionFrame> m_ui;
  QDataWidgetMapper m_dataMapper;
  QPersistentModelIndex m_index;
};

}
