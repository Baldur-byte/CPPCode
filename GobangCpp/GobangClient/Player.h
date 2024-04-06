#pragma once
#include "SocketClient.h"

class Game;

enum class ChessType {
    None,
    Black,
    White,
};

class Player {
public:
    Player();
    ~Player();
    ChessType GetChessType();
    void JoinGame(Game* game);
    virtual void SetChessType(ChessType type, ChessType turn);
    virtual void PlaceChess(int x, int y);
    virtual void UpdateChessBoard(short(*chessBoard)[15]);
    virtual void Change(ChessType turn);
    bool CanPlaceChess();
    virtual void GameFinish(int win);
    bool isTurn;
protected:
    ChessType chessType;
    SocketClient socketClient;
private:
    Game* game;
};
