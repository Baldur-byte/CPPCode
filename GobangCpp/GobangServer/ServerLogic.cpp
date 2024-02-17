#include "ServerLogic.h"

ServerLogic::ServerLogic() {
    clientCount = 0;
}

ServerLogic::~ServerLogic() {

}

void ServerLogic::AddClient(SOCKADDR_IN addr, SOCKET socket) {
    char* ip = CommonMethod::AddrToStr(addr);

    for (int i = 0; i < clientCount; i++) {
        if (_stricmp(clients[i].id, ip)) {
            return;
        }
    }
    clients[clientCount].Start(ip, socket);
    clientCount++;
}