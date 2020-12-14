#include "httprequest.h"

#include "tools.h"
#include <vector>
#include <iostream>

HTTPRequest::HTTPRequest(const std::string &request) {
    std::vector<std::string> res = tool::split(request, "\n");

    std::vector<std::string> line1 = tool::split(res[0], " ");
    std::string _mode = line1[0];
    std::string _url = line1[1];
    std::string _http_version = line1[2];

    for (int i = 1; i < res.size(); i++) {
        std::vector<std::string> line = tool::split(res[i], ":");
        if (line[0] == "Host") {
            _host = tool::strip(line[1]);
        } else if (line[0] == "Accept") {
            _accept = tool::split(tool::strip(line[1]), ",");
        } else if (line[0] == "Accept-Language") {
            _accept_language = tool::strip(line[1]);
        } else if (line[0] == "Accept-Encoding") {
            _accept_encoding = tool::strip(line[1]);
        } else if (line[0] == "Connection") {
            _connection = tool::strip(line[1]);
        } else if (line[0] == "User-Agent") {
            _user_agent = tool::strip(line[1]);
        } else {
            // do something
        }
    }

}