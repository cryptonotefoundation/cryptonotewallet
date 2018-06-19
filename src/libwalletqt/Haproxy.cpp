#include "Haproxy.h"
#include <QDir>
#include <QFile>
#include <QDebug>
#include <stdlib.h>

void Haproxy::haproxy(){
    qDebug() << "my haproxy function file was called";
    QFile file ("haproxy.cfg");
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
        txtStream << "bind 127.0.0.1:3129\n";
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
        txtStream << "server hatls monitor.intensecoin.com:20000 ssl ca-file /home/laion/Desktop/alfasoft/intense-wallet-gui/v2/intensecoinwallet/ca.cert.pem";
        txtStream << "";

        qDebug() << " ----- reading from file ------";
        txtStream.seek(0);
        while(!txtStream.atEnd()){
            qDebug() << txtStream.readLine();
        }
        file.close();
        system("haproxy -d -f haproxy.cfg");
    }else{
        qDebug() << "could not open the file";
        return;
    }


}
