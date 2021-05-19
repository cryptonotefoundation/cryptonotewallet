// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Copyright (c) 2016-2021 Karbo developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <QDataWidgetMapper>
#include <QDialog>

namespace Ui {
class OutputDetailsDialog;
}

namespace WalletGui {

class OutputDetailsDialog : public QDialog {
  Q_OBJECT

public:
  OutputDetailsDialog(const QModelIndex& _index, QWidget* _parent);
  ~OutputDetailsDialog();

  QModelIndex getCurrentOutputIndex() const;

private:
  QScopedPointer<Ui::OutputDetailsDialog> m_ui;
  QDataWidgetMapper m_dataMapper;

  Q_SLOT void prevClicked();
  Q_SLOT void nextClicked();
};

}
