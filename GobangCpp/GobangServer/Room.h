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
    void SetID(int id);
    int ID();
    int PlayerCount();
    bool isFull();
    bool AddPlayer(Player* player);
    bool RemovePlayer(Player* player);
    void CheckState();
    void GameStart(ChessType turn);
    void PlaceChess(int x, int y, ChessType type);
    void GameOver();
    short** GetChessBoardData();
    ChessType turn;
private:
    int id;
    int CheckResult();
    ChessBoardCell chessBoard[15][15];
    Player* Players[2];
    int playerCount;
    mutex mtx;
    ChessType FIRST_TO_PLACE = ChessType::Black;
};