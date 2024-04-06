#include "Player.h"
#include "Game.h"

Player::Player() {
    chessType = ChessType::None;
    socketClient.Start(this);
    isTurn = false;
    game = nullptr;
}

Player::~Player() {

}

ChessType Player::GetChessType() {
    return chessType;
}

void Player::JoinGame(Game* game) {
    this->game = game;
    JoinRoom_Message message;
    socketClient.Send(CSMessageType::JoinRoom, &message, sizeof(JoinRoom_Message));
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
    socketClient.Send(CSMessageType::PlaceChessCS, &message, sizeof(PlaceChessCS_Message));
    isTurn = false;
}

void Player::UpdateChessBoard(short(*chessBoard)[15]) {
    game->SetChessBoardData(chessBoard);
}

void Player::Change(ChessType turn) {
    this->isTurn = turn == chessType;
}

void Player::GameFinish(int win) {
    game->SetGameResult(win);
}