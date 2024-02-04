#pragma once
#include "Player.h"

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

class Game {
public:
    Game();
    ~Game();
    void Start();
    void ChangePlayer();
    bool IsFinished();
    void Click(int x, int y);
    ChessBoardCell** GetChessBoardData();
private:
    Player player;
    ChessBoardCell chessBoard[15][15];
};
