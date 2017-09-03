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

#include <QDateTime>

#include "Gui/Common/WalletTreeView.h"

namespace WalletGui {

class PoolTreeView : public WalletTreeView {
  Q_OBJECT
  Q_DISABLE_COPY(PoolTreeView)

public:
  explicit PoolTreeView(QWidget* _parent);
  ~PoolTreeView();

  bool eventFilter(QObject* _object, QEvent* _event) override;

protected:
  void dragEnterEvent(QDragEnterEvent* _event) override;
  void dragMoveEvent(QDragMoveEvent* _event) override;
  void dropEvent(QDropEvent* _event) override;
  void mouseMoveEvent(QMouseEvent* _event) override;
  void mousePressEvent(QMouseEvent* _event) override;
  void mouseReleaseEvent(QMouseEvent* _event) override;

private:
  QDateTime m_pressedTime;
  QPoint m_pressedPosition;
  QModelIndex m_pressedIndex;
};

}
