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

#include <QObject>

class QDir;
class QFile;

namespace WalletGui {

class WalletLogger :public QObject {
  Q_OBJECT
  Q_DISABLE_COPY(WalletLogger)

public:
  static void init(const QDir& _logDir, bool _debug, QObject* _parent);
  static void deinit();
  static void debug(const QString& _message);
  static void info(const QString& _message);
  static void warning(const QString& _message);
  static void critical(const QString& _message);

private:
  static QThread* m_loggerThread;
  static WalletLogger* m_instance;
  static QFile* m_logFile;

  WalletLogger(bool _debug, QObject* _parent);
  ~WalletLogger();

  static void messageHandler(QtMsgType _type, const QMessageLogContext &_context, const QString &_msg);

  Q_INVOKABLE void debugImpl(const QString& _message);
  Q_INVOKABLE void infoImpl(const QString& _message);
  Q_INVOKABLE void warningImpl(const QString& _message);
  Q_INVOKABLE void criticalImpl(const QString& _message);
};

}
