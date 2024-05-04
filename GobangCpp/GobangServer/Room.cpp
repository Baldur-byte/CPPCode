#include "Room.h"

Room::Room() {
    id = 0;
    playerCount = 0;
    turn = ChessType::None;
    Players[0] = nullptr;
    Players[1] = nullptr;
}

Room::~Room() {

}

void Room::SetID(int id) {
    this->id = id;
}

int Room::ID() {
    return id;
}

int Room::PlayerCount() {
    return playerCount;
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

void Room::CheckState() {
    if (playerCount == 2 && Players[0]->isReady && Players[1]->isReady) {
        GameStart(FIRST_TO_PLACE);
    }
}

void Room::GameStart(ChessType turn) {
    Log::Info("游戏开始了");
    ResetChessBoard();
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

void Room::RestartRequest() {
    if (!isFull()) {
        return;
    }
    if (Players[0]->ReadyToRestart && Players[1]->ReadyToRestart) {
        Players[0]->ReadyToRestart = false;
        Players[1]->ReadyToRestart = false;
        GameStart(FIRST_TO_PLACE);
    }
    else if (!Players[0]->ReadyToRestart) {
        Players[0]->RestartConfirm();
    }
    else if (!Players[1]->ReadyToRestart) {
        Players[1]->RestartConfirm();
    }
}

void Room::ResetChessBoard() {
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            chessBoard[i][j] = ChessBoardCell(i, j);
        }
    }
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
        int preChess = 0;
        int chessCount = 0;
        for (int j = 0; j < columns; j++) {
            if (chessBoard[i][j].cellType == targetType) {
                chessCount++;
                if ((chessCount == 4 || preChess + chessCount == 4) && isEmptyHead) {
                    canWinPoses.insert(i * 100 + (j - chessCount));
                }
                else if (chessCount >= 5) {
                    return 1;
                }
            }
            else if (chessBoard[i][j].cellType == CellType::Empty) {
                if (chessCount == 4) {
                    canWinPoses.insert(i * 100 + j);
                }
                preChess = chessCount;
                chessCount = 0;
                isEmptyHead = true;
            }
            else if (chessBoard[i][j].cellType == CellType::None) {
                Log::Info("棋盘信息错误");
                return -1;
            }
            else {
                preChess = 0;
                chessCount = 0;
                isEmptyHead = false;
            }
        }
    }
#pragma endregion
#pragma region 垂直
    for (int j = 0; j < columns; j++) {
        bool isEmptyHead = false;
        int preChess = 0;
        int chessCount = 0;
        for (int i = 0; i < rows; i++) {
            if (chessBoard[i][j].cellType == targetType) {
                chessCount++;
                if ((chessCount == 4 || preChess + chessCount == 4) && isEmptyHead) {
                    canWinPoses.insert((i - chessCount) * 100 + j);
                }
                else if (chessCount >= 5) {
                    return 1;
                }
            }
            else if (chessBoard[i][j].cellType == CellType::Empty) {
                if (chessCount == 4) {
                    canWinPoses.insert(i * 100 + j);
                }
                preChess = chessCount;
                chessCount = 0;
                isEmptyHead = true;
            }
            else if (chessBoard[i][j].cellType == CellType::None) {
                Log::Info("棋盘信息错误");
                return -1;
            }
            else {
                preChess = 0;
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
            int preChess = 0;
            int chessCount = 0;
            for (int k = 0; k < 5; k++) {
                if (chessBoard[i + k][j + k].cellType == targetType) {
                    chessCount++;
                    if ((chessCount == 4 || preChess + chessCount == 4) && isEmptyHead) {
                        canWinPoses.insert((i + k - chessCount) * 100 + j + k);
                    }
                    else if (chessCount >= 5) {
                        return 1;
                    }
                }
                else if (chessBoard[i + k][j + k].cellType == CellType::Empty) {
                    if (chessCount == 4) {
                        canWinPoses.insert((i + k) * 100 + j + k);
                    }
                    preChess = chessCount;
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
            int preChess = 0;
            int chessCount = 0;
            for (int k = 0; k < 5; k++) {
                if (chessBoard[i - k][j + k].cellType == targetType) {
                    chessCount++;
                    if ((chessCount == 4 || preChess + chessCount == 4) && isEmptyHead) {
                        canWinPoses.insert((i - k - chessCount) * 100 + j + k);
                    }
                    else if (chessCount >= 5) {
                        return 1;
                    }
                }
                else if (chessBoard[i - k][j + k].cellType == CellType::Empty) {
                    if (chessCount == 4) {
                        canWinPoses.insert((i - k) * 100 + j + k);
                    }
                    preChess = chessCount;
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