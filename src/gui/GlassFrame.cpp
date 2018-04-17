// Copyright (c) 2015-2017, The Bytecoin developers
//
// This file is part of Bytecoin.
//
// Karbovanets is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Karbovanets is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Karbovanets.  If not, see <http://www.gnu.org/licenses/>.

#include <QEvent>

#include "GlassFrame.h"

namespace WalletGui {

GlassFrame::GlassFrame(QWidget* _parent) : QFrame(_parent) {
  d_ptr->q_ptr = this;
}

GlassFrame::~GlassFrame() {
}

void GlassFrame::install(QWidget* _parent) {
  if (parent() == _parent) {
    return;
  }

  remove();
  setParent(_parent);
  _parent->installEventFilter(this);
  show();
  QEvent event(QEvent::Resize);
  eventFilter(0, &event);
}

void GlassFrame::remove() {
  if (parentWidget() != nullptr) {
    parentWidget()->removeEventFilter(this);
    hide();
    setParent(nullptr);
  }
}

bool GlassFrame::eventFilter(QObject* _object, QEvent* _event) {
  if (_event->type() == QEvent::Show || _event->type() == QEvent::Paint || _event->type() == QEvent::Resize) {
    resize(parentWidget()->size());
    move(0, 0);
    return true;
  }

  if (qobject_cast<QWidget*>(parent())->isVisible()) {
    setFocus();
  }

  return false;
}

}
