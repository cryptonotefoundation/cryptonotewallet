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

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPixmap>
#include <QTextDocument>
#include <QTimerEvent>
#include <QUrl>
#include <QXmlStreamReader>

#include "BlogReader.h"
#include "Settings/Settings.h"
#include "Style/Style.h"
#include "WalletLogger/WalletLogger.h"

namespace WalletGui {

namespace {

const char BLOG_RSS_SCHEME[] = "https";
const char BLOG_RSS_HOST[] = "intensecoin.com";
const char BLOG_RSS_PATH[] = "/feed/atom/";

const char BLOG_RSS_ID_TAG_NAME[] = "id";
const char BLOG_RSS_LINK_TAG_NAME[] = "link";
const char BLOG_RSS_LINK_HREF_ATTRIBUTE_NAME[] = "href";
const char BLOG_RSS_TITLE_TAG_NAME[] = "title";
const char BLOG_RSS_CONTENT_TAG_NAME[] = "content";
const char BLOG_RSS_UPDATED_TAG_NAME[] = "updated";

const int MAX_MESSAGE_COUNT = 5;

const int UPDATE_TIMER_INTERVAL = 10 * 60 * 1000; // 10 minutes

}

BlogReader::BlogReader(QObject* _parent) : QObject(_parent), m_networkManager(nullptr), m_activeReply(nullptr),
  m_updateTimerId(-1) {
}

BlogReader::~BlogReader() {
}

QString BlogReader::getName() const {
  return tr("Intense Coin Blog:");
}

QPixmap BlogReader::getIcon() const {
  return QPixmap(":icons/blog");
}

QString BlogReader::getLastMessageId() const {
  if (m_messages.isEmpty()) {
    return QString();
  }

  return m_messages.last().messageId;
}

QString BlogReader::getNextMessageId(const QString& _currentMessageId) const {
  Q_ASSERT(m_messageIdIndex.contains(_currentMessageId));
  int currentMessageIndex = m_messageIdIndex.value(_currentMessageId);
  Q_ASSERT(currentMessageIndex < m_messages.size());
  if (currentMessageIndex == m_messages.size() - 1) {
    return QString();
  }

  return m_messages.at(currentMessageIndex + 1).messageId;
}

QString BlogReader::getPrevMessageId(const QString& _currentMessageId) const {
  Q_ASSERT(m_messageIdIndex.contains(_currentMessageId));
  int currentMessageIndex = m_messageIdIndex.value(_currentMessageId);
  Q_ASSERT(currentMessageIndex < m_messages.size());
  if (currentMessageIndex == 0) {
    return QString();
  }

  return m_messages.at(currentMessageIndex - 1).messageId;
}

QString BlogReader::getMessageHeader(const QString& _messageId) const {
  Q_ASSERT(m_messageIdIndex.contains(_messageId));
  int messageIndex = m_messageIdIndex.value(_messageId);
  Q_ASSERT(messageIndex < m_messages.size());
  return m_messages.at(messageIndex).messageTitle;
}

QString BlogReader::getMessageSourceUrl(const QString& _messageId) const {
  Q_ASSERT(m_messageIdIndex.contains(_messageId));
  int messageIndex = m_messageIdIndex.value(_messageId);
  Q_ASSERT(messageIndex < m_messages.size());
  return m_messages.at(messageIndex).messageSourceUrl;
}

QString BlogReader::getMessageText(const QString& _messageId) const {
  Q_ASSERT(m_messageIdIndex.contains(_messageId));
  int messageIndex = m_messageIdIndex.value(_messageId);
  Q_ASSERT(messageIndex < m_messages.size());
  return m_messages.at(messageIndex).messageText;
}

QDateTime BlogReader::getMessageTime(const QString& _messageId) const {
  Q_ASSERT(m_messageIdIndex.contains(_messageId));
  int messageIndex = m_messageIdIndex.value(_messageId);
  Q_ASSERT(messageIndex < m_messages.size());
  return m_messages.at(messageIndex).messageTime;
}

void BlogReader::init() {
  if (m_networkManager == nullptr) {
    WalletLogger::info(tr("[Blog reader] Initializing"));
    m_networkManager = new QNetworkAccessManager(this);
    getBlogRequest();
    m_updateTimerId = startTimer(UPDATE_TIMER_INTERVAL);
  }
}

void BlogReader::deinit() {
  if (m_networkManager == nullptr) {
    return;
  }

  WalletLogger::info(tr("[Blog reader] Deinitializing"));
  if (m_activeReply != nullptr) {
    m_activeReply->abort();
    m_activeReply->deleteLater();
    m_activeReply = nullptr;
  }

  if (m_updateTimerId != -1) {
    killTimer(m_updateTimerId);
    m_updateTimerId = -1;
  }

  m_networkManager->deleteLater();
  m_networkManager = nullptr;
}

void BlogReader::addObserver(INewsReaderObserver* _observer) {
  QObject* observerObject = dynamic_cast<QObject*>(_observer);
  Q_ASSERT(observerObject != nullptr);
  connect(this, SIGNAL(newMessageSignal()), observerObject, SLOT(newMessage()));
}

void BlogReader::removeObserver(INewsReaderObserver* _observer) {
  QObject* observerObject = dynamic_cast<QObject*>(_observer);
  Q_ASSERT(observerObject != nullptr);
  disconnect(this, SIGNAL(newMessageSignal()), observerObject, SLOT(newMessage()));
}

void BlogReader::timerEvent(QTimerEvent* _event) {
  if (_event->timerId() == m_updateTimerId) {
    getBlogRequest();
    return;
  }

  QObject::timerEvent(_event);
}

void BlogReader::getBlogRequest() {
  QUrl blogUrl;
  blogUrl.setScheme(BLOG_RSS_SCHEME);
  blogUrl.setHost(BLOG_RSS_HOST);
  blogUrl.setPath(BLOG_RSS_PATH);
  QNetworkRequest blogRequest(blogUrl);

  m_activeReply = m_networkManager->get(blogRequest);
  connect(m_activeReply, static_cast<void(QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error), this, &BlogReader::blogReplyError);
  connect(m_activeReply, &QNetworkReply::finished, this, &BlogReader::blogReplyFinished);
  connect(m_activeReply, &QNetworkReply::readyRead, this, &BlogReader::blogReplyReadyRead);
}

void BlogReader::blogReplyError(int _errorCode) {
  WalletLogger::critical(tr("[Blog reader] Get blog error: %1").arg(m_activeReply->errorString()));
  m_dataBuffer.clear();
}

void BlogReader::blogReplyFinished() {
  WalletLogger::debug(tr("[Blog reader] Blog request finished"));
  m_activeReply->deleteLater();
  m_activeReply = nullptr;
  if (!m_dataBuffer.isEmpty()) {
    processBlogReplyData(m_dataBuffer);
    m_dataBuffer.clear();
  }
}

void BlogReader::blogReplyReadyRead() {
  WalletLogger::debug(tr("[Blog reader] Blog data ready to read"));
  m_dataBuffer.append(readFromReply(m_activeReply));
}

void BlogReader::processBlogReplyData(const QString& _data) {
  QXmlStreamReader xml(_data);
  QList<MessageItem> newMessages;
  while (!xml.atEnd() && newMessages.size() < MAX_MESSAGE_COUNT) {
    xml.readNext();
    if (xml.isStartElement() && !xml.name().compare(QLatin1String("entry"))) {
      MessageItem messageItem;
	  bool isLinkSet = false;
      while (!xml.atEnd() && !(xml.isEndElement() && !xml.name().compare(QLatin1String("entry")))) {
        xml.readNext();
        if (xml.isStartElement()) {
          if (!xml.name().compare(QLatin1String(BLOG_RSS_ID_TAG_NAME))) {
            messageItem.messageId = xml.readElementText();
          } else if(!xml.name().compare(QLatin1String(BLOG_RSS_LINK_TAG_NAME)) && !isLinkSet) {
            messageItem.messageSourceUrl = xml.attributes().value(BLOG_RSS_LINK_HREF_ATTRIBUTE_NAME).toString();
			isLinkSet = true;
          } else if(!xml.name().compare(QLatin1String(BLOG_RSS_TITLE_TAG_NAME))) {
            messageItem.messageTitle = xml.readElementText();
          } else if(!xml.name().compare(QLatin1String(BLOG_RSS_CONTENT_TAG_NAME))) {
            QString htmlText = xml.readElementText();
            QTextDocument textDoc;
            textDoc.setHtml(htmlText);
            messageItem.messageText = textDoc.toPlainText();
          } else if(!xml.name().compare(QLatin1String(BLOG_RSS_UPDATED_TAG_NAME))) {
            QString timeString = xml.readElementText();
            messageItem.messageTime = QDateTime::fromString(timeString, Qt::ISODate);
          }
        }
      }

      newMessages.append(messageItem);
    }
  }

  if (xml.hasError()) {
    WalletLogger::critical(tr("[Blog reader] Parse XML error: %1").arg(xml.errorString()));
  } else {
    if (newMessages.isEmpty() || (!m_messages.isEmpty() && (newMessages.last().messageId == m_messages.last().messageId ||
      newMessages.last().messageTime <= m_messages.last().messageTime))) {
      return;
    }

    for (int i = newMessages.size() - 1; i >= 0; --i) {
      const MessageItem& messageItem = newMessages[i];
      if (m_messages.isEmpty()) {
        m_messages.append(messageItem);
        m_messageIdIndex.insert(messageItem.messageId, m_messages.size() - 1);
        continue;
      }

      if (messageItem.messageTime > m_messages.last().messageTime) {
        m_messages.append(messageItem);
        if (m_messages.size() > MAX_MESSAGE_COUNT) {
          m_messageIdIndex.remove(m_messages.first().messageId);
          m_messages.removeFirst();
          for (int i = 0; i < m_messages.size(); ++i) {
            m_messageIdIndex.insert(m_messages.at(i).messageId, i);
          }
        } else {
          m_messageIdIndex.insert(m_messages.last().messageId, m_messages.size() - 1);
        }
      }
    }

    Q_EMIT newMessageSignal();
  }
}

QByteArray BlogReader::readFromReply(QNetworkReply* _reply) {
  return _reply->readAll();
}

}
