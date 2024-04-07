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

class GameStart;

class Game {
public:
    Game();
    ~Game();
    void Enter(GameStart* gameStart);
    void Start();
    void ChangePlayer();
    bool IsFinished();
    void Click(int x, int y);
    void SetChessBoardData(short(*chessBoard)[15]);
    ChessBoardCell** GetChessBoardData();
    void SetGameResult(int result);
    void Quit();
private:
    GameStart* gameStart;
    Player player;
    ChessBoardCell chessBoard[15][15];

    bool checkHorizontal(int x, int y);
    bool checkVertical(int x, int y);
};
