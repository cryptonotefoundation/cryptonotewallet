#include "Haproxy.h"
#include <QDir>
#include <QFile>
#include <QDebug>
#include <iostream>
#include <string>

#ifdef Q_OS_WIN
    #include <windows.h>
#endif

void Haproxy::haproxy(const QString &host, const QString &ip, const QString &port, const QString &endpoint, const QString &endpointport, const QString &fixedHost, const QString &auth){
    QFile::remove(host+"/haproxy.cfg");
    QFile file (host+"/haproxy.cfg");

    if(!file.exists()){
        qDebug() << file.fileName() << "does not exists";
    }else{

    }
    if(file.open(QIODevice::ReadOnly | QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream txtStream(&file);
        qDebug() << "---Writing to file---";
        txtStream << "global\n";
        txtStream << "maxconn         2000\n";
        txtStream << "daemon\n";
        //txtStream << "log " +host+ " local0\n";
        //txtStream << "log " +host+ " local1 notice\n";

        txtStream << "log /dev/log    local0\n";
        txtStream << "log /dev/log    local1 notice\n";
        txtStream << "ssl-default-bind-ciphers ECDH+AESGCM:DH+AESGCM:ECDH+AES256:DH+AES256:ECDH+AES128:DH+AES:!aNULL:!MD5:!DSS\n";
        txtStream << "ssl-default-bind-options no-sslv3\n";
        txtStream << "frontend icproxy"
                     "\n";
        txtStream << "bind            "+ip+":"+port+"\n";
        txtStream << "mode            http\n";
        txtStream << "stats           enable  # Enable stats page\n";
        txtStream << "stats           hide-version  # Hide HAProxy version\n";
        txtStream << "stats           uri /haproxy_stats  # Stats URI\n";
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
        txtStream << "http-request add-header X-ITNS-PaymentID "+auth+"\n";
        //
        #ifdef Q_OS_WIN
        txtStream << "server hatls " + endpoint + ":" + endpointport + " ssl ca-file 'ca.cert.pem'";
        #else
        txtStream << "server hatls " + endpoint + ":" + endpointport + " ssl ca-file '"+host+"/ca.cert.pem'";
        #endif

        qDebug() << " ----- reading from file ------";

        txtStream.seek(0);
        while(!txtStream.atEnd()){
            qDebug() << txtStream.readLine();
        }
        file.close();

	QString command = "";
        #ifdef Q_OS_WIN
            command="haproxy.exe -f haproxy.cfg";
            WinExec(qPrintable(command),SW_HIDE);
        #else
            //system("trap 'pkill -f haproxy; echo teste haproxy; exit;' INT TERM");
            command=fixedHost+" -f "+host+"/haproxy.cfg";
            qDebug() << command;
            system(qPrintable(command));
        #endif

    }else{
        qDebug() << "could not open the file";
        return;
    }
}

void Haproxy::haproxyCert(const QString &host, const QString &certificate){
    QFile::remove(host+"/ca.cert.pem");
    QFile file (host+"/ca.cert.pem");
    if(!file.exists()){
        qDebug() << file.fileName() << "Certificate does not exists";
    }
    if(file.open(QIODevice::ReadOnly | QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream txtStream(&file);
        qDebug() << "---Writing to file---";
        txtStream << certificate;
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



void Haproxy::killHAproxy(){
	qDebug() << "kill proxy";
    #ifdef Q_OS_WIN
        WinExec("taskkill /f /im haproxy.exe",SW_HIDE);
    #else
        system("pkill -f haproxy");
    #endif
}

QString Haproxy::verifyHaproxy(const QString &host, const QString &ip, const QString &provider){
    QString command = "";
    QString v = "";

    FILE *fp;
    char path[1035];




    #ifdef Q_OS_WIN
        command="curl --proxy-header 'X-ITNS-MgmtId: "+provider+"' -x http://"+host+":"+ip+"/ http://_remote_/status";
        return QString::number(WinExec(qPrintable(command), SW_HIDE));
    #else


        command="curl -D /dev/stderr --proxy-header 'X-ITNS-MgmtId: "+provider+"' -x http://"+host+":"+ip+"/ http://_remote_/status";
        /*
        //v = system(qPrintable(command));
        return v;
        */

        /* Open the command for reading. */
        fp = popen(qPrintable(command), "r");
        if (fp == NULL) {
            printf("Failed to run command\n" );
            exit(1);
        }

        /* Read the output a line at a time - output it. */
        while (fgets(path, sizeof(path)-1, fp) != NULL) {
            qDebug() << path;
            qDebug() << v;
            v = v + path;
        }

        /* close */
        pclose(fp);

        return v;

    #endif
}
