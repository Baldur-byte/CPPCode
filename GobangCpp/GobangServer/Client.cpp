#include "Client.h"

Client::Client() {
    ip = nullptr;
    socketClient = NULL;
}

Client::~Client()
{
}

void Client::Start(char* ip, SOCKET socketClient) {
    this->socketClient = socketClient;
    this->ip = ip;

    ReceiveThread = thread(&Client::ReceiveT);
    SendThread = thread(&Client::SendT);

    ReceiveThread.detach();
    SendThread.detach();
}

void Client::Send(const char* sendbuf) {
    
}

void Client::CloseSocket() {
    closesocket(socketClient);
}

char* Client::GetIp() {
    return ip;
}

void Client::ReceiveT(void) {
    Log::Info("�ͻ�����Ϣ��ȡ�С����� : ", GetIp());
    while (true)
    {
        memset(receiveBuf, 0, sizeof(receiveBuf));
        recv(socketClient, receiveBuf, sizeof(receiveBuf), 0);
    }
}

void Client::SendT(void) {
    Log::Info("����ͻ��˷�����Ϣ������ : ", GetIp());
    while (true) {
        if (!queueToSend.empty()) {
            const char* sendbuf = queueToSend.front();
            if (send(socketClient, sendbuf, sizeof(sendbuf), 0) != SOCKET_ERROR) {
                queueToSend.pop();
            }
            else {
                Log::Info("��Ϣ����ʧ�� : ", GetIp());
            }
        }
    }
}
