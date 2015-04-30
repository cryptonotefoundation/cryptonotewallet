// Copyright (c) 2011-2015 The Cryptonote developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <QLabel>
#include <QTimer>

namespace WalletGui {

class AnimatedLabel : public QLabel {
  Q_OBJECT
  Q_DISABLE_COPY(AnimatedLabel)

public:
  AnimatedLabel(QWidget* _parent);
  ~AnimatedLabel();

  void setSprite(const QPixmap& _sprite_pixmap, const QSize& _frame_size, quint32 _vertical_space, quint32 _frequency);
  void startAnimation();
  void stopAnimation();

private:
  QPixmap m_spritePixmap;
  QSize m_spriteFrameSize;
  quint32 m_spriteVerticalSpace;
  QTimer m_animationTimer;
  QRect m_frameRect;

  void timeout();
};

}
