#include "Client.h"

static void ReceiveT(Client* client) {
    Log::Info("客户端信息收取中。。。 : ", client->id);
    char receiveBuf[1024];
    while (true)
    {
        memset(receiveBuf, 0, sizeof(receiveBuf));
        recv(client->socketClient, receiveBuf, sizeof(receiveBuf), 0);

    }
}

void Client::SendT(void) {
    /*Log::Info("可向客户端发送信息。。。 : ", GetIp());
    while (true) {
        if (!queueToSend.empty()) {
            const char* sendbuf = queueToSend.front();
            if (send(socketClient, sendbuf, sizeof(sendbuf), 0) != SOCKET_ERROR) {
                queueToSend.pop();
            }
            else {
                Log::Info("消息发送失败 : ", GetIp());
            }
        }
    }*/
    int b = 0;
}

Client::Client() {
    socketClient = NULL;
}

Client::~Client()
{
}

void Client::Start(char* ip, SOCKET socketClient) {
    this->socketClient = socketClient;
    this->id = ip;

    ReceiveThread = thread(ReceiveT, this);
    SendThread = thread(&Client::SendT);

    //ReceiveThread.detach();
    //SendThread.detach();
}

void Client::Send(const char* sendbuf) {
    
}

void Client::CloseSocket() {
    closesocket(socketClient);
}
