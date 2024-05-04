#include "Robot.h"
#include "Game.h"

Robot::Robot(Game* game) : Player(game) {
}

Robot::~Robot(){
}

void Robot::JoinRoom() {
    JoinRoom_Message message;
    message.roomId = this->CurGame()->RoomID();
    Sleep(1 * 1000);
    clientSocket.Send(CSMessageType::JoinRoom, &message, sizeof(JoinRoom_Message));
    isInRoom = true;
}

void Robot::ReadyToStartGame() {
    ReadyToStartGame_Message message;
    clientSocket.Send(CSMessageType::ReadyToStartGame, &message, sizeof(ReadyToStartGame_Message));
    GameStart();
}

void Robot::GameStart() {
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            this->chessBoard[i][j] = 1;
        }
    }
    if (isTurn) {
        tryPlaceChess();
    }
}

void Robot::SetChessType(ChessType type, ChessType turn) {
    this->chessType = type;
    this->isTurn = turn == type;
    if (isTurn) {
        tryPlaceChess();
    }
}

void Robot::UpdateChessBoard(short chessBoard[15][15]) {
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            this->chessBoard[i][j] = chessBoard[i][j];
        }
    }
}

void Robot::Change(ChessType turn) {
    if (turn != chessType) {
        return;
    }
    tryPlaceChess();
}

void Robot::GameFinish(int win) {

}

void Robot::RestartGameRequest() {
    RestartRequest_Message message;
    clientSocket.Send(CSMessageType::RestartRequest, &message, sizeof(RestartRequest_Message));
}

void Robot::RestartConfirm() {
    RestartGameRequest();
}

void Robot::tryPlaceChess() {
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            if (chessBoard[i][j] == 1) {
                Sleep(1 * 1000);
                PlaceChess(i, j);
                return;
            }
        }
    }
}