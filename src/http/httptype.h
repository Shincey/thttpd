#ifndef __HTTPTYPE_H__
#define __HTTPTYPE_H__


enum class HTTPRequestMode {
    GET,
    HEAD,
    POST,
    DELETE,
};

enum class HTTPCode {
    OK = 200, // 客户端请求成功
    BAD_REQUEST = 400, // 客户端请求有语法错误，不能被服务器识别
    UNAUTHORIZED = 401, // 请求未经授权
    FORBIDDEN = 403, // 服务器收到请求，拒绝服务
    NOT_FOUND = 404, // 请求的资源不存在
    INTERNAL_SERVER_ERROR = 500, // 服务器发生不可预期的错误
    SERVICE_UNAVAILABLE = 503, // 服务器当前不能够处理客户端的请求
};


#endif // __HTTPTYPE_H__