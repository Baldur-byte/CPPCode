#include "Player.h"

Player::Player() {
    chessType = ChessType::None;
    socketClient.Start(this);
}

Player::~Player() {

}

ChessType Player::GetChessType() {
    return chessType;
}

void Player::SetChessType(ChessType type) {
    this->chessType = type;
}

void Player::PlaceChess(int x, int y)
{
    socketClient.Send();
}