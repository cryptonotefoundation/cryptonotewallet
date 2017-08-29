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

#include <QCoreApplication>
#include <QFileOpenEvent>

#include "ApplicationEventHandler.h"
#include "NotificationMonitor.h"

namespace WalletGui {

const char SCREEN_LOCKED_NOTIFICATION_NAME[] = "com.apple.screenIsLocked";
const char SCREEN_UNLOCKED_NOTIFICATION_NAME[] = "com.apple.screenIsUnlocked";

ApplicationEventHandler::ApplicationEventHandler(QObject* _parent) : QObject(_parent) {
  QCoreApplication::instance()->installEventFilter(this);
  m_notificationMonitor = [[NotificationMonitor alloc] init: this];
}

ApplicationEventHandler::~ApplicationEventHandler() {
  [static_cast<id>(m_notificationMonitor) dealloc];
}

void ApplicationEventHandler::init() {
  // Do nothing
}

QUrl ApplicationEventHandler::getLastReceivedUrl() {
  return m_receivedUrl;
}

void ApplicationEventHandler::sendUrl(const QUrl& _url) {
  Q_UNUSED(_url)
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

void ApplicationEventHandler::notify(const QString& _notification) {
  if (_notification == SCREEN_LOCKED_NOTIFICATION_NAME) {
    Q_EMIT screenLockedSignal();
  } else if (_notification == SCREEN_UNLOCKED_NOTIFICATION_NAME) {
    Q_EMIT screenUnlockedSignal();
  }
}

bool WalletGui::ApplicationEventHandler::eventFilter(QObject* _object, QEvent* _event) {
  if (_object == QCoreApplication::instance() && _event->type() == QEvent::FileOpen) {
    QFileOpenEvent* fileOpenEvent = static_cast<QFileOpenEvent*>(_event);
    if (fileOpenEvent->url().isValid() && !fileOpenEvent->url().isEmpty()) {
      m_receivedUrl = fileOpenEvent->url();
      Q_EMIT urlReceivedSignal(fileOpenEvent->url());
    }

    return false;
  }

  return QObject::eventFilter(_object, _event);
}

}
