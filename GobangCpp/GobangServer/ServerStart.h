#pragma once
#include "WinSock2.h"
#include "ServerLogic.h"

#define PORT 4444
#define MESGSIZE 1024
#pragma comment(lib, "ws2_32.lib")

class ServerStart {
public:
    ServerStart();
    ~ServerStart();
    void Start();
private:
    ServerLogic serverLogic;
};