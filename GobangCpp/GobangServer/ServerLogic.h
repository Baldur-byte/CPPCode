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
    Room** RoomList();
    bool JoinRoom(int roomId, Player* player);
    bool ExitRoom(Player* player);
private:
    ClientSocket clientSockets[100];
    Room rooms[20];
    int clientSocketCount;
};