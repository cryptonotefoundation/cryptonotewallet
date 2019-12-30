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

#include <QFile>
#include <QFileSystemWatcher>
#include <QTextStream>
#include <QTimerEvent>

#include "LogFileWatcher.h"

namespace WalletGui {

LogFileWatcher::LogFileWatcher(const QString& _fileName, QObject* _parent) : QObject(_parent),
  m_logFile(new QFile(_fileName, this)) {
  if (m_logFile->open(QFile::ReadOnly | QFile::Text)) {
    m_logFile->seek(m_logFile->size());
    m_fileCheckTimer = startTimer(300);
  }
}

LogFileWatcher::~LogFileWatcher() {
}

void LogFileWatcher::timerEvent(QTimerEvent* _event) {
  if (_event->timerId() == m_fileCheckTimer) {
    if (!m_logFile->atEnd()) {
      fileChanged();
    }
  }

  QObject::timerEvent(_event);
}

void LogFileWatcher::fileChanged() {
  QTextStream stream(m_logFile);
  while(!stream.atEnd()) {
    QString line = stream.readLine();
    Q_EMIT newLogStringSignal(line);
  }
}

}
