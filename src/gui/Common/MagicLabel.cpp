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

#include <QTimer>

#include "MagicLabel.h"

namespace WalletGui {

MagicLabel::MagicLabel(QWidget* _parent) : MagicLabel(500, 1000, _parent) {
}

MagicLabel::MagicLabel(quint32 _showDuration, quint32 _hideDuration, QWidget* _parent) : QLabel(_parent), m_effect(this),
  m_showAnimation(), m_hideAnimation() {
  setGraphicsEffect(&m_effect);

  m_showAnimation.setTargetObject(&m_effect);
  m_showAnimation.setPropertyName("opacity");
  m_showAnimation.setDuration(_showDuration);
  m_showAnimation.setStartValue(0);
  m_showAnimation.setEndValue(1);
  m_showAnimation.setEasingCurve(QEasingCurve::OutQuad);

  m_hideAnimation.setTargetObject(&m_effect);
  m_hideAnimation.setPropertyName("opacity");
  m_hideAnimation.setDuration(_hideDuration);
  m_hideAnimation.setStartValue(1);
  m_hideAnimation.setEndValue(0);
  m_hideAnimation.setEasingCurve(QEasingCurve::OutQuad);

  connect(&m_showAnimation, &QPropertyAnimation::finished, [this]() {
      QTimer::singleShot(500, &this->m_hideAnimation, SLOT(start()));
    });

  connect(&m_hideAnimation, &QPropertyAnimation::finished, this, &MagicLabel::completedSignal);
  m_effect.setOpacity(0);
}

MagicLabel::~MagicLabel() {
}

void MagicLabel::start() {
  m_effect.setOpacity(0);
  m_showAnimation.start();
}

}
