#include "Player.h"
#include "Game.h"

Player::Player(Game* game) {
    chessType = ChessType::None;
    isTurn = false;
    chessBoard = nullptr;
    curGame = game;
    roomId = 0;
    isInRoom = false;
}

Player::~Player() {

}

void Player::Start() {
    clientSocket.Start(this);
}

bool Player::IsInRoom() {
    return isInRoom;
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

void Player::SetRoomList(short(*roomList)[2]) {
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
    isInRoom = true;
}

void Player::ExitRoom() {
    if (!isInRoom) {
        return;
    }
    this->roomId = 0;
    chessBoard = nullptr;
    ExitRoom_Message message;
    clientSocket.Send(CSMessageType::ExitRoom, &message, sizeof(ExitRoom_Message));
    isInRoom = false;
}

void Player::ReadyToStartGame() {
    ReadyToStartGame_Message message;
    clientSocket.Send(CSMessageType::ReadyToStartGame, &message, sizeof(ReadyToStartGame_Message));
    chessBoard->ResetChessBoard();
    curGame->ShowMessage("准备开始");
}

void Player::SetChessType(ChessType type, ChessType turn) {
    this->chessType = type;
    this->isTurn = turn == type;
}

void Player::GameStart() {
    curGame->ShowMessage("游戏开始");
    curGame->ResetChessBoard();
}

void Player::PlaceChess(int x, int y) {
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
    if (isTurn) {
        curGame->ShowMessage("轮到你下");
    }
    else {
        curGame->ShowMessage("轮到对方下");
    }
}

void Player::GameFinish(int win) {
    curGame->ShowResult(win);
}

void Player::RestartGameRequest() {
    RestartRequest_Message message;
    clientSocket.Send(CSMessageType::RestartRequest, &message, sizeof(RestartRequest_Message));
}

void Player::RestartConfirm() {
    curGame->ShowMessage("对方请求重新开始...");
}

void Player::RegistOperatorResultEvent(CSMessageType type, void (*action)(Game*)) {
    clientSocket.RegistOperatorResultEvent(type, action);
}

void Player::UnregistOperatorResultEvent(CSMessageType type) {
    clientSocket.UnregistOperatorResultEvent(type);
}
