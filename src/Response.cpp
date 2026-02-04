#include "Response.h"
#include "json.hpp"

Response::Response(unsigned int version)
{
    // ctor
    response.version(version);
}

Response::~Response()
{
    // dtor
}

void Response::sendStatus(int status, string body)
{
    response.result(status);
    response.set(http::field::server, "Akla Server");
    response.set(http::field::content_type, "text/plain");
    response.body() = body;
    response.prepare_payload();
}

void Response::send(string body)
{
    response.result(http::status::ok);
    response.set(http::field::server, "Akla Server");
    response.set(http::field::content_type, "text/plain");
    response.body() = body;
    response.prepare_payload();
}

void Response::json(map<string, string> obj)
{
    using json = nlohmann::json;
    json j = obj;
    string jsonText = j.dump();

    response.result(http::status::ok);
    response.set(http::field::server, "Akla Server");
    response.set(http::field::content_type, "application/json");
    response.body() = jsonText;
    response.prepare_payload();
}

void Response::file(string file)
{
    ifstream MyFile(file);

    string line;
    string fullText;

    while (getline(MyFile, line))
        fullText += line;
    MyFile.close();

    response.result(http::status::ok);
    response.set(http::field::server, "Akla Server");
    response.set(http::field::content_type, "text/html");
    response.body() = fullText;
    response.prepare_payload();
}

map<string, variant<int, double, string>> Response::bodyToJson(string body)
{
    using json = nlohmann::json;
    using Value = std::variant<int, double, std::string>;
    using JsonMap = std::map<std::string, Value>;
    json j = json::parse(body);
    JsonMap jsonMap;

    for (auto &[key, value] : j.items())
    {
        if (value.is_number_integer())
        {
            jsonMap[key] = value.get<int>();
        }
        else if (value.is_number_float())
        {
            jsonMap[key] = value.get<double>();
        }
        else if (value.is_string())
        {
            jsonMap[key] = value.get<std::string>();
        }
        return jsonMap;
    }
}