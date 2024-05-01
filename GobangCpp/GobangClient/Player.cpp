#include "Player.h"
#include "Game.h"

Player::Player(Game* game) {
    chessType = ChessType::None;
    isTurn = false;
    chessBoard = nullptr;
    curGame = game;
    roomId = 0;
}

Player::~Player() {

}

void Player::Start() {
    clientSocket.Start(this);
}

int Player::RoomID() {
    return roomId;
}

Game* Player::CurGame() {
    return curGame;
}

ChessType Player::GetChessType() {
    return chessType;
}

void Player::SetRoomList(int(*roomList)[2]) {
    curGame->ReceiveRoomList(roomList);
}

void Player::GetRoomList() {
    GetRoomList_Message message;
    clientSocket.Send(CSMessageType::GetRoomList, &message, sizeof(GetRoomList_Message));
}

void Player::JoinRoom(int roomId, ChessBoard* chessBoard) {
    this->chessBoard = chessBoard;
    this->roomId = roomId;
    JoinRoom_Message message;
    message.roomId = roomId;
    clientSocket.Send(CSMessageType::JoinRoom, &message, sizeof(JoinRoom_Message));
}

void Player::ReadyToStartGame() {
    ReadyToStartGame_Message message;
    clientSocket.Send(CSMessageType::ReadyToStartGame, &message, sizeof(ReadyToStartGame_Message));
    chessBoard->ResetChessBoard();
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
    curGame->UpdateChessBoard();
}

void Player::Change(ChessType turn) {
    this->isTurn = turn == chessType;
}

void Player::GameFinish(int win) {
    chessBoard->SetGameResult(win);
}

void Player::RegistOperatorResultEvent(CSMessageType type, void (*action)(Game*)) {
    clientSocket.RegistOperatorResultEvent(type, action);
}

void Player::UnregistOperatorResultEvent(CSMessageType type) {
    clientSocket.UnregistOperatorResultEvent(type);
}
