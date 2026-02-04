#ifndef RESPONSE_H
#define RESPONSE_H

#include <iostream>
#include <boost/beast.hpp>
#include <variant>
#include <fstream>

namespace http = boost::beast::http;
using namespace std;

class Response
{
    public:
        Response(unsigned int version);
        void sendStatus(int status, string body);
        void send(string text);
        void json(map<string, string> obj);
        void file(string file);
        static map<string, variant<int, double, string>> bodyToJson(string body);
        virtual ~Response();
        http::response<http::string_body> response;

    protected:

    private:
      
        unsigned int version;
        
};

#endif // RESPONSE_H
