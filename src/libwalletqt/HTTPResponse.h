#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include <boost/algorithm/string.hpp>
#include <map>


class HttpResponse {

private:
    int _statusCode;
    // associative array ["Date" => "2018-09-20", ...]
    std::map<std::string, std::string> _header;
    std::string _body;

public:
    HttpResponse(int code);


    // getters and setters
    std::map<std::string, std::string> getHeaders();


    // parse the header and insert it into our headers array
    void addHeader(std::string header);

    // update existing response headers
    void setHeader(std::map<std::string, std::string> header);

    // get a string with the body of the response
    std::string getBody();

    // set the body of the response
    void setBody(std::string body);

    // get the status code of the response: 200 or other
    int getStatusCode();

    // set the http status code for the response
    void setStatusCode(int code);

};

#endif // HTTPRESPONSE_HPP

