#include "ServerLogic.h"

ServerLogic::ServerLogic() {
    clientCount = 0;
}

ServerLogic::~ServerLogic() {

}

void ServerLogic::AddClient(char* ip, SOCKET socket) {
    for (int i = 0; i < clientCount; i++) {
        if (_stricmp(clients[i].GetIp(), ip)) {
            return;
        }
    }
    clients[clientCount].Start(ip, socket);
    clientCount++;
}