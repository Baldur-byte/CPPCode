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
        Log::Info("��ʼ��ʧ��");
        return;
    }

    SOCKET socketServer = socket(AF_INET, SOCK_STREAM, 0);

    SOCKADDR_IN addrServer;
    addrServer.sin_family = AF_INET;
    addrServer.sin_port = htons(port);
    addrServer.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

    if (bind(socketServer, (LPSOCKADDR)&addrServer, sizeof(SOCKADDR_IN)) == SOCKET_ERROR) {
        Log::Info("����������ʧ��");
        return;
    }

    if (listen(socketServer, 10) == SOCKET_ERROR) {
        Log::Info("����������ʧ��");
        return;
    }

    Log::Info("�����������ɹ��ȴ��ͻ������ӣ�");
    Log::Info("������IP : ", inet_ntoa(addrServer.sin_addr));
    Log::Info("�������˿ں� �� ", addrServer.sin_port);

    SOCKADDR_IN addrClient;
    int len = sizeof(SOCKADDR);

    while (true) {
        SOCKET socketConnect = accept(socketServer, (SOCKADDR*)&addrClient, &len);
        if (socketConnect == SOCKET_ERROR) {
            Log::Info("�ͻ�������ʧ��");
            break;
        }

        char* ip = inet_ntoa(addrClient.sin_addr);

        Log::Info("�ͻ������ӳɹ� IP : ", ip);

        serverLogic.AddClient(ip, socketConnect);
    }

    closesocket(socketServer);
    WSACleanup();
}