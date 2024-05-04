#include "ClientSocket.h"
#include "Player.h"
#include <ws2tcpip.h>

void ClientSocket::SendT(ClientSocket* clientSocket) {
    char sendbuf[MESGSIZE];
    while (clientSocket->GetState() == ClientState::Connected) {
        if (!clientSocket->queueToSend.empty()) {
            memset(sendbuf, 0, sizeof(sendbuf));
            memcpy(sendbuf, &(clientSocket->queueToSend.front()), sizeof(MessagePack));
            if (send(clientSocket->socketClient, sendbuf, sizeof(sendbuf), 0) != SOCKET_ERROR) {
                clientSocket->queueToSend.pop();
            }
            else {
            }
        }
    }
}

void ClientSocket::ReceiveT(ClientSocket* clientSocket) {
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
}

void ClientSocket::HeartBeatT(ClientSocket* clientSocket) {
    while (clientSocket->GetState() == ClientState::Connected)
    {
        Sleep(100);
        clientSocket->TimePass();
        if (clientSocket->GetHeartBeatInterval() % 100 == 0) {
            HeartBeatCS_Message message;
            clientSocket->Send(CSMessageType::HeartBeatCS, &message, sizeof(message));
        }
        else if (clientSocket->GetHeartBeatInterval() > 600) {
            clientSocket->CloseSocket();
        }
    }
}

ClientSocket::ClientSocket() {
    state = ClientState::DisConnected;
    heartInterval = -1;
    isStarted = false;
}

ClientSocket::~ClientSocket() {

}

void ClientSocket::Receive(MessagePack* message) {
    int len = message->len;
    //CSMessageType messageType = toCSMessageType(message->type);
    SCMessageType messageType = static_cast<SCMessageType>(message->type);
    switch (messageType) {
    case SCMessageType::None:
        break;
    case SCMessageType::HeartBeatSC:
        heartInterval = 0;
        break;
    case SCMessageType::CreatePlayerSC:
        break;
    case SCMessageType::PlayerInfo:
        break;
    case SCMessageType::RoomList:
        RoomList_Message roomList_Data;
        memcpy(&roomList_Data, message->content, sizeof(RoomList_Message));
        player->SetRoomList(roomList_Data.roomList);
        break;
    case SCMessageType::RoomInfo:
        break;
    case SCMessageType::UpdateChessBoard:
        UpdateChessBoard_Message updateChessBoard_Data;
        memcpy(&updateChessBoard_Data, message->content, sizeof(UpdateChessBoard_Message));
        player->UpdateChessBoard(updateChessBoard_Data.chessBoard);
        break;
    case SCMessageType::OperationResult:
        OperationResult_Message operationResult_Date;
        memcpy(&operationResult_Date, message->content, sizeof(OperationResult_Message));
        CSMessageType mesageType;
        mesageType = static_cast<CSMessageType>(operationResult_Date.messageType);
        if (eventMap.count(mesageType) > 0) {
            eventMap[mesageType](player->CurGame());
        }
        break;
    case SCMessageType::GameStart:
        GameStart_Message gameStart_Data;
        memcpy(&gameStart_Data, message->content, sizeof(GameStart_Message));
        player->SetChessType(static_cast<ChessType>(gameStart_Data.chessType), static_cast<ChessType>(gameStart_Data.turn));
        player->GameStart();
        break;
    case SCMessageType::Change:
        Change_Message change_Data;
        memcpy(&change_Data, message->content, sizeof(Change_Message));
        player->Change(static_cast<ChessType>(change_Data.chessType));
        break;
    case SCMessageType::GameFinish:
        GameFinish_Message gameFinish_Data;
        memcpy(&gameFinish_Data, message->content, sizeof(GameFinish_Message));
        player->GameFinish(gameFinish_Data.isWin);
        break;
    case SCMessageType::RestartConfirm:
        RestartConfirm_Message restartConfirm_Data;
        memcpy(&restartConfirm_Data, message->content, sizeof(RestartConfirm_Message));
        player->RestartConfirm();
        break;
    }
    /*int len = data->textlen;
    string str = data->text;
    str = str.substr(0, len);
    Log::Info("消息接收成功 : ", const_cast<char*>(str.c_str()));*/
}

void ClientSocket::Start(Player* player) {
    if (isStarted) {
        return;
    }

    if (WSAStartup(MAKEWORD(2, 2), &wsaDATA) != 0) {
        return;
    }

    SOCKADDR_IN addrServer;
    addrServer.sin_family = AF_INET;
    addrServer.sin_port = htons(PORT);
    //addrServer.sin_addr.S_un.S_addr =  inet_addr(SERVER_ADDRESS);
    inet_pton(AF_INET, SERVER_ADDRESS, &(addrServer.sin_addr));

    socketClient = socket(AF_INET, SOCK_STREAM, 0);
    if (SOCKET_ERROR == socketClient) {
        //string error = WSAGetLastError();
        return;
    }

    if (connect(socketClient, (sockaddr*)&addrServer, sizeof(addrServer)) == INVALID_SOCKET) {
        int error = WSAGetLastError();
        return;
    }

    this->state = ClientState::Connected;
    this->heartInterval = 0;
    this->player = player;
    this->SendThread = thread(&ClientSocket::SendT, this);
    this->ReceiveThread = thread(&ClientSocket::ReceiveT, this);
    this->HeartBeatThread = thread(&ClientSocket::HeartBeatT, this);

    isStarted = true;
}

void ClientSocket::Send(CSMessageType type, IMessage* message, size_t len) {
    MessagePack pack;
    pack.type = static_cast<short>(type);
    pack.len = len;
    memcpy(pack.content, message, len);
    memset(sendbuf, 0, sizeof(sendbuf));
    memcpy(sendbuf, &pack, sizeof(pack));
    if (send(socketClient, sendbuf, sizeof(pack), 0) != SOCKET_ERROR) {
    }
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
    state = ClientState::DisConnected;
    closesocket(socketClient);
    WSACleanup();
}

void ClientSocket::RegistOperatorResultEvent(CSMessageType type, void (*action)(Game*)) {
    eventMap.insert(std::make_pair(type, action));
}

void ClientSocket::UnregistOperatorResultEvent(CSMessageType type) {
    eventMap.erase(type);
}
