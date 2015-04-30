// Copyright (c) 2011-2015 The Cryptonote developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QFontDatabase>

#include "MainWindow.h"
#include "TransactionFrame.h"
#include "TransactionsModel.h"

#include "ui_transactionframe.h"

namespace WalletGui {

class RecentTransactionDelegate : public QStyledItemDelegate {
  Q_OBJECT

public:
  RecentTransactionDelegate(QObject* _parent) : QStyledItemDelegate(_parent) {

  }

  ~RecentTransactionDelegate() {
  }

  void setEditorData(QWidget* _editor, const QModelIndex& _index) const Q_DECL_OVERRIDE {
    switch(_index.column()) {
    case TransactionsModel::COLUMN_AMOUNT:
    case TransactionsModel::COLUMN_HASH:
    case TransactionsModel::COLUMN_DATE:
      static_cast<QLabel*>(_editor)->setText(_index.data().toString());
      return;
    case TransactionsModel::COLUMN_TYPE:
      static_cast<QLabel*>(_editor)->setPixmap(_index.data(TransactionsModel::ROLE_ICON).value<QPixmap>());
      return;
    default:
      return;
    }
  }
};

TransactionFrame::TransactionFrame(const QModelIndex& _index, QWidget* _parent) : QFrame(_parent),
  m_ui(new Ui::TransactionFrame), m_dataMapper(this), m_index(_index) {
  m_ui->setupUi(this);
  QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
  font.setPixelSize(11);
  m_ui->m_hashLabel->setFont(font);

  m_dataMapper.setModel(const_cast<QAbstractItemModel*>(m_index.model()));
  m_dataMapper.setItemDelegate(new RecentTransactionDelegate(this));
  m_dataMapper.addMapping(m_ui->m_iconLabel, TransactionsModel::COLUMN_TYPE);
  m_dataMapper.addMapping(m_ui->m_amountLabel, TransactionsModel::COLUMN_AMOUNT);
  m_dataMapper.addMapping(m_ui->m_timeLabel, TransactionsModel::COLUMN_DATE);
  m_dataMapper.addMapping(m_ui->m_hashLabel, TransactionsModel::COLUMN_HASH);
  m_dataMapper.setCurrentModelIndex(m_index);
}

TransactionFrame::~TransactionFrame() {
}

void TransactionFrame::mousePressEvent(QMouseEvent* _event) {
  MainWindow::instance().scrollToTransaction(TransactionsModel::instance().index(m_index.data(TransactionsModel::ROLE_ROW).toInt(), 0));
}

}

#include "TransactionFrame.moc"
