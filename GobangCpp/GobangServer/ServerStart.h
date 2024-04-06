#pragma once
#include "WinSock2.h"
#include "ServerLogic.h"
#include "Log.h"

#define PORT 4444
#pragma comment(lib, "ws2_32.lib")

class ServerStart {
public:
    ServerStart();
    ~ServerStart();
    void Start();
private:
    ServerLogic serverLogic;
};