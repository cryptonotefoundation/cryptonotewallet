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

#include "INewsReader.h"

class QNetworkAccessManager;
class QNetworkReply;

namespace WalletGui {

class BlogReader : public QObject, public INewsReader {
  Q_OBJECT
  Q_DISABLE_COPY(BlogReader)

public:
  explicit BlogReader(QObject* _parent);
  ~BlogReader();

  // INewsReader
  virtual QString getName() const override;
  virtual QPixmap getIcon() const override;
  virtual QString getLastMessageId() const override;
  virtual QString getNextMessageId(const QString& _currentMessageId) const override;
  virtual QString getPrevMessageId(const QString& _currentMessageId) const override;
  virtual QString getMessageHeader(const QString& _messageId) const override;
  virtual QString getMessageSourceUrl(const QString& _messageId) const override;
  virtual QString getMessageText(const QString& _messageId) const override;
  virtual QDateTime getMessageTime(const QString& _messageId) const override;
  virtual void init() override;
  virtual void deinit() override;
  virtual void addObserver(INewsReaderObserver* _observer) override;
  virtual void removeObserver(INewsReaderObserver* _observer) override;

protected:
  virtual void timerEvent(QTimerEvent* _event) override;

private:
  struct MessageItem {
    QString messageId;
    QString messageSourceUrl;
    QString messageTitle;
    QString messageText;
    QDateTime messageTime;
  };

  QNetworkAccessManager* m_networkManager;
  QNetworkReply* m_activeReply;
  QByteArray m_dataBuffer;
  QList<MessageItem> m_messages;
  QHash<QString, int> m_messageIdIndex;
  int m_updateTimerId;

  void getBlogRequest();
  void blogReplyError(int _errorCode);
  void blogReplyFinished();
  void blogReplyReadyRead();
  void processBlogReplyData(const QString& _data);

  static QByteArray readFromReply(QNetworkReply* _reply);

Q_SIGNALS:
  void newMessageSignal();
};

}
