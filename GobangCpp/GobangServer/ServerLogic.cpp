#include "ServerLogic.h"

ServerLogic::ServerLogic() {
    clientSocketCount = 0;
}

ServerLogic::~ServerLogic() {

}

void ServerLogic::AddClient(SOCKADDR_IN addr, SOCKET socket) {
    char* ip = CommonMethod::AddrToStr(addr);

    for (int i = 0; i < clientSocketCount; i++) {
        if (strcmp(clientSockets[i].id, ip) == 0) {
            return;
        }
    }
    clientSockets[clientSocketCount].Start(ip, socket, this);
    clientSocketCount++;
}

bool ServerLogic::JoinRoom(Player* player) {
    for (int i = 0; i < 20; i++) {
        if (!rooms[i].isFull()) {
            player->JoinRoom(&rooms[i]);
            rooms[i].AddPlayer(player);
            return true;
        }
    }
    return false;
}

bool ServerLogic::ExitRoom(Player* player) {
    if (!player->GetRoom()) {
        return false;
    }
    player->GetRoom()->RemovePlayer(player);
    player->ExitRoom();
    return true;
}