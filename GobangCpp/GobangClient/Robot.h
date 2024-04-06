#pragma once
#include "Player.h"
class Robot : public Player{
public:
    Robot();
    ~Robot();
    void JoinGame();
    void SetChessType(ChessType type, ChessType turn) override;
    void UpdateChessBoard(short chessBoard[15][15]) override;
    void Change(ChessType turn) override;
    void GameFinish(int win) override;
private:
    short chessBoard[15][15];
};