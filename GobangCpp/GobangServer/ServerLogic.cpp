#include "ServerLogic.h"

ServerLogic::ServerLogic() {
    clientCount = 0;
}

ServerLogic::~ServerLogic() {

}

void ServerLogic::AddClient(SOCKADDR_IN addr, SOCKET socket) {
    char* ip = CommonMethod::AddrToStr(addr);

    for (int i = 0; i < clientCount; i++) {
        if (strcmp(clients[i].id, ip) == 0) {
            return;
        }
    }
    clients[clientCount].Start(ip, socket, this);
    clientCount++;
}

bool ServerLogic::JoinRoom(Player* player) {
    if (rooms[0].isFull()) {
        return false;
    }
    player->JoinRoom(&rooms[0]);
    rooms[0].AddPlayer(player);
    return true;
}

bool ServerLogic::ExitRoom(Player* player) {
    if (!player->GetRoom()) {
        return false;
    }
    player->GetRoom()->RemovePlayer(player);
    player->ExitRoom();
    return true;
}