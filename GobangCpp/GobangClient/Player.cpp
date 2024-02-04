#include "Player.h"

Player::Player() {
    chessType = ChessType::None;
}

Player::~Player() {

}

ChessType Player::GetChessType() {
    return chessType;
}

void Player::SetChessType(ChessType type) {
    this->chessType = type;
}