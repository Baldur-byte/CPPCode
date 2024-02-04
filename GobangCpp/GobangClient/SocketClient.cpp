#include "SocketClient.h"

SocketClient::SocketClient() {

}

SocketClient::~SocketClient() {

}

void SocketClient::Start() {
    
    memset(buff, 0, sizeof(buff));

    if (WSAStartup(MAKEWORD(2, 2), &wsaDATA) != 0) {
        return;
    }

    SOCKADDR_IN addrServer;
    addrServer.sin_family = AF_INET;
    addrServer.sin_port = htons(PORT);
    addrServer.sin_addr.S_un.S_addr = inet_addr(SERVER_ADDRESS);

    socketClient = socket(AF_INET, SOCK_STREAM, 0);
    if (SOCKET_ERROR == socketClient) {
        //string error = WSAGetLastError();
        return;
    }

    if (connect(socketClient, (sockaddr*)&addrServer, sizeof(addrServer)) == INVALID_SOCKET) {
        //string error = WSAGetLastError();
        return;
    }
    else {
        recv(socketClient, buff, sizeof(buff), 0);
    }

    send(socketClient, buff, sizeof(buff), 0);
}

void SocketClient::Receive() {
    recv(socketClient, buff, sizeof(buff), 0);
}

void SocketClient::Send() {
    send(socketClient, buff, sizeof(buff), 0);
}

void SocketClient::Close() {
    closesocket(socketClient);
    WSACleanup();
}