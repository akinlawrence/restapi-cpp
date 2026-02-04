#ifndef URLHANDLER_H
#define URLHANDLER_H

#include <boost/algorithm/string.hpp>
#include <boost/beast.hpp>
#include <iostream>
#include <array>
#include <string>
#include <map>

namespace http = boost::beast::http;
using namespace std;


class URLHandler
{
    public:
        URLHandler();
        static vector<string> urlToArray(http::request<http::string_body>& request);
        static map<string, string> urlParser(string url);
        static string urlTopath(http::request<http::string_body> &request);
        virtual ~URLHandler();

    protected:

    private:
        
};

#endif // URLHANDLER_H
