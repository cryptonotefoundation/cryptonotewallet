// Copyright (c) 2016 The Karbowanec developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#include "Update.h"
#include "Settings.h"
#include <QDesktopServices>
#include <QApplication>
#include <QMessageBox>

#include <iostream>
#include <sstream>
#include <vector>
#include <iterator>

using namespace WalletGui;

Updater::Updater(QObject *parent) :
    QObject(parent)
{
}
// http://stackoverflow.com/questions/2941491/compare-versions-as-strings/2941895#2941895
class Version
{
    // An internal utility structure just used to make the std::copy in the constructor easy to write.
    struct VersionDigit
    {
        int value;
        operator int() const {return value;}
    };
    friend std::istream& operator>>(std::istream& str, Version::VersionDigit& digit);
    public:
        Version(std::string const& versionStr)
        {
            // To Make processing easier in VersionDigit prepend a '.'
            std::stringstream   versionStream(std::string(".") + versionStr);

            // Copy all parts of the version number into the version Info vector.
            std::copy(  std::istream_iterator<VersionDigit>(versionStream),
                        std::istream_iterator<VersionDigit>(),
                        std::back_inserter(versionInfo)
                     );
        }

        // Test if two version numbers are the same.
        bool operator<(Version const& rhs) const
        {
            return std::lexicographical_compare(versionInfo.begin(), versionInfo.end(), rhs.versionInfo.begin(), rhs.versionInfo.end());
        }

    private:
        std::vector<int>    versionInfo;
};

// Read a single digit from the version.
std::istream& operator>>(std::istream& str, Version::VersionDigit& digit)
{
    str.get();
    str >> digit.value;
    return str;
}

void Updater::checkForUpdate()
{
    manager = new QNetworkAccessManager(this);
    if(manager->networkAccessible() == QNetworkAccessManager::Accessible)
    {
        connect(manager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(replyFinished(QNetworkReply*)));
        manager->get(QNetworkRequest(QUrl(KRBCOIN_UPDATE_URL)));
    }
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
