#pragma once
#include "WinSock2.h"
#include "Client.h"
#include "CommonMethod.h"

class ServerLogic {
public:
    ServerLogic();
    ~ServerLogic();
    void AddClient(SOCKADDR_IN addr, SOCKET socket);
private:
    Client clients[100];
    int clientCount;
};