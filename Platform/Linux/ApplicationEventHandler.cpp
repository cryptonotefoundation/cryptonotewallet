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

#include <QtDBus/QtDBus>
#include <QDBusConnection>
#include <QDBusError>
#include <QLocalSocket>

#include "ApplicationEventHandler.h"
#include "WalletLogger/WalletLogger.h"

namespace WalletGui {

namespace {

const char DBUS_GNOME_SS_SERVICE_NAME[] = "org.gnome.ScreenSaver";
const char DBUS_GNOME_SS_PATH[] = "/org/gnome/ScreenSaver";
const char DBUS_GNOME_SS_INTERFACE[] = "org.gnome.ScreenSaver";
const char DBUS_GNOME_SS_MEMBER[] = "ActiveChanged";

const char DBUS_FREEDESKTOP_SS_SERVICE_NAME[] = "org.freedesktop.ScreenSaver";
const char DBUS_FREEDESKTOP_SS_PATH[] = "/org/freedesktop/ScreenSaver";
const char DBUS_FREEDESKTOP_SS_INTERFACE[] = "org.freedesktop.ScreenSaver";
const char DBUS_FREEDESKTOP_SS_MEMBER[] = "ActiveChanged";

const char PAYMENT_SERVER_NAME[] = "chavezcoin-payment-server-YrmF1jZEv5ahqXuD99d5H1jXQ";

}

ApplicationEventHandler::ApplicationEventHandler(QObject* _parent) : QLocalServer(_parent) {
}

ApplicationEventHandler::~ApplicationEventHandler() {
}

void ApplicationEventHandler::init() {
  QDBusConnection::sessionBus().connect(DBUS_GNOME_SS_SERVICE_NAME, DBUS_GNOME_SS_PATH, DBUS_GNOME_SS_INTERFACE, DBUS_GNOME_SS_MEMBER,
    this, SLOT(screenStateChanged(bool)));
  QDBusConnection::sessionBus().connect(DBUS_FREEDESKTOP_SS_SERVICE_NAME, DBUS_FREEDESKTOP_SS_PATH, DBUS_FREEDESKTOP_SS_INTERFACE,
    DBUS_FREEDESKTOP_SS_MEMBER, this, SLOT(screenStateChanged(bool)));

  connect(this, &ApplicationEventHandler::newConnection, this, &ApplicationEventHandler::accept);
  removeServer(PAYMENT_SERVER_NAME);
  if (!listen(PAYMENT_SERVER_NAME)) {
    WalletLogger::critical(tr("[Application event handler] Start payment server error: %1.").arg(errorString()));
  }
  WalletLogger::info(tr("[ApplicationEventHandler] 1"));
}

QUrl ApplicationEventHandler::getLastReceivedUrl() {
  return m_receivedUrl;
}

void ApplicationEventHandler::sendUrl(const QUrl& _url) {
  m_receivedUrl = _url;
  QLocalSocket* socket = new QLocalSocket(this);
  socket->connectToServer(PAYMENT_SERVER_NAME);
  if (!socket->waitForConnected(5000)) {
    socket->deleteLater();
    return;
  }

  QByteArray data = _url.toString().toUtf8();
  socket->waitForBytesWritten(100);
  socket->deleteLater();
}

void ApplicationEventHandler::addObserver(IApplicationEventHandlerObserver* _observer) {
  QObject* observer = dynamic_cast<QObject*>(_observer);
  connect(this, SIGNAL(urlReceivedSignal(QUrl)), observer, SLOT(urlReceived(QUrl)));
  connect(this, SIGNAL(screenLockedSignal()), observer, SLOT(screenLocked()));
  connect(this, SIGNAL(screenUnlockedSignal()), observer, SLOT(screenUnlocked()));
}

void ApplicationEventHandler::removeObserver(IApplicationEventHandlerObserver* _observer) {
  QObject* observer = dynamic_cast<QObject*>(_observer);
  disconnect(this, SIGNAL(urlReceivedSignal(QUrl)), observer, SLOT(urlReceived(QUrl)));
  disconnect(this, SIGNAL(screenLockedSignal()), observer, SLOT(screenLocked()));
  disconnect(this, SIGNAL(screenUnlockedSignal()), observer, SLOT(screenUnlocked()));
}

void ApplicationEventHandler::accept() {
  WalletLogger::debug(tr("[Application event handler] New socket connection."));
  QLocalSocket* socket = nextPendingConnection();
  if (socket == nullptr) {
    return;
  }

  connect(socket, &QLocalSocket::readyRead, this, &ApplicationEventHandler::readyRead);
  connect(socket, &QLocalSocket::disconnected, socket, &QLocalSocket::deleteLater);
}

void ApplicationEventHandler::readyRead() {
  QByteArray rawData = qobject_cast<QLocalSocket*>(sender())->readAll();
  WalletLogger::debug(tr("[Application event handler] Received data: %1.").arg(QString::fromUtf8(rawData)));
  QUrl paymentUrl = QUrl::fromUserInput(rawData);
  if (paymentUrl.isValid()) {
    WalletLogger::info(tr("[Application event handler] Received URI: %1.").arg(paymentUrl.toString()));
    Q_EMIT urlReceivedSignal(paymentUrl);
  }
}

void ApplicationEventHandler::screenStateChanged(bool _isLocked) {
  if (_isLocked) {
    Q_EMIT screenLockedSignal();
  } else {
    Q_EMIT screenUnlockedSignal();
  }
}

}
