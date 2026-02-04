

#pragma once
#include "AklaHttp.h"


void motivationApi(AklaHttp &http){
    http.context("/api/motivation");

    http.get("/", [](AklaParams params, Response &res){
        string end = "moti";
        res.send(end);
    });

}
