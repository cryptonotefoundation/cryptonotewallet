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

#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QLoggingCategory>
#include <QTextStream>
#include <QThread>

#include "WalletLogger.h"

Q_DECLARE_LOGGING_CATEGORY(infoLogging)
Q_DECLARE_LOGGING_CATEGORY(Wallet)
Q_LOGGING_CATEGORY(Wallet, "Wallet")
Q_LOGGING_CATEGORY(infoLogging, "logger.info")

namespace WalletGui {

namespace {

  const char LOG_FILE_NAME[] = "walletgui.log";

}

QThread* WalletLogger::m_loggerThread = nullptr;

WalletLogger* WalletLogger::m_instance = nullptr;

QFile* WalletLogger::m_logFile = nullptr;

void WalletLogger::init(const QDir& _logDir, bool _debug, QObject* _parent) {
  Q_ASSERT(m_instance == nullptr);
  m_loggerThread = new QThread(_parent);
  m_instance = new WalletLogger(_debug, nullptr);
  m_logFile = new QFile(nullptr);

  m_logFile->setFileName(_logDir.absoluteFilePath(LOG_FILE_NAME));
  if (!m_logFile->open(QFile::WriteOnly | QFile::Append | QFile::Text)) {
    fprintf(stderr, "[Logger] Can't open log file\n");
  }

  m_instance->moveToThread(m_loggerThread);
  m_logFile->moveToThread(m_loggerThread);
  m_loggerThread->start();
}

void WalletLogger::deinit() {
  m_loggerThread->quit();
  m_loggerThread->wait();
}

void WalletLogger::debug(const QString& _message) {
  Q_ASSERT(m_instance != nullptr);
  QMetaObject::invokeMethod(m_instance, "debugImpl", Qt::QueuedConnection, Q_ARG(QString, _message));
}

void WalletLogger::info(const QString& _message) {
  Q_ASSERT(m_instance != nullptr);
  QMetaObject::invokeMethod(m_instance, "infoImpl", Qt::QueuedConnection, Q_ARG(QString, _message));
}

void WalletLogger::warning(const QString& _message) {
  Q_ASSERT(m_instance != nullptr);
  QMetaObject::invokeMethod(m_instance, "warningImpl", Qt::QueuedConnection, Q_ARG(QString, _message));
}

void WalletLogger::critical(const QString& _message) {
  Q_ASSERT(m_instance != nullptr);
  QMetaObject::invokeMethod(m_instance, "criticalImpl", Qt::QueuedConnection, Q_ARG(QString, _message));
}

WalletLogger::WalletLogger(bool _debug, QObject* _parent) : QObject(_parent) {
  qInstallMessageHandler(&WalletLogger::messageHandler);
  if (_debug) {
    QLoggingCategory::setFilterRules("qt.network.ssl.warning=false\nWallet.debug=true");
  } else {
    QLoggingCategory::setFilterRules("qt.network.ssl.warning=false\nWallet.debug=false");
  }
}

WalletLogger::~WalletLogger() {
}

void WalletLogger::messageHandler(QtMsgType _type, const QMessageLogContext &_context, const QString &_msg) {
  if (!QLoggingCategory(_context.category).isEnabled(_type)) {
    return;
  }

  QString timeString = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
  QString typeString;
  switch (_type) {
  case QtDebugMsg:
    typeString = "debug";
    break;
#if QT_VERSION >= 0x050500
  case QtInfoMsg:
    typeString = "info";
    break;
#endif
  case QtWarningMsg:
#if QT_VERSION < 0x050500
    if (!QString(_context.category).compare("logger.info")) {
      typeString = "info";
    } else {
      typeString = "warning";
    }
#else
    typeString = "warning";
#endif
    break;
  case QtCriticalMsg:
    typeString = "critical";
    break;
  case QtFatalMsg:
    fprintf(stderr, "%s [fatal] %s\n", qPrintable(timeString), qPrintable(_msg));
    abort();
  }

  QString logString = QString("%1 [%2] %3").arg(timeString).arg(typeString).arg(_msg);
  if (m_logFile->isOpen()) {
    QTextStream logStream(m_logFile);
    logStream << logString << endl;
  }

  if (QLoggingCategory::defaultCategory()->isEnabled(QtDebugMsg)) {
    fprintf(stderr, "%s\n", qPrintable(logString));
    fflush(stderr);
  }
}

void WalletLogger::debugImpl(const QString& _message) {
  qCDebug(Wallet) << qPrintable(_message);
}

void WalletLogger::infoImpl(const QString& _message) {
#if QT_VERSION < 0x050500
  qCWarning(infoLogging) << qPrintable(_message);
#else
  qCInfo(Wallet, "%s", qPrintable(_message));
#endif
}

void WalletLogger::warningImpl(const QString& _message) {
  qCWarning(Wallet) << qPrintable(_message);
}

void WalletLogger::criticalImpl(const QString& _message) {
  qCCritical(Wallet) << qPrintable(_message);
}

}
