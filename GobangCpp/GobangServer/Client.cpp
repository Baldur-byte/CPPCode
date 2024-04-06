#include "Client.h"
#include "ServerLogic.h"

void Client::ReceiveT(Client* client) {
    string s = client->id;
    Log::Info("客户端信息收取中... : ", const_cast<char*>(s.c_str()));
    char receiveBuf[1024];
    while (client)
    {
        MessagePack data;
        memset(receiveBuf, 0, sizeof(receiveBuf));
        recv(client->socketClient, receiveBuf, sizeof(receiveBuf), 0);

        memcpy(&data, receiveBuf, sizeof(MessagePack));
        client->Receive(&data);
    }
    Log::Info("客户端断开连接 : ", const_cast<char*>(s.c_str()));
}

void Client::SendT(Client* client) {
    string s = client->id;
    Log::Info("可向客户端发送信息... : ", const_cast<char*>(s.c_str()));
    char sendbuf[MESGSIZE];
    while (client) {
        if (!client->queueToSend.empty()) {
            memset(sendbuf, 0, sizeof(sendbuf));
            memcpy(sendbuf, &(client->queueToSend.front()), sizeof(MessagePack));
            if (send(client->socketClient, sendbuf, sizeof(sendbuf), 0) != SOCKET_ERROR) {
                client->queueToSend.pop();
                Log::Info("客户端信息发送成功... : ", const_cast<char*>(s.c_str()));
            }
            else {
                Log::Info("消息发送失败 : ", const_cast<char*>(s.c_str()));
            }
        }
    }
}

Client::Client() {
    id = nullptr;
    player = Player(this);
    socketClient = NULL;
    serverLogic = nullptr;
}

Client::~Client()
{
}

void Client::Start(char* ip, SOCKET socketClient, ServerLogic* serverLogic) {
    this->socketClient = socketClient;
    this->id = ip;

    ReceiveThread = thread(&Client::ReceiveT, this);
    SendThread = thread(&Client::SendT, this);

    this->serverLogic = serverLogic;

    //ReceiveThread.detach();
    //SendThread.detach();
}

void Client::Send(SCMessageType type, IMessage* message, size_t len) {
    MessagePack pack;
    pack.type = static_cast<short>(type);
    pack.len = len;
    memcpy(pack.content, message, len);
    /*memset(sendbuf, 0, sizeof(sendbuf));
    memcpy(sendbuf, &pack, sizeof(pack));
    if (send(socketClient, sendbuf, sizeof(pack), 0) != SOCKET_ERROR) {
        Log::Info("客户端信息发送成功... : ", id);
    }*/
    queueToSend.push(pack);
}

void Client::Receive(MessagePack* message) {
    //CSMessageType type;
    int len = message->len;
    //CSMessageType messageType = toCSMessageType(message->type);
    CSMessageType messageType = static_cast<CSMessageType>(message->type);
    switch (messageType) {
    case CSMessageType::None:
        break;
    case CSMessageType::HeartBeatCS:
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
        PlaceChessCS_Message data;
        memcpy(&data, message->content, sizeof(PlaceChessCS_Message));
        player.GetRoom()->PlaceChess(static_cast<int>(data.x), static_cast<int>(data.y), static_cast<ChessType>(data.chess));
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
    Log::Info("消息接收成功 : ", const_cast<char*>(str.c_str()));*/
}

void Client::CloseSocket() {
    closesocket(socketClient);
}
