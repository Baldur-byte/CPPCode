#include "ServerLogic.h"

ServerLogic::ServerLogic() {
    clientSocketCount = 0;
    for (int i = 0; i < 20; i++) {
        rooms[i].SetID(100 + i + 1);
    }
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

Room** ServerLogic::RoomList() {
    Room** result = new Room*[20];
    for (int i = 0; i < 20; i++) {
        result[i] = &rooms[i];
    }
    return result;
}

bool ServerLogic::JoinRoom(int roomId, Player* player) {
    for (int i = 0; i < 20; i++) {
        if (rooms[i].ID() == roomId && !rooms[i].isFull()) {
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