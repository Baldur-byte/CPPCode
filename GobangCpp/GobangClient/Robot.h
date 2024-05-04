#pragma once
#include "Player.h"
class Robot : public Player{
public:
    Robot(Game* game);
    ~Robot();
    void JoinRoom();
    void ReadyToStartGame() override;
    void GameStart() override;
    void SetChessType(ChessType type, ChessType turn) override;
    void UpdateChessBoard(short chessBoard[15][15], int x, int y) override;
    void Change(ChessType turn) override;
    void GameFinish(int win) override;
    void RestartGameRequest() override;
    void RestartConfirm() override;
private:
    short chessBoard[15][15];

    void tryPlaceChess();
};