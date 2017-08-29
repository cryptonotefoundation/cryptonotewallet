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

#include <QObject>

namespace WalletGui {

class IPoolClientObserver {
public:
  virtual ~IPoolClientObserver() {}
  virtual void started() = 0;
  virtual void stopped() = 0;
  virtual void socketError() = 0;
  virtual void difficultyChanged(quint32 _difficulty) = 0;
  virtual void goodShareCountChanged(quint32 _goodShareCount) = 0;
  virtual void badShareCountChanged(quint32 _badShareCount) = 0;
  virtual void connectionErrorCountChanged(quint32 _connectionErrorCount) = 0;
  virtual void lastConnectionErrorTimeChanged(const QDateTime& _connectionErrorTime) = 0;
};

class IPoolClient {
public:
  virtual ~IPoolClient() {}

  virtual void start() = 0;
  virtual void stop() = 0;

  virtual QString getLogin() const = 0;
  virtual QString getPoolHost() const = 0;
  virtual quint16 getPoolPort() const = 0;
  virtual quint32 getDifficulty() const = 0;
  virtual quint32 getGoodShareCount() const = 0;
  virtual quint32 getBadShareCount() const = 0;
  virtual quint32 getConnectionErrorCount() const = 0;
  virtual QDateTime getLastConnectionErrorTime() const = 0;

  virtual void addObserver(IPoolClientObserver* _observer) = 0;
  virtual void removeObserver(IPoolClientObserver* _observer) = 0;
};

}
