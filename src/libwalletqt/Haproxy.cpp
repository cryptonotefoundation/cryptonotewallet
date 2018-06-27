#include "Haproxy.h"
#include <QDir>
#include <QFile>
#include <QDebug>
#include <iostream>
#include <string>
//#include <stdlib.h>

void Haproxy::haproxy(const QString &host, const QString &ip, const QString &port){
    QFile file (host+"haproxy.cfg");
    if(!file.exists()){
        qDebug() << file.fileName() << "does not exists";
    }
    if(file.open(QIODevice::ReadOnly | QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream txtStream(&file);
        qDebug() << "---Writing to file---";
        txtStream << "global\n";
        txtStream << "maxconn         2000\n";
        txtStream << "daemon\n";
        txtStream << "ssl-default-bind-ciphers ECDH+AESGCM:DH+AESGCM:ECDH+AES256:DH+AES256:ECDH+AES128:DH+AES:RSA+AESGCM:RSA+AES:!aNULL:!MD5:!DSS\n";
        txtStream << "ssl-default-bind-options no-sslv3\n";
        txtStream << "frontend icproxy\n";
        txtStream << "bind            "+ip+":"+port+"\n";
        txtStream << "mode            http\n";
        txtStream << "log             global\n";
        txtStream << "option          httplog\n";
        txtStream << "option          dontlognull\n";
        txtStream << "option          nolinger\n";
        txtStream << "option          http_proxy\n";
        txtStream << "option          contstats\n";
        txtStream << "maxconn         8000\n";
        txtStream << "timeout client  30s\n";
        txtStream << "default_backend http-proxy\n";
        txtStream << "backend http-proxy\n";
        txtStream << "mode            http\n";
        txtStream << "timeout connect 5s\n";
        txtStream << "timeout server  5s\n";
        txtStream << "retries         2\n";
        txtStream << "option          nolinger\n";
        txtStream << "option          httplog\n";
        txtStream << "http-request add-header X-ITNS-PaymentID authid1\n";
        txtStream << "server hatls monitor.intensecoin.com:20000 ssl ca-file "+host+"ca.cert.pem";
        txtStream << "";

        qDebug() << " ----- reading from file ------";
        txtStream.seek(0);
        while(!txtStream.atEnd()){
            qDebug() << txtStream.readLine();
        }
        file.close();
        #ifdef Q_OS_WIN
            qDebug() << " ----- run windows haproxy ------";
            const QString command=host+"haproxy.exe -f "+host+"haproxy.cfg";
            system(qPrintable(command));
        #else
            qDebug() << " ----- run linux haproxy ------";
            const QString command="haproxy -f "+host+"haproxy.cfg";
            system(qPrintable(command));
        #endif

    }else{
        qDebug() << "could not open the file";
        return;
    }
}
