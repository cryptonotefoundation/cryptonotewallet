// Copyright (c) 2016-2017 The Karbovanets developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <QDnsLookup>
#include <QHostAddress>
#include <QStringList>

namespace WalletGui {

class DnsManager : public QObject {
  Q_OBJECT

public:
  DnsManager(QObject *parent);
  ~DnsManager();

  void getAddresses(const QString& _urlString);

public slots:
  void handleTxtRecords();

private:
  QDnsLookup *m_dns;
  QStringList Records;

Q_SIGNALS:
  void aliasFoundSignal(const QString& _name, const QString& _address);

};

}

