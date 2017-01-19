// Copyright (c) 2011-2015 The Cryptonote developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <QFrame>
#include <QMenu>
#include <QStyledItemDelegate>

namespace Ui {
class TransactionsFrame;
}

namespace WalletGui {

class TransactionsListModel;

class TransactionsFrame : public QFrame {
  Q_OBJECT
  Q_DISABLE_COPY(TransactionsFrame)

public:
  TransactionsFrame(QWidget* _parent);
  ~TransactionsFrame();
  QModelIndex index;

  void scrollToTransaction(const QModelIndex& _index);

public slots:
    void onCustomContextMenu(const QPoint &point);

public Q_SLOTS:
    void copyTxHash();
    void copyAmount();
    void copyPaymentID();
    void showTxDetails();
    void computeSelected();
    void showTransactionDetails(const QModelIndex& _index);

private:
  QScopedPointer<Ui::TransactionsFrame> m_ui;
  QScopedPointer<TransactionsListModel> m_transactionsModel;
  QMenu* contextMenu;

  QString formatAmount(int64_t _amount) const;

  Q_SLOT void exportToCsv();


};

}
