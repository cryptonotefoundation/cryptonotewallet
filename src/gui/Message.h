// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <QDateTime>
#include <QList>
#include <QPair>

namespace WalletGui {

typedef QList<QPair<QString, QString> > MessageHeader;

class Message {
public:
  Message();
  Message(const Message& _message);
  Message(const Message&& _message);
  Message(const QString& _message);
  ~Message();

  QString getMessage() const;
  QString getFullMessage() const;
  QString getHeaderValue(const QString& _key) const;

  Message& operator=(const Message& _message);

  static QString makeTextMessage(const QString& _message, const MessageHeader& _header);

private:
  QString m_message;
  MessageHeader m_header;
};

}
