#ifndef CLIENT_H
#define CLIENT_H

#include <WinSock2.h>
#include <queue>
#include <thread>
#include "Player.h"
#include "Log.h"
#include "IMessage.h"

#define MESGSIZE 1024

using namespace std;

class ServerLogic;

enum class ClientState {
	DisConnected = 0,
	Connected = 1,
};

class ClientSocket
{
public:
	ClientSocket();
	~ClientSocket();
	void Start(char* ip, SOCKET socketClient, ServerLogic* serverLogic);
	void Send(SCMessageType type, IMessage* message, size_t len);
	void Receive(MessagePack* message);
	ClientState GetState();
	int GetHeartBeatInterval();
	void TimePass();
	void CloseSocket();
	char* id;
	SOCKET socketClient;
	queue<MessagePack> queueToSend;
private:
	static void SendT(ClientSocket* client);
	static void ReceiveT(ClientSocket* client);
	static void HeartBeatT(ClientSocket* client);
	ServerLogic* serverLogic;
	Player player;
	thread ReceiveThread;
	thread SendThread;
	thread HeartBeatThread;
	char sendbuf[MESGSIZE];
	ClientState state;
	int heartInterval;
};
#endif