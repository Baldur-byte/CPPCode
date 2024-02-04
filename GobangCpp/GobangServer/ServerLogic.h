#pragma once
#include "WinSock2.h"
#include "Client.h";

class ServerLogic {
public:
    ServerLogic();
    ~ServerLogic();
    void AddClient(char* ip, SOCKET socket);
private:
    Client clients[100];
    int clientCount;
};