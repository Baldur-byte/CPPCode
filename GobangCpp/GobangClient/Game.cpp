#include "Game.h"
#include "GameStart.h"

Game::Game() {
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            chessBoard[i][j] = ChessBoardCell(i, j);
        }
    }
}

Game::~Game() {

}

void Game::Start(GameStart* gameStart) {
    this->gameStart = gameStart;
    player.JoinGame(this);
}

void Game::ChangePlayer() {

}

bool Game::IsFinished() {
    return false;
}

void Game::Click(int x, int y) {
    if (!player.isTurn)
    {
        return;
    }
    if (chessBoard[x][y].cellType != CellType::Empty) {
        return;
    }
    /*if (player.GetChessType() == ChessType::Black) {
        chessBoard[x][y].cellType = CellType::Black;
    }
    else if (player.GetChessType() == ChessType::White) {
        chessBoard[x][y].cellType = CellType::White;
    }*/
    player.PlaceChess(x, y);
}

ChessBoardCell** Game::GetChessBoardData() {
    ChessBoardCell** result = new ChessBoardCell*[15];
    for (int i = 0; i < 15; i++) {
        result[i] = chessBoard[i];
    }
    return result;
}

void Game::SetChessBoardData(short(*chessBoard)[15]) {
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            this->chessBoard[i][j].cellType = static_cast<CellType>(chessBoard[i][j]);
        }
    }
    gameStart->DrawChess();
}

bool Game::checkHorizontal(int x, int y)
{
    return false;
}

bool Game::checkVertical(int x, int y)
{
    return false;
}

void Game::SetGameResult(int result) {
    if (result == 1) {
        gameStart->ShowMessage("你赢了");
    }
    else if (result == 0) {
        gameStart->ShowMessage("平局");
    }
    else if (result == -1) {
        gameStart->ShowMessage("你输了");
    }
    else {
        gameStart->ShowMessage("故障");
    }
}
