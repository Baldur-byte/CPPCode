#pragma once
#include <winsock2.h>

#define SERVER_ADDRESS "127.0.0.1"
#define PORT 4444
#define MESGSIZE 1024
#pragma comment(lib, "ws2_32.lib")

class SocketClient {
public:
    SocketClient();
    ~SocketClient();
    void Start();
    void Receive();
    void Send();
    void Close();
private:
    WSADATA wsaDATA;
    char buff[MESGSIZE];
    SOCKET socketClient;
};