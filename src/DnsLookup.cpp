// Copyright (c) 2016-2017 The Karbovanets developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QDnsLookup>
#include <QHostAddress>
#include <QUrl>
#include <QDebug>

#include "DnsLookup.h"

namespace WalletGui {

DnsManager::DnsManager(QObject *parent) : QObject(parent), m_dns() {
}

DnsManager::~DnsManager() {
}

void DnsManager::getAddresses(const QString& _urlString) {
  m_dns = new QDnsLookup(this);
  m_dns->setType(QDnsLookup::TXT);

  QString urlString = QUrl::fromUserInput(_urlString).host();

  if(urlString.contains('.')) {
    m_dns->setName(urlString);
    m_dns->lookup();
    connect(m_dns, SIGNAL(finished()), this, SLOT(handleTxtRecords()));
  }
}

void DnsManager::handleTxtRecords(){
  if (m_dns->error() != QDnsLookup::NoError){
    qDebug() << m_dns->error() << m_dns->errorString();
    m_dns->deleteLater();
    return;
  }
  foreach (const QDnsTextRecord &record, m_dns->textRecords()) {
    foreach (const QByteArray &ba, record.values()) {
      QString txt = QString::fromUtf8(ba);
      if(txt.contains("oa1:krb")) {
        int s1 = txt.indexOf("recipient_address=",0,Qt::CaseInsensitive);
        int s2 = txt.indexOf("recipient_name=",0,Qt::CaseInsensitive);
        QString address = txt.mid(s1+18,95);
        QString name = txt.mid(s2+15).split(";")[0].toUtf8();
        if (!address.isEmpty()) {
          Q_EMIT aliasFoundSignal(name, address);
        }
      }
    }
  }
  m_dns->deleteLater();
}

}
