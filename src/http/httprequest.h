#ifndef __HTTPREQUEST_H__
#define __HTTPREQUEST_H__

#include <string>
#include <vector>

#include "httptype.h"

class HTTPRequest {
public:
    HTTPRequest(const std::string &request);

    std::string _mode;
    std::string _url;
    std::string _http_version;
    std::vector<std::string> _accept;
    std::string _connection;
    std::string _host;
    std::string _user_agent;
    std::string _accept_encoding;
    std::string _accept_language;
};


#endif // __HTTPREQUEST_H__