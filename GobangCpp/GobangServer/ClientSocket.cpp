#include "ClientSocket.h"
#include "ServerLogic.h"

void ClientSocket::SendT(ClientSocket* clientSocket) {
    string s = clientSocket->id;
    Log::Info("����ͻ��˷�����Ϣ... : ", const_cast<char*>(s.c_str()));
    char sendbuf[MESGSIZE];
    while (clientSocket->GetState() == ClientState::Connected) {
        if (!clientSocket->queueToSend.empty()) {
            memset(sendbuf, 0, sizeof(sendbuf));
            memcpy(sendbuf, &(clientSocket->queueToSend.front()), sizeof(MessagePack));
            if (send(clientSocket->socketClient, sendbuf, sizeof(sendbuf), 0) != SOCKET_ERROR) {
                clientSocket->queueToSend.pop();
                Log::Info("�ͻ�����Ϣ���ͳɹ�... : ", const_cast<char*>(s.c_str()));
            }
            else {
                Log::Info("��Ϣ����ʧ�� : ", const_cast<char*>(s.c_str()));
            }
        }
    }
    Log::Info("ֹͣ��ͻ��˷�����Ϣ... : ", const_cast<char*>(s.c_str()));
}

void ClientSocket::ReceiveT(ClientSocket* clientSocket) {
    string s = clientSocket->id;
    Log::Info("�ͻ�����Ϣ��ȡ��... : ", const_cast<char*>(s.c_str()));
    char receiveBuf[1024];
    while (clientSocket->GetState() == ClientState::Connected)
    {
        MessagePack data;
        memset(receiveBuf, 0, sizeof(receiveBuf));
        if (recv(clientSocket->socketClient, receiveBuf, sizeof(receiveBuf), 0) != SOCKET_ERROR) {
            memcpy(&data, receiveBuf, sizeof(MessagePack));
            clientSocket->Receive(&data);
        }
    }
    Log::Info("ֹͣ�ͻ�����Ϣ��ȡ... : ", const_cast<char*>(s.c_str()));
}

void ClientSocket::HeartBeatT(ClientSocket* clientSocket) {
    while (clientSocket->GetState() == ClientState::Connected)
    {
        Sleep(100);
        clientSocket->TimePass();
        if (clientSocket->GetHeartBeatInterval() > 600) {
            clientSocket->CloseSocket();
        }
    }
}

ClientSocket::ClientSocket() {
    id = nullptr;
    player = Player(this);
    socketClient = NULL;
    serverLogic = nullptr;
    state = ClientState::DisConnected;
    heartInterval = -1;
    sendbuf;
}

ClientSocket::~ClientSocket()
{
}

void ClientSocket::Start(char* ip, SOCKET socketClient, ServerLogic* serverLogic) {
    this->socketClient = socketClient;
    this->id = ip;
    state = ClientState::Connected;
    heartInterval = 0;
    SendThread = thread(&ClientSocket::SendT, this);
    ReceiveThread = thread(&ClientSocket::ReceiveT, this);
    HeartBeatThread = thread(&ClientSocket::HeartBeatT, this);

    this->serverLogic = serverLogic;

    //ReceiveThread.detach();
    //SendThread.detach();
}

void ClientSocket::Send(SCMessageType type, IMessage* message, size_t len) {
    MessagePack pack;
    pack.type = static_cast<short>(type);
    pack.len = len;
    memcpy(pack.content, message, len);
    /*memset(sendbuf, 0, sizeof(sendbuf));
    memcpy(sendbuf, &pack, sizeof(pack));*/
    /*if (send(socketClient, sendbuf, sizeof(pack), 0) != SOCKET_ERROR) {
        Log::Info("�ͻ�����Ϣ���ͳɹ�... : ", id);
    }*/
    queueToSend.push(pack);
}

void ClientSocket::Receive(MessagePack* message) {
    //CSMessageType type;
    int len = message->len;
    //CSMessageType messageType = toCSMessageType(message->type);
    CSMessageType messageType = static_cast<CSMessageType>(message->type);
    switch (messageType) {
    case CSMessageType::None:
        break;
    case CSMessageType::HeartBeatCS:
        heartInterval = 0;
        Log::Info("�յ������� : ", id);
        HeartBeatSC_Message heartbeat;
        Send(SCMessageType::HeartBeatSC, &heartbeat, sizeof(HeartBeatSC_Message));
        break;
    case CSMessageType::CreatePlayerCS:
        break;
    case CSMessageType::UpdatePlayerInfo:
        break;
    case CSMessageType::GetPlayerInfo:
        break;
    case CSMessageType::GetRoomList:
        break;
    case CSMessageType::GetRoomInfo:
        break;
    case CSMessageType::PlaceChessCS:
        PlaceChessCS_Message placeChessCS_Data;
        memcpy(&placeChessCS_Data, message->content, sizeof(PlaceChessCS_Message));
        player.GetRoom()->PlaceChess(static_cast<int>(placeChessCS_Data.x), static_cast<int>(placeChessCS_Data.y), static_cast<ChessType>(placeChessCS_Data.chess));
        break;
    case CSMessageType::CreateRoom:
        break;
    case CSMessageType::JoinRoom:
        serverLogic->JoinRoom(&player);
        break;
    case CSMessageType::ExitRoom:
        serverLogic->ExitRoom(&player);
        break;
    case CSMessageType::StartGame:
        //player.StartGame();
        break;
    case CSMessageType::QuitGame:
        break;
    case CSMessageType::Update:
        break;
    }
    /*int len = data->textlen;
    string str = data->text;
    str = str.substr(0, len);
    Log::Info("��Ϣ���ճɹ� : ", const_cast<char*>(str.c_str()));*/
}

ClientState ClientSocket::GetState() {
    return state;
}

int ClientSocket::GetHeartBeatInterval() {
    return heartInterval;
}

void ClientSocket::TimePass() {
    heartInterval++;
}

void ClientSocket::CloseSocket() {
    Log::Info("�ͻ��˶Ͽ����� : ", id);
    state = ClientState::DisConnected;

    closesocket(socketClient);
}
