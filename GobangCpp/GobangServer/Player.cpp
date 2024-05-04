#include "Player.h"
#include "Room.h"
#include "ClientSocket.h"

Player::Player()
{
    this->chessType = ChessType::None;
    this->clientSocket = nullptr;
    this->room = nullptr;
    this->isInRoom = false;
    this->isReady = false;
    this->ReadyToRestart = false;
}

Player::Player(ClientSocket* clientSocket)
{
    this->chessType = ChessType::None;
    this->clientSocket = clientSocket;
    this->room = nullptr;
    this->isInRoom = false;
    this->isReady = false;
    this->ReadyToRestart = false;
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
    clientSocket->Send(SCMessageType::UpdateChessBoard, &message, sizeof(UpdateChessBoard_Message));
}

void Player::GameStart(ChessType type) {
    GameStart_Message message;
    message.chessType = static_cast<int>(this->chessType);
    message.turn = type == this->chessType;
    clientSocket->Send(SCMessageType::GameStart, &message, sizeof(GameStart_Message));
}

void Player::ChangePlayer() {
    Change_Message message;
    message.chessType = static_cast<int>(room->turn);
    clientSocket->Send(SCMessageType::Change, &message, sizeof(Change_Message));
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
    clientSocket->Send(SCMessageType::GameFinish, &message, sizeof(GameFinish_Message));

    this->isReady = false;
}

void Player::RestartConfirm() {
    RestartConfirm_Message message;
    clientSocket->Send(SCMessageType::RestartConfirm, &message, sizeof(RestartConfirm_Message));
}
#pragma endregion

#pragma region Client->Server
void Player::JoinRoom(Room* room) {
    this->room = room;
    this->isInRoom = true;
}

void Player::ExitRoom() {
    this->room = nullptr;
    this->chessType = ChessType::None;
    this->isInRoom = false;
}

void Player::ReadyToStartGame() {
    this->isReady = true;
    if (isInRoom && this->room->isFull()) {
        this->room->CheckState();
    }
}

void Player::QuitGame() {
    clientSocket->CloseSocket();
    this->isReady = false;
}
#pragma endregion

void Player::SetChessType(ChessType type) {
    chessType = type;
}

Room* Player::GetRoom() {
    return this->room;
}

