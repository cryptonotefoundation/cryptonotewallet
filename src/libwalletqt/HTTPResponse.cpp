#include "HTTPResponse.h"

HttpResponse::HttpResponse(int code) {
    _statusCode = code;
}


// getters and setters
std::map<std::string, std::string> HttpResponse::getHeaders() {
    return _header;
}


// parse the header and insert it into our headers array
void HttpResponse::addHeader(std::string header) {
    std::vector<std::string> parts;
    // split the header by the :
    boost::algorithm::split(parts, header, boost::algorithm::is_any_of(":"));
    if (parts.size() == 2) {

        // trim spaces from key and value
        boost::algorithm::trim(parts[1]);
        boost::algorithm::trim(parts[0]);

        _header.emplace(parts[0], parts[1]);
    }
    else { // if there are more parts, rejoin the parts without the first which is the key
        std::string key = parts[0];
        parts.erase(parts.begin());
        std::string joined = boost::algorithm::join(parts, ":");

        // trim spaces from value
        boost::algorithm::trim(joined);

        _header.emplace(key, joined);
    }

}

void HttpResponse::setHeader(std::map<std::string, std::string> header) {
    _header = header;
}

std::string HttpResponse::getBody() {
    return _body;
}



void HttpResponse::setBody(std::string body) {
    _body = body;
}

int HttpResponse::getStatusCode() {
    return _statusCode;
}

void HttpResponse::setStatusCode(int code) {
    _statusCode = code;
}
