#include "AklaHttp.h"
#include "URLHandler.h"
#include "Response.h"

std::map<std::string, std::function<void(std::map<std::string, std::string> &, Response &)>> AklaHttp::allPostCallBack;
std::map<std::string, std::function<void(std::map<std::string, std::string> &, Response &)>> AklaHttp::allGetCallBack;
std::map<std::string, std::function<void(std::map<std::string, std::string> &, Response &)>> allGetMiddwareCallBack;

AklaHttp::AklaHttp()
{
    // ctor
}

void AklaHttp::handleRequest(http::request<http::string_body> &request, tcp::socket &socket)
{
    std::mutex cout_mutex;
    std::lock_guard<std::mutex> lock(cout_mutex);
    string method{request.method_string()};

    http::response<http::string_body> response;
    response.version(request.version());
    response.result(http::status::not_found);
    response.set(http::field::server, "Akla Server");
    response.set(http::field::content_type, "text/plain");
    response.body() = "404 not found";
    response.prepare_payload();

    vector<string> paths = URLHandler::urlToArray(request);
    vector<string>::iterator it = paths.end() - 1;

    auto params = URLHandler::urlParser(*it);
    auto activeURL = URLHandler::urlTopath(request);

    string body = request.body();
    // Time related
    auto end = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    string time_string = std::ctime(&end_time);
    time_string.erase(std::remove(time_string.begin(), time_string.end(), '\n'),
                      time_string.end());

    string output = "[" + time_string + "] " + "\033[96;1;4m" + method + " " + activeURL + "\033[0m";

    cout << output;

    Response res(request.version());

    if (method.compare("GET") == 0)
    {
        if (AklaHttp::allGetCallBack.find(activeURL) != AklaHttp::allGetCallBack.end())
        {
            try
            {
                AklaHttp::allGetCallBack[activeURL](params, res);
            }
            catch (const std::exception &e)
            {
                res.sendStatus(500, "");
                std::cerr << "\nIn path " + activeURL + "; " << "Exception: " << e.what() << std::endl;
            }
        }

        else
        {
            boost::beast::http::write(socket, response);
            return;
        }
    }
    else if (method.compare("POST") == 0)
    {
        if (AklaHttp::allPostCallBack.find(activeURL) != AklaHttp::allPostCallBack.end())
        {
            map<string, string> rawBody;
            rawBody.insert({"_raw", body});
            AklaHttp::allPostCallBack[activeURL](rawBody, res);
        }
        else
        {
            boost::beast::http::write(socket, response);
            return;
        }
    }
    boost::beast::http::write(socket, res.response);
}

void AklaHttp::runServer(short unsigned int port, function<void()> run)
{
    boost::asio::io_context io_context;
    tcp::acceptor acceptor(io_context, {tcp::v4(), port});

    run();
    while (true)
    {
        tcp::socket socket(io_context);
        acceptor.accept(socket);

        boost::beast::flat_buffer buffer;
        http::request<http::string_body> request;

        boost::beast::http::read(socket, buffer, request);

        handleRequest(request, socket);
        // std::flush( std::cout );
        socket.shutdown(tcp::socket::shutdown_send);
    }
}

void AklaHttp::get(string route, function<void(map<string, string> &, Response &)> fun)
{
    if (route.back() == '/')
        route.pop_back();
    cout << endl
         << activeContextPath + route;
    AklaHttp::allGetCallBack.insert({activeContextPath + route, fun});
}

void AklaHttp::get(string route, vector<function<void(Response &)>> middlewares, function<void(map<string, string> &, Response &)> fun)
{
    if (route.back() == '/')
        route.pop_back();
    for (auto middleware : middlewares)
    {
        AklaHttp::allGetCallBack.insert({activeContextPath + route, fun});
        // ware
    }
    AklaHttp::allGetCallBack.insert({route, fun});
}

void AklaHttp::post(string route, function<void(map<string, string> &, Response &)> fun)
{
    if (route.back() == '/')
        route.pop_back();
    AklaHttp::allPostCallBack.insert({activeContextPath + route, fun});
}

void AklaHttp::context(string route)
{
    if (route.back() == '/')
        route.pop_back();

    cout << route;

    activeContextPath = route;
}

AklaHttp::~AklaHttp()
{
    // dtor
}
