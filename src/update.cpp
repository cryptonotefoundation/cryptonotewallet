// Copyright (c) 2016 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#include "update.h"
#include "Settings.h"
#include <QDesktopServices>
#include <QApplication>
#include <QMessageBox>
#include <cstdio>
#include <string>
#include <iostream>

using namespace WalletGui;

Updater::Updater(QObject *parent) :
    QObject(parent)
{
}

struct Version
{
    int major, minor, revision;

    Version(const std::string& version)
    {
        std::sscanf(version.c_str(), "%d.%d.%d", &major, &minor, &revision);
        if (major < 0) major = 0;
        if (minor < 0) minor = 0;
        if (revision < 0) revision = 0;

    }

    bool operator < (const Version& other)
    {
        if (major < other.major)
            return true;
        if (minor < other.minor)
            return true;
        if (revision < other.revision)
            return true;

        return false;
    }

    bool operator == (const Version& other)
    {
        return major == other.major
            && minor == other.minor
            && revision == other.revision;
    }

    friend std::ostream& operator << (std::ostream& stream, const Version& ver)
    {
        stream << ver.major;
        stream << '.';
        stream << ver.minor;
        stream << '.';
        stream << ver.revision;
        return stream;
    }
};

void Updater::checkForUpdate()
{
    manager = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));

    manager->get(QNetworkRequest(QUrl(KRBCOIN_UPDATE_URL)));
}

void Updater::replyFinished (QNetworkReply *reply)
{
    if(reply->error())
    {
 //       qDebug() << "ERROR!";
 //       qDebug() << reply->errorString();
    }
    else
    {
        Version ourVersion = Settings::instance().getVersion().split("-")[0].toStdString();
	
		QString result = reply->readAll().data();      

		Version remoteVersion = result.toStdString();

         if (ourVersion < remoteVersion) {

             if (QMessageBox::warning(nullptr, QObject::tr("New version available"), QObject::tr("There is update available.\nDo you want to go to download page?"), QMessageBox::Ok, QMessageBox::Cancel) == QMessageBox::Ok) {
                 QString link = "http://karbowanec.com/#downloads";
                 QDesktopServices::openUrl(QUrl(link));
             }

         }
    }

    reply->deleteLater();
}
