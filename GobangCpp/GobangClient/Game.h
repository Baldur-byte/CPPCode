#pragma once
#include "Player.h"
#include "Robot.h"
#include "Room.h"

//enum class CellType {
//    None,
//    Empty,
//    Black,
//    White,
//};
//
//struct ChessBoardCell {
//    int x;
//    int y;
//    CellType cellType;
//
//    ChessBoardCell() {
//        this->x = -1;
//        this->y = -1;
//        this->cellType = CellType::None;
//    }
//
//    ChessBoardCell(int x, int y) {
//        this->x = x;
//        this->y = y;
//        this->cellType = CellType::Empty;
//    }
//};

class GameStart;

class Game {
public:
    Game();
    ~Game();
    void Enter(GameStart* gameStart);
    void Start();
    void Click(int x, int y);
    void SetChessBoardData(short(*chessBoard)[15]);
    ChessBoardCell** GetChessBoardData();
    void SetGameResult(int result);
    void Quit();

    void Restart();
    void AddNetworkRobot();
    void QuitToLobby();
    void QuitToStart();
private:
    GameStart* gameStart;
    Player player;
    Robot robot;
    Room room;
    ChessBoardCell chessBoard[15][15];
};
