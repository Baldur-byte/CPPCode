#include "Player.h"
#include "Room.h"
#include "Client.h"

Player::Player()
{
    chessType = ChessType::None;
    client = nullptr;
    room = nullptr;
    isInRoom = false;
}

Player::Player(Client* clientSocket)
{
    chessType = ChessType::None;
    client = clientSocket;
    room = nullptr;
    isInRoom = false;
}

Player::~Player()
{
}
#pragma region Server->Client


void Player::UpdateChessBoard() {
    UpdateChessBoard_Message message;
    short** chessboard = room->GetChessBoardData();
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            message.chessBoard[i][j] = chessboard[i][j];
        }
    }
    client->Send(SCMessageType::UpdateChessBoard, &message, sizeof(UpdateChessBoard_Message));
}

void Player::GameStart(ChessType type) {
    GameStart_Message message;
    message.chessType = static_cast<int>(this->chessType);
    message.turn = type == this->chessType;
    client->Send(SCMessageType::GameStart, &message, sizeof(GameStart_Message));
}
void Player::ChangePlayer() {
    Change_Message message;
    message.chessType = static_cast<int>(room->turn);
    client->Send(SCMessageType::Change, &message, sizeof(Change_Message));
}

void Player::GameFinish(int win) {
    GameFinish_Message message;
    if (win == 0) {
        message.isWin = 0;
    }
    else {
        if (room->turn == this->chessType) {
            message.isWin = 1;
        }
        else {
            message.isWin = -1;
        }
    }
    client->Send(SCMessageType::GameFinish, &message, sizeof(GameFinish_Message));
}
#pragma endregion

#pragma region Client->Server
void Player::JoinRoom(Room* room) {
    this->room = room;
}

void Player::ExitRoom() {
    this->room = nullptr;
    chessType = ChessType::None;
}

void Player::StartGame() {

}

void Player::QuitGame() {
    client->CloseSocket();
}
#pragma endregion

void Player::SetChessType(ChessType type) {
    chessType = type;
}

Room* Player::GetRoom() {
    return this->room;
}

