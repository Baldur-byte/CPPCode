#pragma once
#include "ClientSocket.h"

class ChessBoard;

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
    void JoinRoom(ChessBoard* room);
    virtual void SetChessType(ChessType type, ChessType turn);
    virtual void PlaceChess(int x, int y);
    virtual void UpdateChessBoard(short(*chessBoard)[15]);
    virtual void Change(ChessType turn);
    virtual void GameFinish(int win);
    bool isTurn;
protected:
    ChessType chessType;
    ClientSocket clientSocket;
private:
    ChessBoard* chessBoard;
};
