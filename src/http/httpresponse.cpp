#include "httpresponse.h"

#include "tools.h"

HTTPResponse::HTTPResponse() {

}


std::string HTTPResponse::DoResponse(HTTPRequest &request){
    using namespace tool;
    std::string content = "<!DOCTYPE html><html><head><title>THTTPD HOME PAGE</title></head><body><h1>Title</h1><p>一段中文测试使用</p></body></html>";
    std::string response;
    response << "HTTP/1.1 200 OK\r\n";
    response << "Server: thttpd\r\n";
    response << "Content-Language: zh-CN,zh\r\n";
    response << "Content-Type: text/html;charset=GB2312\r\n";
    response << "Content-Length: " << (int32_t)content.size() << "\r\n\r\n";
    response << content << "\r\n";
    return response;
}