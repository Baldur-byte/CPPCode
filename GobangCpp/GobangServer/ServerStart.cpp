#include "ServerStart.h"
#include "Log.h"

ServerStart::ServerStart() {
}

ServerStart::~ServerStart() {

}

void ServerStart::Start() {
    WSADATA wsaDATA;
    int port = 8888;

    if (WSAStartup(MAKEWORD(2, 2), &wsaDATA) != 0) {
        Log::Info("初始化失败");
        return;
    }

    SOCKET socketServer = socket(AF_INET, SOCK_STREAM, 0);

    SOCKADDR_IN addrServer;
    addrServer.sin_family = AF_INET;
    addrServer.sin_port = htons(port);
    addrServer.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

    if (bind(socketServer, (LPSOCKADDR)&addrServer, sizeof(SOCKADDR_IN)) == SOCKET_ERROR) {
        Log::Info("服务器连接失败");
        return;
    }

    if (listen(socketServer, 10) == SOCKET_ERROR) {
        Log::Info("服务器监听失败");
        return;
    }

    Log::Info("服务器开启成功等待客户端连接！");
    Log::Info("服务器IP : ", inet_ntoa(addrServer.sin_addr));
    Log::Info("服务器端口号 ： ", addrServer.sin_port);

    SOCKADDR_IN addrClient;
    int len = sizeof(SOCKADDR);

    while (true) {
        SOCKET socketConnect = accept(socketServer, (SOCKADDR*)&addrClient, &len);
        if (socketConnect == SOCKET_ERROR) {
            Log::Info("客户端连接失败");
            break;
        }

        char* ip = inet_ntoa(addrClient.sin_addr);

        Log::Info("客户端连接成功 IP : ", ip);

        serverLogic.AddClient(ip, socketConnect);
    }

    closesocket(socketServer);
    WSACleanup();
}