#include <QDir>
#include <QFile>
#include <QDebug>

void haproxy(){
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

        //bind            127.0.0.1:3129

/*

                             default_backend http-proxy

                     # Backend for proxying requests to another proxy
                     backend http-proxy

                             mode            http
                             timeout connect 5s
                             timeout server  5s
                             retries         2
                             option          nolinger
                             option          httplog

                             http-request add-header X-ITNS-PaymentID authid1
                             server hatls monitor.intensecoin.com:20000 ssl ca-file $/ca.cert.pem
*/


        qDebug() << " ----- reading from file ------";
        txtStream.seek(0);
        while(!txtStream.atEnd()){
            qDebug() << txtStream.readLine();
        }
        file.close();

    }else{
        qDebug() << "could not open the file";
        return;
    }


}
