#include "ClientSocket.h"
#include "ServerLogic.h"

void ClientSocket::SendT(ClientSocket* clientSocket) {
    string s = clientSocket->id;
    Log::Info("可向客户端发送信息... : ", const_cast<char*>(s.c_str()));
    char sendbuf[MESGSIZE];
    while (clientSocket->GetState() == ClientState::Connected) {
        if (!clientSocket->queueToSend.empty()) {
            memset(sendbuf, 0, sizeof(sendbuf));
            memcpy(sendbuf, &(clientSocket->queueToSend.front()), sizeof(MessagePack));
            if (send(clientSocket->socketClient, sendbuf, sizeof(sendbuf), 0) != SOCKET_ERROR) {
                clientSocket->queueToSend.pop();
                Log::Info("客户端信息发送成功... : ", const_cast<char*>(s.c_str()));
            }
            else {
                Log::Info("消息发送失败 : ", const_cast<char*>(s.c_str()));
            }
        }
    }
    Log::Info("停止向客户端发送信息... : ", const_cast<char*>(s.c_str()));
}

void ClientSocket::ReceiveT(ClientSocket* clientSocket) {
    string s = clientSocket->id;
    Log::Info("客户端信息收取中... : ", const_cast<char*>(s.c_str()));
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
    Log::Info("停止客户端信息收取... : ", const_cast<char*>(s.c_str()));
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
        Log::Info("客户端信息发送成功... : ", id);
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
        Log::Info("收到心跳包 : ", id);
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
        //GetRoomList_Message getRoomList_Data;
        //memcpy(&getRoomList_Data, message->content, sizeof(PlaceChessCS_Message));
        //player.GetRoom()->PlaceChess(static_cast<int>(placeChessCS_Data.x), static_cast<int>(placeChessCS_Data.y), static_cast<ChessType>(placeChessCS_Data.chess));

        RoomList_Message roomList_message;
        for (int i = 0; i < 12; i++) {
            roomList_message.roomList[i][0] = serverLogic->RoomList()[i]->ID();
            roomList_message.roomList[i][1] = serverLogic->RoomList()[i]->PlayerCount();
        }
        Send(SCMessageType::RoomList, &roomList_message, sizeof(RoomList_Message));
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
        JoinRoom_Message joinRoom_Data;
        memcpy(&joinRoom_Data, message->content, sizeof(JoinRoom_Message));

        OperationResult_Message operationResult_Message;
        operationResult_Message.messageType = static_cast<int>(CSMessageType::JoinRoom);
        operationResult_Message.result = serverLogic->JoinRoom(joinRoom_Data.roomId, &player) ? 0 : 1;
        Send(SCMessageType::OperationResult, &operationResult_Message, sizeof(OperationResult_Message));
        break;
    case CSMessageType::ExitRoom:
        serverLogic->ExitRoom(&player);
        break;
    case CSMessageType::ReadyToStartGame:
        player.ReadyToStartGame();
        break;
    case CSMessageType::QuitGame:
        break;
    case CSMessageType::Update:
        break;
    case CSMessageType::RestartRequest:
        player.ReadyToRestart = true;
        player.GetRoom()->RestartRequest();
        break;
    }
    /*int len = data->textlen;
    string str = data->text;
    str = str.substr(0, len);
    Log::Info("消息接收成功 : ", const_cast<char*>(str.c_str()));*/
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
    Log::Info("客户端断开连接 : ", id);
    state = ClientState::DisConnected;
    if (player.isInRoom) {
        serverLogic->ExitRoom(&player);
    }
    closesocket(socketClient);
}
