#include "Room.h"

Room::Room() {
    playerCount = 0;
    turn = ChessType::None;
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            chessBoard[i][j] = ChessBoardCell(i, j);
        }
    }
    Players[0] = nullptr;
    Players[1] = nullptr;
}

Room::~Room() {

}

bool Room::isFull() {
    return playerCount == 2;
}

bool Room::AddPlayer(Player* player) {
    if (playerCount >= 2) {
        return false;
    }
    mtx.lock();
    Players[playerCount] = player;
    playerCount++;
    Log::Info("玩家加入房间，当前房间人数 ：", playerCount);
    if (playerCount == 1) {
        player->SetChessType(ChessType::Black);
    }
    else if (playerCount == 2) {
        player->SetChessType(ChessType::White);
        GameStart(ChessType::Black);
    }
    mtx.unlock();
    return true;
}

bool Room::RemovePlayer(Player* player) {
    if (playerCount == 0) {
        return false;
    }
    mtx.lock();
    bool isFound = false;
    for (int i = 0; i < playerCount; i++) {
        if (isFound) {
            Players[i - 1] = Players[i];
            continue;
        }
        if (player == Players[i]) {
            isFound = true;
        }
    }
    if (!isFound){
        Log::Info("玩家不在当前房间，无法退出，当前房间人数 ：", playerCount);
        mtx.unlock();
        return false;
    }
    else {
        playerCount--;
        Log::Info("玩家退出房间，当前房间人数 ：", playerCount);
        mtx.unlock();
        return true;
    }
}

void Room::GameStart(ChessType turn) {
    Log::Info("游戏开始了");
    this->turn = turn;
    for (int i = 0; i < playerCount; i++) {
        Players[i]->GameStart(turn);
    }
}

void Room::PlaceChess(int x, int y, ChessType type) {
    if (type == ChessType::Black) {
        Log::Info("黑子下棋");
        chessBoard[x][y].cellType = CellType::Black;
    }
    else if (type == ChessType::White) {
        Log::Info("白子下棋");
        chessBoard[x][y].cellType = CellType::White;
    }
    else {

    }
    for (int i = 0; i < playerCount; i++) {
        Players[i]->UpdateChessBoard();
    }

    int win = CheckResult();

    if (win != -1) {
        for (int i = 0; i < playerCount; i++) {
            Players[i]->GameFinish(win);
        }
    }
    else {
        if (turn == ChessType::White) {
            turn = ChessType::Black;
        }
        else if (turn == ChessType::Black) {
            turn = ChessType::White;
        }
        else {
            Log::Info("棋局暂停");
            turn = ChessType::None;
        }
        for (int i = 0; i < playerCount; i++) {
            Players[i]->ChangePlayer();
        }
    }
}

void Room::GameOver() {

}

short** Room::GetChessBoardData() {
    mtx.lock();
    short** result = new short*[15];
    for (int i = 0; i < 15; i++) {
        result[i] = new short[15];
    }
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            result[i][j] = static_cast<short>(chessBoard[i][j].cellType);
        }
    }
    mtx.unlock();
    return result;
}

int Room::CheckResult() {
    int result = -1;

    CellType targetType = CellType::None;

    if (turn == ChessType::White) {
        targetType = CellType::White;
    }
    else if (turn == ChessType::Black) {
        targetType = CellType::Black;
    }
    else {
        return result;
    }

    int rows = CHESSBOARD_ROWS;
    int columns = CHESSBOARD_COLUMNS;
    set<int> canWinPoses;
    int preWin = 1;
#pragma region 水平
    for (int i = 0; i < rows; i++) {
        bool isEmptyHead = false;
        int chessCount = 0;
        for (int j = 0; j < columns; j++) {
            if (chessBoard[i][j].cellType == targetType) {
                chessCount++;
                if (chessCount == 4 && isEmptyHead) {
                    canWinPoses.insert(i * 100 + (j - 4));
                }
                else if (chessCount >= 5) {
                    return 1;
                }
            }
            else if (chessBoard[i][j].cellType == CellType::Empty) {
                if (chessCount == 4) {
                    canWinPoses.insert(i * 100 + j);
                }
                chessCount = 0;
                isEmptyHead = true;
            }
            else if (chessBoard[i][j].cellType == CellType::None) {
                Log::Info("棋盘信息错误");
                return -1;
            }
            else {
                chessCount = 0;
                isEmptyHead = false;
            }
        }
    }
#pragma endregion
#pragma region 垂直
    for (int j = 0; j < columns; j++) {
        bool isEmptyHead = false;
        int chessCount = 0;
        for (int i = 0; i < rows; i++) {
            if (chessBoard[i][j].cellType == targetType) {
                chessCount++;
                if (chessCount == 4 && isEmptyHead) {
                    canWinPoses.insert((i - 4) * 100 + j);
                }
                else if (chessCount >= 5) {
                    return 1;
                }
            }
            else if (chessBoard[i][j].cellType == CellType::Empty) {
                if (chessCount == 4) {
                    canWinPoses.insert(i * 100 + j);
                }
                chessCount = 0;
                isEmptyHead = true;
            }
            else if (chessBoard[i][j].cellType == CellType::None) {
                Log::Info("棋盘信息错误");
                return -1;
            }
            else {
                chessCount = 0;
                isEmptyHead = false;
            }
        }
    }
#pragma endregion
#pragma region 右斜
    for (int i = 0; i < rows - 4; i++) {
        for (int j = 0; j < columns - 4; j++) {
            bool isEmptyHead = false;
            int chessCount = 0;
            for (int k = 0; k < 5; k++) {
                if (chessBoard[i + k][j + k].cellType == targetType) {
                    chessCount++;
                    if (chessCount == 4 && isEmptyHead) {
                        canWinPoses.insert((i + k - 4) * 100 + j + k);
                    }
                    else if (chessCount >= 5) {
                        return 1;
                    }
                }
                else if (chessBoard[i + k][j + k].cellType == CellType::Empty) {
                    if (chessCount == 4) {
                        canWinPoses.insert((i + k) * 100 + j + k);
                    }
                    chessCount = 0;
                    isEmptyHead = true;
                }
                else if (chessBoard[i + k][j + k].cellType == CellType::None) {
                    Log::Info("棋盘信息错误");
                    return -1;
                }
                else {
                    break;
                }
            }
        }
    }
#pragma endregion
#pragma region 左斜
    for (int i = rows - 1; i >= 4; i--) {
        for (int j = 0; j < columns - 4; j++) {
            bool isEmptyHead = false;
            int chessCount = 0;
            for (int k = 0; k < 5; k++) {
                if (chessBoard[i - k][j + k].cellType == targetType) {
                    chessCount++;
                    if (chessCount == 4 && isEmptyHead) {
                        canWinPoses.insert((i - k - 4) * 100 + j + k);
                    }
                    else if (chessCount >= 5) {
                        return 1;
                    }
                }
                else if (chessBoard[i - k][j + k].cellType == CellType::Empty) {
                    if (chessCount == 4) {
                        canWinPoses.insert((i - k) * 100 + j + k);
                    }
                    chessCount = 0;
                    isEmptyHead = true;
                }
                else if (chessBoard[i - k][j + k].cellType == CellType::None) {
                    Log::Info("棋盘信息错误");
                    return -1;
                }
                else {
                    break;
                }
            }
        }
    }
#pragma endregion
    if (preWin && canWinPoses.size() >= 2) {
        result = 1;
    }
    return result;
}