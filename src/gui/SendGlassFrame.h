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

#pragma once

#include "GlassFrame.h"

namespace WalletGui {

class SendGlassFrame : public GlassFrame {
  Q_OBJECT
  Q_DISABLE_COPY(SendGlassFrame)

public:
  explicit SendGlassFrame(QWidget* _parent);
  ~SendGlassFrame();

  void updateSynchronizationState(quint32 _current, quint32 _total);

protected:
  void paintEvent(QPaintEvent* _event) override;

private:
  quint64 m_currentHeight;
  quint64 m_totalHeight;
  QPixmap m_pixmapBuffer;
  QString m_lastThemeName;

  void paintInBuffer();
  void drawProgressGraph(QPainter &_painter);
  void drawProgressLabel(QPainter &_painter);
  void drawProgressValue(QPainter &_painter);
};

}
