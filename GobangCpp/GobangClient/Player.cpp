#include "Player.h"
#include "Game.h"

Player::Player() {
    chessType = ChessType::None;
    clientSocket.Start(this);
    isTurn = false;
    chessBoard = nullptr;
}

Player::~Player() {

}

ChessType Player::GetChessType() {
    return chessType;
}

void Player::JoinRoom(ChessBoard* chessBoard) {
    this->chessBoard = chessBoard;
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

void Player::UpdateChessBoard(short(*chessBoardData)[15]) {
    this->chessBoard->SetChessBoardData(chessBoardData);
}

void Player::Change(ChessType turn) {
    this->isTurn = turn == chessType;
}

void Player::GameFinish(int win) {
    chessBoard->SetGameResult(win);
}