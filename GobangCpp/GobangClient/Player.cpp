#include "Player.h"
#include "Game.h"

Player::Player() {
    chessType = ChessType::None;
    clientSocket.Start(this);
    isTurn = false;
    room = nullptr;
}

Player::~Player() {

}

ChessType Player::GetChessType() {
    return chessType;
}

void Player::JoinRoom(Room* room) {
    this->room = room;
    JoinRoom_Message message;
    clientSocket.Send(CSMessageType::JoinRoom, &message, sizeof(JoinRoom_Message));
}

void Player::SetChessType(ChessType type, ChessType turn) {
    this->chessType = type;
    this->isTurn = turn == type;
}

void Player::PlaceChess(int x, int y)
{
    PlaceChessCS_Message message;
    message.x = x;
    message.y = y;
    message.chess = static_cast<short>(chessType);
    clientSocket.Send(CSMessageType::PlaceChessCS, &message, sizeof(PlaceChessCS_Message));
    isTurn = false;
}

void Player::UpdateChessBoard(short(*chessBoard)[15]) {
    room->SetChessBoardData(chessBoard);
}

void Player::Change(ChessType turn) {
    this->isTurn = turn == chessType;
}

void Player::GameFinish(int win) {
    room->SetGameResult(win);
}