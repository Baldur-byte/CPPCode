#pragma once
#include <winsock2.h>
#include <thread>
#include "IMessage.h"

#define SERVER_ADDRESS "127.0.0.1"
#define PORT 8888
#define MESGSIZE 1024
#pragma comment(lib, "ws2_32.lib")

using namespace std;

class Player;

class SocketClient {
public:
    SocketClient();
    ~SocketClient();
    void Start(Player* player);
    void Receive(MessagePack* message);
    void Send(CSMessageType type, IMessage* message, size_t len);
    void Close();
    SOCKET socketClient;
private:
    WSADATA wsaDATA;
    char sendbuf [MESGSIZE];
    thread ReceiveThread;
    Player* player;
};