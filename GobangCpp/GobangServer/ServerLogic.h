#pragma once
#include "WinSock2.h"
#include "CommonMethod.h"
#include "Room.h"
#include "Client.h"

class ServerLogic {
public:
    ServerLogic();
    ~ServerLogic();
    void AddClient(SOCKADDR_IN addr, SOCKET socket);
    bool JoinRoom(Player* player);
    bool ExitRoom(Player* player);
private:
    Client clients[100];
    Room rooms[1];
    int clientCount;
};