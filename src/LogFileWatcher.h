// Copyright (c) 2015-2017, The Bytecoin developers
// Copyright (c) 2017-2018, The Karbo developers
//
// This file is part of Karbo.
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

#include <QObject>

class QFile;
class QFileSystemWatcher;

namespace WalletGui {

class LogFileWatcher : public QObject {
  Q_OBJECT
  Q_DISABLE_COPY(LogFileWatcher)

public:
  LogFileWatcher(const QString& _filePath, QObject* _parent);
  ~LogFileWatcher();

protected:
  void timerEvent(QTimerEvent* _event) override;

private:
  int m_fileCheckTimer;
  QFile* m_logFile;

  void fileChanged();

Q_SIGNALS:
  void newLogStringSignal(const QString& _string);
};

}
