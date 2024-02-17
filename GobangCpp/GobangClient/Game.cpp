#include "Game.h"

Game::Game() {
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            chessBoard[i][j] = ChessBoardCell(i, j);
        }
    }
}

Game::~Game() {

}

void Game::Start() {
    player.SetChessType(ChessType::Black);
    isTurn = true;
}

void Game::ChangePlayer() {

}

bool Game::IsFinished() {
    return false;
}

void Game::Click(int x, int y) {
    if (!isTurn)
    {
        return;
    }
    if (player.GetChessType() == ChessType::Black) {
        chessBoard[x][y].cellType = CellType::Black;
    }
    else if (player.GetChessType() == ChessType::White) {
        chessBoard[x][y].cellType = CellType::White;
    }
    player.PlaceChess(x, y);
    isTurn = false;
}

ChessBoardCell** Game::GetChessBoardData() {
    ChessBoardCell** result = new ChessBoardCell*[15];
    for (int i = 0; i < 15; i++) {
        result[i] = chessBoard[i];
    }
    return result;
}

bool Game::checkHorizontal(int x, int y)
{

    return false;
}

bool Game::checkVertical(int x, int y)
{
    return false;
}
