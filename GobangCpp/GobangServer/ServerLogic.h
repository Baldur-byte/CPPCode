#pragma once
#include "WinSock2.h"
#include "CommonMethod.h"
#include "Room.h"
#include "ClientSocket.h"

class ServerLogic {
public:
    ServerLogic();
    ~ServerLogic();
    void AddClient(SOCKADDR_IN addr, SOCKET socket);
    bool JoinRoom(Player* player);
    bool ExitRoom(Player* player);
private:
    ClientSocket clientSockets[100];
    Room rooms[1];
    int clientSocketCount;
};