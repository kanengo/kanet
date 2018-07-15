#pragma once
#include "kanet_server.h"
class Server
{
public:
    static Server* GetInstance()
    {

    }

    void run()
    {
        KanetServer::GetInstance()->run();
    }
};