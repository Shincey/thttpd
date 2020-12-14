#ifndef __HTTPRESPONSE_H__
#define __HTTPRESPONSE_H__

#include "httprequest.h"

struct HTTPResponseHeader {

};

class HTTPResponse {
public:
    HTTPResponse();
    static std::string DoResponse(HTTPRequest &request);
};

#endif // __HTTPRESPONSE_H__