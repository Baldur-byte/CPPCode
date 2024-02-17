#pragma once
#include "SocketClient.h"

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
    void SetChessType(ChessType type);
    void PlaceChess(int x, int y);
private:
    ChessType chessType;
    SocketClient socketClient;
};
