// Copyright (c) 2011-2016 The Cryptonote developers
// Copyright (c) 2015-2016 XDN developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QStringList>
#include <QUrl>

#include "AliasProvider.h"

namespace WalletGui {

Q_DECL_CONSTEXPR char ALIAS_OBJECT_NAME[] = "digitalname1";
Q_DECL_CONSTEXPR char ALIAS_NAME_TAG[] = "name";
Q_DECL_CONSTEXPR char ALIAS_ADDRESS_TAG[] = "xdn";

AliasProvider::AliasProvider(QObject *parent) : QObject(parent), m_networkManager() {
}

AliasProvider::~AliasProvider() {
}

void AliasProvider::getAddresses(const QString& _urlString) {
  QUrl url = QUrl::fromUserInput(_urlString);
  if (!url.isValid()) {
    return;
  }

  QNetworkRequest request(url);
  QNetworkReply* reply = m_networkManager.get(request);
  connect(reply, &QNetworkReply::readyRead, this, &AliasProvider::readyRead);
  connect(reply, &QNetworkReply::finished, reply, &QNetworkReply::deleteLater);
}

void AliasProvider::readyRead() {
  QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
  QByteArray data = reply->readAll();
  QJsonDocument doc = QJsonDocument::fromJson(data);
  if (doc.isNull()) {
    return;
  }

  QJsonArray array = doc.object().value(ALIAS_OBJECT_NAME).toArray();
  if (array.isEmpty()) {
    return;
  }

  QJsonObject obj = array.first().toObject();
  QString name = obj.value(ALIAS_NAME_TAG).toString();
  QString address = obj.value(ALIAS_ADDRESS_TAG).toString();

  if (!address.isEmpty()) {
    Q_EMIT aliasFoundSignal(name, address);
  }
}

}
