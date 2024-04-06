#pragma once
#include "Player.h"
#include "Log.h"
#include <mutex>
#include <set>

#define CHESSBOARD_ROWS 15
#define CHESSBOARD_COLUMNS 15

enum class CellType {
    None,
    Empty,
    Black,
    White,
};

struct ChessBoardCell {
    int x;
    int y;
    CellType cellType;

    ChessBoardCell() {
        this->x = -1;
        this->y = -1;
        this->cellType = CellType::None;
    }

    ChessBoardCell(int x, int y) {
        this->x = x;
        this->y = y;
        this->cellType = CellType::Empty;
    }
};

class Room {
public:
    Room();
    ~Room();
    bool isFull();
    bool AddPlayer(Player* player);
    bool RemovePlayer(Player* player);
    void GameStart(ChessType turn);
    void PlaceChess(int x, int y, ChessType type);
    void GameOver();
    short** GetChessBoardData();
    ChessType turn;
private:
    int CheckResult();
    ChessBoardCell chessBoard[15][15];
    Player* Players[2];
    int playerCount;
    mutex mtx;
};