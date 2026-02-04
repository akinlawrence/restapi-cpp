#include "AklaHttp.h"
#include "Response.h"
#include <iostream>
#include "api/Motivation.h"

int main()
{
    try {
        const int port = 8080;
        AklaHttp http;

        http.context("/api");

        http.get("/", [](AklaParams params, Response &res){
            int result =  stoi(params["one"]) + stoi(params["two"]);
            string end = "vastaus on: "+ to_string(result);
            res.send(end);
        });

        http.post("/", [](AklaParams params, Response &res){
            cout << params["_raw"] << endl;
            auto json = Response::bodyToJson(params["_raw"]);
            cout << endl << get<int>(json["dd"]) << " JSON" << endl;
            res.send("Home");
        });

        http.get("/home", [](AklaParams params, Response &res){
            cout << params["dd"] << endl;
            res.send("Home");
        });


        motivationApi(http);
        
        http.runServer(port,[](){
            cout << "Server started on http://localhost:" << port << endl;
        });

      
        
    } catch (const std::exception &e){
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
