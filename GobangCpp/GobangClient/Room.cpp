#include "Room.h"

Room::Room() {

}

Room::~Room() {

}

void Room::ResetRoom() {
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            chessBoard[i][j] = ChessBoardCell(i, j);
        }
    }
}

void Room::SetChessBoardData(short(*chessBoard)[15]) {
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            this->chessBoard[i][j].cellType = static_cast<CellType>(chessBoard[i][j]);
        }
    }
}

ChessBoardCell** Room::GetChessBoardData() {
    ChessBoardCell** result = new ChessBoardCell * [15];
    for (int i = 0; i < 15; i++) {
        result[i] = chessBoard[i];
    }
    return result;
}


void Room::SetGameResult(int result) {
    if (result == 1) {
        //gameStart->ShowMessage("你赢了");
    }
    else if (result == 0) {
        //gameStart->ShowMessage("平局");
    }
    else if (result == -1) {
        //gameStart->ShowMessage("你输了");
    }
    else {
        //gameStart->ShowMessage("故障");
    }
}