#include "ServerStart.h"

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

    //char hostName[256];
    //if(!gethostname(hostName,sizeof(hostName)))
    //{
    //    hostent *host=gethostbyname(hostName);
    //    getaddrinfo();
    //    if(host!=NULL)
    //    {
    //        //*(struct in_addr*)*host->h_addr_list
    //        Log::Info("������IP : ", *host->h_addr_list);
    //    }
    //}
    Log::Info("�����������ɹ��ȴ��ͻ������ӣ�");
    //Log::Info("������IP : ", addrServer);
    //Log::Info("�������˿ں� �� ", addrServer.sin_port);
    Log::Info("�������˿ں� �� ", port);

    SOCKADDR_IN addrClient;
    int len = sizeof(SOCKADDR);

    while (true) {
        SOCKET socketConnect = accept(socketServer, (SOCKADDR*)&addrClient, &len);
        if (socketConnect == SOCKET_ERROR) {
            Log::Info("�ͻ�������ʧ��");
            break;
        }

        Log::Info("�ͻ������ӳɹ� IP : ", addrClient);

        serverLogic.AddClient(addrClient, socketConnect);
    }

    closesocket(socketServer);
    WSACleanup();
}