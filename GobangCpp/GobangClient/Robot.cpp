#include "Robot.h"

Robot::Robot() : Player() {
}

Robot::~Robot(){
}

void Robot::JoinGame() {
    JoinRoom_Message message;
    socketClient.Send(CSMessageType::JoinRoom, &message, sizeof(JoinRoom_Message));
}


void Robot::SetChessType(ChessType type, ChessType turn) {
    this->chessType = type;
    this->isTurn = turn == type;
    if (isTurn) {
        Sleep(1 * 1000);
        PlaceChess(6, 7);
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

void Robot::GameFinish(int win) {

}