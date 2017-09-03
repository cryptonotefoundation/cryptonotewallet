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

#include <QFrame>

#include "INewsReader.h"
#include "Application/IWalletUiItem.h"

namespace Ui {
  class NewsFrame;
}

namespace WalletGui {

class NewsFrame : public QFrame, public INewsReaderObserver, public IWalletUiItem {
  Q_OBJECT
  Q_DISABLE_COPY(NewsFrame)

public:
  explicit NewsFrame(QWidget* _parent);
  ~NewsFrame();

  void setNewsReader(INewsReader* _reader);

  // QObject
  virtual bool eventFilter(QObject* _object, QEvent* _event) override;

  // INewsReaderObserver
  Q_SLOT virtual void newMessage() override;

protected:
  virtual void resizeEvent(QResizeEvent* _event) override;

private:
  QScopedPointer<Ui::NewsFrame> m_ui;
  INewsReader* m_reader;
  QString m_currentMessageId;

  Q_INVOKABLE void updateText();
  void updateSourceText();
  void updateScrollButtons();

  Q_SLOT void upClicked();
  Q_SLOT void downClicked();
  static QString getElidedText(const QString& _text, const QString& _timeString, const QSize& _size);

Q_SIGNALS:
  void linkActivatedSignal(const QString& _title);
};

}
