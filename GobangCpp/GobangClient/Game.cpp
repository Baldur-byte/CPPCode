#include "Game.h"
#include "GameStart.h"

Game::Game() {
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            chessBoard[i][j] = ChessBoardCell(i, j);
        }
    }
    gameStart = nullptr;
}

Game::~Game() {
    delete gameStart;
}

void Game::Enter(GameStart* gameStart) {
    this->gameStart = gameStart;
}

void Game::Start() {
    player.JoinRoom(&room);
    gameStart->ChangePage(2);
}

void Game::Click(int x, int y) {
    if (!player.isTurn)
    {
        return;
    }
    if (chessBoard[x][y].cellType != CellType::Empty) {
        return;
    }
    player.PlaceChess(x, y);
}

ChessBoardCell** Game::GetChessBoardData() {
    return room.GetChessBoardData();
}

//void Game::SetChessBoardData(short(*chessBoard)[15]) {
//    for (int i = 0; i < 15; i++) {
//        for (int j = 0; j < 15; j++) {
//            this->chessBoard[i][j].cellType = static_cast<CellType>(chessBoard[i][j]);
//        }
//    }
//    gameStart->DrawChess();
//}
//
//void Game::SetGameResult(int result) {
//    if (result == 1) {
//        gameStart->ShowMessage("你赢了");
//    }
//    else if (result == 0) {
//        gameStart->ShowMessage("平局");
//    }
//    else if (result == -1) {
//        gameStart->ShowMessage("你输了");
//    }
//    else {
//        gameStart->ShowMessage("故障");
//    }
//}

void Game::Quit() {

}
void Game::Restart() {

}

void Game::AddNetworkRobot() {
    robot.JoinGame();
}

void Game::QuitToLobby() {

}

void Game::QuitToStart() {

}
