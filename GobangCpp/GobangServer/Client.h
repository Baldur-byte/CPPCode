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
	char* id;
	SOCKET socketClient;
private:
	static void SendT();

	Player player;
	queue<const char*> queueToSend;
	thread ReceiveThread;
	thread SendThread;
};