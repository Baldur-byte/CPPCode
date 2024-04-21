#pragma once
#include <winsock2.h>
#include <queue>
#include <thread>
#include "IMessage.h"

//#define SERVER_ADDRESS "115.236.153.177"
#define SERVER_ADDRESS "127.0.0.1"
//#define PORT 22965
#define PORT 8888
#define MESGSIZE 1024
#pragma comment(lib, "ws2_32.lib")

using namespace std;

class Player;

enum class ClientState {
    DisConnected = 0,
    Connected = 1,
};

class ClientSocket {
public:
    ClientSocket();
    ~ClientSocket();
    void Start(Player* player);
    void Receive(MessagePack* message);
    void Send(CSMessageType type, IMessage* message, size_t len);
    ClientState GetState();
    int GetHeartBeatInterval();
    void TimePass();
    void CloseSocket();
    SOCKET socketClient;
    queue<MessagePack> queueToSend;
private:
    static void SendT(ClientSocket* client);
    static void ReceiveT(ClientSocket* client);
    static void HeartBeatT(ClientSocket* client);
    WSADATA wsaDATA;
    char sendbuf [MESGSIZE];
    thread ReceiveThread;
    thread SendThread;
    thread HeartBeatThread;
    Player* player;
    ClientState state;
    int heartInterval;
};