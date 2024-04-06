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

class Client
{
public:
	Client();
	~Client();
	void Send(SCMessageType type, IMessage* message, size_t len);
	void Receive(MessagePack* message);
	void Start(char* ip, SOCKET socketClient, ServerLogic* serverLogic);
	void CloseSocket();
	char* id;
	SOCKET socketClient;
	queue<MessagePack> queueToSend;
private:
	static void SendT(Client* client);
	static void ReceiveT(Client* client);
	ServerLogic* serverLogic;
	Player player;
	thread ReceiveThread;
	thread SendThread;
	char sendbuf[MESGSIZE];
};
#endif