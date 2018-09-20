#include "Haproxy.h"
#include "HTTPResponse.h"
#include <QDir>
#include <QFile>
#include <QDebug>
#include <string>
#include <iostream>
#include <istream>
#include <ostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

#ifdef Q_OS_WIN
    #include <windows.h>
#endif

void Haproxy::haproxy(const QString &host, const QString &ip, const QString &port, const QString &endpoint, const QString &endpointport, const QString &fixedHost, const QString &auth, const QString &provider, const QString &plan, const QString &serviceName) {
    qDebug() << "Starting haproxy";

    QFile::remove(host + "/provider.http");
    QFile fileProvider(host + "/provider.http");

    // delete old file and create new one
    QFile::remove(host + "/haproxy.cfg");
    QFile file(host + "/haproxy.cfg");

    if(!file.exists()){
        qDebug() << file.fileName() << " does not exist";
    }
    else{
        qDebug() << file.fileName() << " already exists";
    }

    //create provider.http
    if(fileProvider.open(QIODevice::ReadOnly | QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream txtStream(&fileProvider);
        qDebug() << "---Writing to file---";

        txtStream << "HTTP/1.0 200 PROVIDER\n";
        txtStream << "Access-Control-Allow-Origin: *\n";
        txtStream << "Access-Control-Allow-Methods: GET\n";
        txtStream << "Cache-Control: no-cache\n";
        txtStream << "Content-Type: text/html\n\n";

        txtStream << serviceName + "," + plan;

        qDebug() << " ----- reading from file ------";

        txtStream.seek(0);
        while(!txtStream.atEnd()) {
            qDebug() << txtStream.readLine();
        }
        fileProvider.close();
    }


    //create config file
    if (file.open(QIODevice::ReadOnly | QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Opened Config file for update";

        QTextStream txtStream(&file);
        qDebug() << "---Writing to file---";
        txtStream << "global\n";
        txtStream << "maxconn         2000\n";
        txtStream << "daemon\n";
        txtStream << "ssl-default-bind-ciphers ECDH+AESGCM\n";
        txtStream << "ssl-default-bind-options force-tlsv12\n";
        txtStream << "no log\n";


        txtStream << "frontend icproxy\n";
        txtStream << "bind            "+ip+":"+port+"\n";
        txtStream << "mode            http\n";
        txtStream << "log             global\n";
        //txtStream << "option          httplog\n";
        txtStream << "option          dontlognull\n";
        txtStream << "option          nolinger\n";
        txtStream << "option          http_proxy\n";
        txtStream << "option          contstats\n";
        txtStream << "maxconn         8000\n";
        txtStream << "timeout client  30s\n";
        //txtStream << "timeout server  30s\n";
        //txtStream << "timeout connect 5s\n";



        txtStream << "acl is_mgmt_host url_dom _local_\n";
        txtStream << "acl is_mgmt_path path_beg /status\n";
        txtStream << "acl is_stats_path path_beg /stats\n";
        txtStream << "acl is_mgmt_id hdr_reg(X-ITNS-MgmtID) ^"+provider+"$\n";
        txtStream << "acl is_proxy_request url_reg '.*://.*'\n";
        txtStream << "acl is_connect method CONNECT\n";
        txtStream << "acl is_options method OPTIONS\n";

        //txtStream << "acl is_provider path_beg /provider\n";

        //txtStream << "use_backend b-status if is_mgmt_host is_mgmt_path is_mgmt_id\n";
        //txtStream << "use_backend b-stats if is_mgmt_host is_stats_path is_mgmt_id\n";
        //txtStream << "use_backend b-err if is_mgmt_host is_mgmt_path\n";

        txtStream << "# If this is local request with right authid /stats, forward to stats backend\n";
        txtStream << "use_backend b-stats if !is_options !is_proxy_request is_stats_path is_mgmt_id\n";

        txtStream << "#  If this is local request with authid /status, forward to status backend\n";
        txtStream << "use_backend b-status if !is_proxy_request is_mgmt_path is_mgmt_id\n";

        txtStream << "# If this is proxy request with right id\n";
        txtStream << "use_backend b-status if is_mgmt_host is_mgmt_path is_mgmt_id\n";

        txtStream << "# If this is proxy request with right id\n";
        txtStream << "use_backend b-stats if is_mgmt_host is_stats_path is_mgmt_id\n";

        txtStream << "# Wrong mgmtid\n";
        txtStream << "use_backend b-err if is_mgmt_host is_mgmt_path !is_mgmt_id\n";

        txtStream << "# Forward OPTIONS to status\n";
        txtStream << "use_backend b-status if is_options !is_proxy_request is_mgmt_path is_mgmt_id\n";
        txtStream << "use_backend b-status if is_options !is_proxy_request is_stats_path\n";
        txtStream << "use_backend http-proxy if is_proxy_request || is_connect\n";


        //txtStream << "default_backend http-proxy\n";


        txtStream << "backend http-proxy\n";
        txtStream << "mode            http\n";
        txtStream << "timeout connect 5s\n";
        txtStream << "timeout server  30s\n";
        //txtStream << "timeout client  30s\n";
        txtStream << "retries         2\n";
        txtStream << "option          nolinger\n";
        txtStream << "option          httplog\n";
        txtStream << "http-request add-header X-ITNS-PaymentID "+auth+"\n";
        #ifdef Q_OS_WIN
        txtStream << "server hatls " + endpoint + ":" + endpointport + " force-tlsv12 ssl ca-file 'ca.cert.pem'\n";
        #else
        txtStream << "server hatls " + endpoint + ":" + endpointport + " force-tlsv12 ssl ca-file '"+host+"/ca.cert.pem'\n";
        #endif

        #ifdef Q_OS_WIN
        txtStream << "errorfile 503 ha_err_connect.http\n";
        #else
        txtStream << "errorfile 503 "+host+"/ha_err_connect.http\n";
        #endif

        txtStream << "backend b-err\n";
        txtStream << "mode            http\n";
        txtStream << "timeout server  30s\n";
        txtStream << "timeout connect 5s\n";
        //txtStream << "timeout client  30s\n";
        #ifdef Q_OS_WIN
        txtStream << "errorfile 503 ha_err_badid.http\n";
        #else
        txtStream << "errorfile 503 "+host+"/ha_err_badid.http\n";
        #endif

        txtStream << "backend b-status\n";
        txtStream << "mode            http\n";
        txtStream << "timeout server  30s\n";
        txtStream << "timeout connect 5s\n";
        //txtStream << "timeout client  30s\n";
        #ifdef Q_OS_WIN
        txtStream << "errorfile 503 ha_info.http\n";
        #else
        txtStream << "errorfile 503 "+host+"/ha_info.http\n";
        #endif

        txtStream << "backend b-stats\n";
        txtStream << "mode            http\n";
        txtStream << "timeout server  30s\n";
        txtStream << "timeout connect 5s\n";
        txtStream << "server Local 127.0.0.1:8181\n";

        txtStream << "listen  stats\n";
        txtStream << "timeout client  30s\n";
        txtStream << "timeout server  30s\n";
        txtStream << "timeout connect 5s\n";
        txtStream << "bind  127.0.0.1:8181\n";
        txtStream << "mode            http\n";
        txtStream << "stats enable\n";
        txtStream << "stats hide-version\n";
        txtStream << "stats refresh 30s\n";
        txtStream << "stats show-node\n";
        txtStream << "stats uri  /stats\n";

        txtStream << "listen provider\n";
        txtStream << "timeout client  30s\n";
        txtStream << "mode            http\n";
        txtStream << "timeout server  30s\n";
        txtStream << "timeout connect 5s\n";
        txtStream << "errorfile 503 "+host+"/provider.http\n";
        txtStream << "bind 127.0.0.1:8182\n";

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

    }
    else {
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




// helper function to store head and body response from boost
std::string buffer_to_string(const boost::asio::streambuf &buffer)
{
  using boost::asio::buffers_begin;
  auto bufs = buffer.data();
  std::string result(buffers_begin(bufs), buffers_begin(bufs) + buffer.size());
  return result;
}


// make a request to the proxy and return a response object with code headers and body
HttpResponse proxyRequest(std::string proxyHost, std::string proxyPort, std::string requestURL, std::string provider) {
    HttpResponse output = HttpResponse(0);

    // regular proxies use \r\n as boundaries but not this one, we need \n validation only
    std::string regularBoundary = "\r\n";
    std::string haproxyBoundary = "\n";

    try {

        boost::asio::io_service io_service;

        // Get a list of endpoints corresponding to the server name.
        tcp::resolver resolver(io_service);

        // set the hostand port of the proxy to query
        tcp::resolver::query query(proxyHost, proxyPort, boost::asio::ip::resolver_query_base::numeric_service);


        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

        // Try each endpoint until we successfully establish a connection.
        tcp::socket socket(io_service);
        boost::asio::connect(socket, endpoint_iterator);

        // create the request wihh all headers including mgmtid
        boost::asio::streambuf request;
        std::ostream request_stream(&request);

        request_stream << "GET " << requestURL << " HTTP/1.0" << regularBoundary;
        request_stream << "Host: " << proxyHost << regularBoundary;
        request_stream << "Accept: */*" << regularBoundary;
        request_stream << "Connection: close" << regularBoundary;
        request_stream << "X-ITNS-MgmtId: " << provider << regularBoundary << regularBoundary;

        std::cout << "Writing to socket\n";

        // Send the request.
        boost::asio::write(socket, request);

        // Read the response status line
        boost::asio::streambuf response;

        // regular boundaries do not work here
        boost::asio::read_until(socket, response, haproxyBoundary);

        // Check that response is OK.
        std::istream response_stream(&response);
        std::string http_version;
        response_stream >> http_version;
        unsigned int status_code;
        response_stream >> status_code;
        std::string status_message;
        std::getline(response_stream, status_message);


        std::cout << "Setting status code " << status_code << "\n";

        output.setStatusCode(status_code);

        if (!response_stream || http_version.substr(0, 5) != "HTTP/")
        {
            std::cout << "Invalid response\n";
            return output;
        }

        std::cout << "Response returned with status code " << status_code << "\n";


        // Read the response headers, which are terminated by a blank line.
        boost::asio::read_until(socket, response, "\n\n");

        // Process the response headers.
        std::string header;

        // append header to the response headers variable
        while (std::getline(response_stream, header) && header != haproxyBoundary) {
            output.addHeader(header);
        }

        // variable to keep contents of response body
        std::string body;

        // start storing response body
        if (response.size() > 0) {
            body += buffer_to_string(response);
        }


        // Read until EOF, save data for sending to caller
        boost::system::error_code error;
        while (boost::asio::read(socket, response, boost::asio::transfer_at_least(1), error)) {
            body += buffer_to_string(response);
        }

        if (error != boost::asio::error::eof) {
            throw boost::system::system_error(error);
        }

        output.setBody(body);

    }
    catch (std::exception& e)
    {
        std::cout << "Exception: " << e.what() << "\n";
    }

    return output;
}


// returns true if proxy is online and accepting connections, false otherwise
bool Haproxy::verifyHaproxy(const QString &host, const QString &port, const QString &provider)
{
    // TODO - this needs to be updated when new dispatcher is available
    std::string endpoint = std::string("http://_remote_/status");

    HttpResponse response = proxyRequest(host.toStdString(), port.toStdString(), endpoint, provider.toStdString());

    std::cout << std::endl << std::endl;
    std::cout << "Status " << std::endl;
    std::cout << response.getStatusCode() << std::endl;
    std::cout << "Header " << std::endl;
    for (auto const& s : response.getHeaders()) {
        std::cout << s.first << " -> " << s.second << std::endl;
    }

    std::cout << "Body " << std::endl;
    std::cout << response.getBody() << std::endl;

    // return response based on the header
    if ( response.getHeaders().find("X-ITNS-Status") != response.getHeaders().end() && response.getStatusCode() != 200) {
        std::cout << "ITNS Header found: " << response.getHeaders()["X-ITNS-Status"] << std::endl;
    }
    else {
        std::cout << "ITNS Header found: 200";
        return true;
    }


    return false;
}
