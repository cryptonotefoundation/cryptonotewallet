// Copyright (c) 2011-2015 The Cryptonote developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <QWidget>

class QMovie;

namespace Ui {
class ExitWidget;
}

namespace WalletGui {

class ExitWidget : public QWidget {
  Q_OBJECT
  Q_DISABLE_COPY(ExitWidget)

public:
  ExitWidget(QWidget* _parent);
  ~ExitWidget();

private:
  QScopedPointer<Ui::ExitWidget> m_ui;
  QMovie* m_clockMovie;
};

}
