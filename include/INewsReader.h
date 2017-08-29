// Copyright (c) 2015-2017, The Bytecoin developers
//
// This file is part of Bytecoin.
//
// Chavezcoin is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Chavezcoin is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Chavezcoin.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <QDateTime>

class QPixmap;

namespace WalletGui {

class INewsReaderObserver {
public:
  virtual ~INewsReaderObserver() {}
  virtual void newMessage() = 0;
};

class INewsReader {
public:
  virtual ~INewsReader() {}
  virtual QString getName() const = 0;
  virtual QPixmap getIcon() const = 0;
  virtual QString getLastMessageId() const = 0;

  virtual QString getNextMessageId(const QString& _currentMessageId) const = 0;
  virtual QString getPrevMessageId(const QString& _currentMessageId) const = 0;
  virtual QString getMessageHeader(const QString& _messageId) const = 0;
  virtual QString getMessageSourceUrl(const QString& _messageId) const = 0;
  virtual QString getMessageText(const QString& _messageId) const = 0;
  virtual QDateTime getMessageTime(const QString& _messageId) const = 0;

  virtual void init() = 0;
  virtual void deinit() = 0;
  virtual void addObserver(INewsReaderObserver* _observer) = 0;
  virtual void removeObserver(INewsReaderObserver* _observer) = 0;
};

}
