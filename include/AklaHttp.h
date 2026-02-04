#ifndef AKLAHTTP_H
#define AKLAHTTP_H

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <iostream>
#include <string>
#include "Response.h"

using tcp = boost::asio::ip::tcp;
namespace http = boost::beast::http;
using namespace std;
typedef std::map<std::string, std::string>& AklaParams;

class AklaHttp
{
public:
    AklaHttp();
    virtual ~AklaHttp();
    static map<string, function<void(map<string, string>&, Response &)>> allGetCallBack;
    static map<string, function<void(map<string, string>&, Response &)>> allPostCallBack;
    void get(string route, function<void(map<string, string>&, Response &)> fun);
    void get(string route, vector<function<void( Response &)>> middlewares, function<void(map<string, string> &, Response &)> fun);
    void post(string route, function<void(map<string, string>&, Response &)> fun);
    void context(string route);
    void runServer(short unsigned int port, function<void()> run);
    void handleRequest(http::request<http::string_body> &request, tcp::socket &socket);

protected:
private:
    string activeContextPath;
};

#endif // AKLAHTTP_H
