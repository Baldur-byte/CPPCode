#include "SocketClient.h"
#include <ws2tcpip.h>

static void ReceiveT(SocketClient* Client) {
    char receiveBuf[1024];
    while (true)
    {
        memset(receiveBuf, 0, sizeof(receiveBuf));
        recv(Client->socketClient, receiveBuf, sizeof(receiveBuf), 0);

    }
}

SocketClient::SocketClient() {

}

SocketClient::~SocketClient() {

}

void SocketClient::Start(Player* player) {
    memset(buff, 0, sizeof(buff));

    if (WSAStartup(MAKEWORD(2, 2), &wsaDATA) != 0) {
        return;
    }

    SOCKADDR_IN addrServer;
    addrServer.sin_family = AF_INET;
    addrServer.sin_port = htons(PORT);
    //addrServer.sin_addr.S_un.S_addr =  inet_addr(SERVER_ADDRESS);
    inet_pton(AF_INET, SERVER_ADDRESS, &(addrServer.sin_addr));

    socketClient = socket(AF_INET, SOCK_STREAM, 0);
    if (SOCKET_ERROR == socketClient) {
        //string error = WSAGetLastError();
        return;
    }

    if (connect(socketClient, (sockaddr*)&addrServer, sizeof(addrServer)) == INVALID_SOCKET) {
        //string error = WSAGetLastError();
        return;
    }
    ReceiveThread = thread(ReceiveT, this);
    this->player = player;
}

void SocketClient::Receive() {
    
}

void SocketClient::Send() {
    send(socketClient, buff, sizeof(buff), 0);
}

void SocketClient::Close() {
    closesocket(socketClient);
    WSACleanup();
}