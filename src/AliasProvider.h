// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <QObject>
#include <QNetworkAccessManager>

namespace WalletGui {

class AliasProvider : public QObject {
  Q_OBJECT

public:
  AliasProvider(QObject *parent);
  ~AliasProvider();

  void getAddresses(const QString& _urlString);

private:
  QNetworkAccessManager m_networkManager;
  void readyRead();

Q_SIGNALS:
  void aliasFoundSignal(const QString& _name, const QString& _address);
};

}
