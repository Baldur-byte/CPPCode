#pragma once
#include <WinSock2.h>
#include <queue>
#include <thread>
#include "Player.h"
#include "Log.h"

using namespace std;

class Client
{
public:
	Client();
	~Client();
	void Send(const char* sendbuf);
	void Start(char* ip, SOCKET socketClient);
	void CloseSocket();
	char* GetIp();
private:
	void ReceiveT();
	void SendT();

	char* ip;
	SOCKET socketClient;
	Player player;
	char receiveBuf[1024];
	queue<const char*> queueToSend;
	thread ReceiveThread;
	thread SendThread;
};