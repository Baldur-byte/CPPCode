#pragma once

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
private:
    ChessType chessType;
};
