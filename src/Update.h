// Copyright (c) 2016-2020 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef UPDATE_H
#define UPDATE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

const static QString KARBO_UPDATE_URL   = "https://api.github.com/repos/Karbovanets/karbowanecwallet/tags";
const static QString KARBO_DOWNLOAD_URL = "https://github.com/Karbovanets/karbowanecwallet/releases/";

class Updater : public QObject
{
    Q_OBJECT
public:
    explicit Updater(QObject *parent = 0);

    ~Updater() {
        delete manager;
    }

    void checkForUpdate();

signals:
    
public slots:
    void replyFinished (QNetworkReply *reply);

private:
   QNetworkAccessManager *manager;

};

#endif // UPDATE_H
