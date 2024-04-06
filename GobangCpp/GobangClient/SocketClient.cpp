#include "SocketClient.h"
#include "Player.h"
#include <ws2tcpip.h>

static void ReceiveT(SocketClient* client) {
    char receiveBuf[1024];
    while (true)
    {
        MessagePack data;
        memset(receiveBuf, 0, sizeof(receiveBuf));
        recv(client->socketClient, receiveBuf, sizeof(receiveBuf), 0);

        memcpy(&data, receiveBuf, sizeof(MessagePack));
        client->Receive(&data);
    }
}

SocketClient::SocketClient() {

}

SocketClient::~SocketClient() {

}

void SocketClient::Start(Player* player) {
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
    ReceiveThread = thread(ReceiveT, this);
    this->player = player;
}

void SocketClient::Receive(MessagePack* message) {
    int len = message->len;
    //CSMessageType messageType = toCSMessageType(message->type);
    SCMessageType messageType = static_cast<SCMessageType>(message->type);
    switch (messageType) {
    case SCMessageType::None:
        break;
    case SCMessageType::HeartBeatSC:
        break;
    case SCMessageType::CreatePlayerSC:
        break;
    case SCMessageType::PlayerInfo:
        break;
    case SCMessageType::RoomList:
        break;
    case SCMessageType::RoomInfo:
        break;
    case SCMessageType::PlaceChessSC:
        PlaceChessSC_Message placeChessSC_Data;
        memcpy(&placeChessSC_Data, message->content, sizeof(PlaceChessSC_Message));
        player->UpdateChessBoard(placeChessSC_Data.chessBoard);
        break;
    case SCMessageType::OperationResult:
        break;
    case SCMessageType::GameStart:
        GameStart_Message gameStart_Data;
        memcpy(&gameStart_Data, message->content, sizeof(GameStart_Message));
        player->SetChessType(static_cast<ChessType>(gameStart_Data.chessType), static_cast<ChessType>(gameStart_Data.turn));
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
    }
    /*int len = data->textlen;
    string str = data->text;
    str = str.substr(0, len);
    Log::Info("消息接收成功 : ", const_cast<char*>(str.c_str()));*/
}

void SocketClient::Send(CSMessageType type, IMessage* message, size_t len) {
    MessagePack pack;
    pack.type = static_cast<short>(type);
    pack.len = len;
    memcpy(pack.content, message, len);
    memset(sendbuf, 0, sizeof(sendbuf));
    memcpy(sendbuf, &pack, sizeof(pack));
    if (send(socketClient, sendbuf, sizeof(pack), 0) != SOCKET_ERROR) {
    }
}

void SocketClient::Close() {
    closesocket(socketClient);
    WSACleanup();
}