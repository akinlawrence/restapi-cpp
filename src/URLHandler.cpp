#include "URLHandler.h"

URLHandler::URLHandler()
{
    // ctor
}

URLHandler::~URLHandler()
{
    // dtor
}

vector<string> URLHandler::urlToArray(http::request<http::string_body> &request)
{
    string input(request.target().to_string());
    vector<string> result;
    boost::split(result, input, boost::is_any_of("/"));
    return result;
}

string URLHandler::urlTopath(http::request<http::string_body> &request)
{
    string input(request.target().to_string());
    string result = input.substr(0, input.find("?"));
    if (!result.empty() && result.back() == '/')
    {
        result.pop_back();
    }
    return result.compare("") == 0 ? "/" : result;
}

map<string, string> URLHandler::urlParser(string url)
{
    string input(url.substr(url.find("?") + 1, url.length()));
    vector<string> result;
    boost::split(result, input, boost::is_any_of("&"));
    map<string, string> params;

    // cout << endl;
    for (vector<string>::iterator it = result.begin(); it != result.end(); ++it)
    {
        string first_param = ((string)*it).substr(0, ((string)*it).find("="));
        string second_param = ((string)*it).substr((int)((string)*it).find("=") + 1, ((string)*it).length());

        params.insert({first_param, second_param});
        // cout << first_param << "->" << second_param << endl;
    }

    return params;
}
