#pragma once
#include <winsock2.h>
#include <thread>
#include "Player.h"

#define SERVER_ADDRESS "127.0.0.1"
#define PORT 4444
#define MESGSIZE 1024
#pragma comment(lib, "ws2_32.lib")

using namespace std;

class SocketClient {
public:
    SocketClient();
    ~SocketClient();
    void Start(Player* player);
    void Receive();
    void Send();
    void Close();
    SOCKET socketClient;
private:
    WSADATA wsaDATA;
    char buff[MESGSIZE];
    thread ReceiveThread;
    Player* player;
};