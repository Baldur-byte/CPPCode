#pragma once

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

class ChessBoard {
public:
    ChessBoard();
    ~ChessBoard();
    void ResetChessBoard();
    void SetChessBoardData(short(*chessBoard)[15]);
    ChessBoardCell** GetChessBoardData();
    bool CheckPosIsEmpty(int x, int y);
private:
    ChessBoardCell chessBoard[15][15];
};