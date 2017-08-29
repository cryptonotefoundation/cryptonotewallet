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

class QUrl;

namespace WalletGui {

class IApplicationEventHandlerObserver {
public:
  virtual ~IApplicationEventHandlerObserver() {}
  virtual void urlReceived(const QUrl& _url) = 0;
  virtual void screenLocked() = 0;
  virtual void screenUnlocked() = 0;
};

class IApplicationEventHandler {
public:
  virtual ~IApplicationEventHandler() {}

  virtual void init() = 0;
  virtual QUrl getLastReceivedUrl() = 0;
  virtual void sendUrl(const QUrl& _url) = 0;
  virtual void addObserver(IApplicationEventHandlerObserver* _observer) = 0;
  virtual void removeObserver(IApplicationEventHandlerObserver* _observer) = 0;
};

}
