#include <iostream>
#include <string>
#include <queue>
#include <graphics.h>
#include <random>
#include "resource.h"
#include <winres.h>

using namespace std;

#define ROW 9
#define COLUMN 9

int offset_x[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
int offset_y[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
IMAGE block_4[16];
IMAGE block_2[16];
IMAGE num_4[12];
IMAGE num_2[12];
IMAGE emotion_4[5];
IMAGE emotion_2[5];

enum class CellType {
    None,
    Empty,
    Num,
    Bomb,
};

enum class CellState {
    None,
    Lock,
    Unlock,
};

enum class CellTag {
    None,
    Empty,
    Flag,
    Doubt,
};

struct DrawParam {
    float cellSize;
    float titleHeight;
    float badge;
    float space;
    int row;
    int column;
    int bomb;

    DrawParam(float cellSize, float titleHeight, float badge, float space, int row, int column, int bomb) {
        this->cellSize = cellSize;
        this->titleHeight = titleHeight;
        this->badge = badge;
        this->space = space;
        this->row = row;
        this->column = column;
        this->bomb = bomb;
    }
};

struct BoardCell {
    CellType cellType;
    CellState cellState;
    CellTag cellTag;
    int count;

    BoardCell() {
        this->cellType = CellType::None;
        this->cellState == CellState::None;
        this->cellTag = CellTag::None;
        this->count = 0;
    }

    BoardCell(CellType cellType, CellState cellState, CellTag cellTag, int count) {
        this->cellType = cellType;
        this->cellState == cellState;
        this->cellTag = cellTag;
        this->count = count;
    }
};

class Board {
public:
    Board(int row, int column, int bomb);
    ~Board();
    void Click(int pos_x, int pos_y, int type);
    int Result();
    BoardCell** GetData();
private:
    BoardCell Data[100][100];
    int row;
    int column;
    int clearCount;
    int bomb;
    int result; //0 没有结果   1 失败   2 成功
};

Board::Board(int row = 9, int column = 9, int bomb = 10) {
    this->row = row;
    this->column = column;
    this->result = 0;
    this->clearCount = 0;
    this->bomb = bomb;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            Data[i][j] = BoardCell(CellType::Empty, CellState::Lock, CellTag::Empty, 0);
        }
    }

    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_int_distribution<int> random_row(0, row - 1);
    std::uniform_int_distribution<int> random_column(0, column - 1);

    for (int i = 0; i < bomb; i++) {
        int random_x = random_row(eng);
        int random_y = random_column(eng);
        if (Data[random_x][random_y].cellType == CellType::Bomb || Data[random_x][random_y].cellType == CellType::None) {
            i--;
        }
        else if ((Data[random_x][random_y].cellType == CellType::Empty) || (Data[random_x][random_y].cellType == CellType::Num)) {
            Data[random_x][random_y].cellType = CellType::Bomb;
            Data[random_x][random_y].count = 0;
            for (int i = 0; i < 8; i++) {
                int temp_x = random_x + offset_x[i];
                int temp_y = random_y + offset_y[i];
                if (temp_x < 0 || temp_x >= row || temp_y < 0 || temp_y >= column || Data[temp_x][temp_y].cellType == CellType::Bomb || Data[temp_x][temp_y].cellType == CellType::None) {
                    continue;
                }
                if (Data[temp_x][temp_y].cellType == CellType::Empty) {
                    Data[temp_x][temp_y].cellType = CellType::Num;
                    Data[temp_x][temp_y].count = 0;
                }
                Data[temp_x][temp_y].count++;
            }
        }
    }
}

Board::~Board() {
    delete[] Data;
}

void Board::Click(int pos_x, int pos_y, int type) {
    if (Data[pos_x][pos_y].cellState == CellState::Unlock) {
        return;
    }
    if (type == 0) {
        if (Data[pos_x][pos_y].cellTag == CellTag::Flag || Data[pos_x][pos_y].cellTag == CellTag::Doubt) {
            return;
        }
        Data[pos_x][pos_y].cellState = CellState::Unlock;
        clearCount++;
        if (Data[pos_x][pos_y].cellType == CellType::Empty) {
            queue<pair<int, int>> q;
            q.push(pair<int, int>(pos_x, pos_y));
            while (!q.empty())
            {
                pair<int, int> pos = q.front();
                q.pop();
                for (int i = 0; i < 8; i++) {
                    int temp_x = pos.first + offset_x[i];
                    int temp_y = pos.second + offset_y[i];
                    if (temp_x < 0 || temp_x >= row || temp_y < 0 || temp_y >= column || Data[temp_x][temp_y].cellType == CellType::None || Data[temp_x][temp_y].cellState == CellState::Unlock) {
                        continue;
                    }
                    if (Data[temp_x][temp_y].cellType == CellType::Empty) {
                        q.push(pair<int, int>(temp_x, temp_y));
                    }
                    Data[temp_x][temp_y].cellState = CellState::Unlock;
                    clearCount++;
                }
            }
        }
        else if(Data[pos_x][pos_y].cellType == CellType::Bomb){
            result = 1;
            return;
        }
        if (clearCount == row * column - bomb) {
            result = 2;
        }
    }
    else {
        if (Data[pos_x][pos_y].cellTag == CellTag::Empty) {
            Data[pos_x][pos_y].cellTag = CellTag::Flag;
        }
        else if (Data[pos_x][pos_y].cellTag == CellTag::Flag) {
            Data[pos_x][pos_y].cellTag = CellTag::Doubt;
        }
        else {
            Data[pos_x][pos_y].cellTag = CellTag::Empty;
        }
    }
}

int Board::Result() {
    return result;
}

BoardCell** Board::GetData() {
    BoardCell** data = new BoardCell*[row];
    for (int i = 0; i < row; i++) {
        data[i] = Data[i];
    }
    return data;
}

pair<int, int> GetPos(float cellSize, float titleHeight, float badge, float space, int row, int column, int x, int y) {
    pair<int, int> result(-1, -1);
    for (int i = 0; i < row; i++) {
        int top = titleHeight + badge + (i + 1) * space + i * cellSize;
        int bottom = titleHeight + badge + (i + 1) * (space + cellSize);
        if (y < top || y > bottom) {
            continue;
        }
        result.first = i;
        for (int j = 0; j < column; j++) {
            int left = badge + (j + 1) * space + j * cellSize;
            int right = badge + (j + 1) * (space + cellSize);
            if (x > left && x < right) {
                result.second = j;
                return result;
            }
        }
    }
    return result;
}

void loadBitmaps(int cellSize) {
    int block_width = cellSize;
    int block_height = cellSize * 16;

    IMAGE img1;
    loadimage(&img1, _T("IMAGE"), MAKEINTRESOURCEW(IDB_BITMAP1), block_width, block_height, true);
    IMAGE img2;
    loadimage(&img2, _T("IMAGE"), MAKEINTRESOURCEW(IDB_BITMAP2));
    IMAGE img3;
    loadimage(&img3, _T("IMAGE"), MAKEINTRESOURCEW(IDB_BITMAP3));
    IMAGE img4;
    loadimage(&img4, _T("IMAGE"), MAKEINTRESOURCEW(IDB_BITMAP4));
    IMAGE img5;
    loadimage(&img5, _T("IMAGE"), MAKEINTRESOURCEW(IDB_BITMAP5));
    IMAGE img6;
    loadimage(&img6, _T("IMAGE"), MAKEINTRESOURCEW(IDB_BITMAP6));

    IMAGE temp;

    SetWorkingImage(&img1);
    for (int i = 0; i < 16; i++) {
        int srcWidth = 16;
        int srcHeight = 16;
        int srcX = 0;
        int srcY = i * srcHeight;
        getimage(&temp, srcX, srcY, srcWidth, srcHeight);
        block_4[i] = temp;
    }
    
    SetWorkingImage(&img2);
    for (int i = 0; i < 16; i++) {
        int srcWidth = 16;
        int srcHeight = 16;
        int srcX = 0;
        int srcY = i * srcHeight;
        getimage(&temp, srcX, srcY, srcWidth, srcHeight);
        block_2[i] = temp;
    }

    SetWorkingImage(&img3);
    for (int i = 0; i < 12; i++) {
        int srcWidth = 13;
        int srcHeight = 23;
        int srcX = 0;
        int srcY = i * srcHeight;
        getimage(&temp, srcX, srcY, srcWidth, srcHeight);
        num_4[i] = temp;
    }

    SetWorkingImage(&img4);
    for (int i = 0; i < 12; i++) {
        int srcWidth = 13;
        int srcHeight = 23;
        int srcX = 0;
        int srcY = i * srcHeight;
        getimage(&temp, srcX, srcY, srcWidth, srcHeight);
        num_2[i] = temp;
    }

    SetWorkingImage(&img5);
    for (int i = 0; i < 5; i++) {
        int srcWidth = 24;
        int srcHeight = 24;
        int srcX = 0;
        int srcY = i * srcHeight;
        getimage(&temp, srcX, srcY, srcWidth, srcHeight);
        emotion_4[i] = temp;
    }

    SetWorkingImage(&img6);
    for (int i = 0; i < 5; i++) {
        int srcWidth = 24;
        int srcHeight = 24;
        int srcX = 0;
        int srcY = i * srcHeight;
        getimage(&temp, srcX, srcY, srcWidth, srcHeight);
        emotion_2[i] = temp;
    }
}

IMAGE* getImageById(int index, int type, int color) {
    if (type == 0) {
        if (index < 0 || index > 15) {
            return nullptr;
        }
        if (color == 2) {
            return &block_2[index];
        }
        else if (color == 4) {
            return &block_4[index];
        }
    }
    else if (type == 1) {
        if (index < 0 || index > 11) {
            return nullptr;
        }
        if (color == 2) {
            return &num_2[index];
        }
        else if (color == 4) {
            return &num_4[index];
        }
    }
    else if (type == 2) {
        if (index < 0 || index > 4) {
            return nullptr;
        }
        if (color == 2) {
            return &emotion_2[index];
        }
        else if (color == 4) {
            return &emotion_4[index];
        }
    }
    return nullptr;
}

void PaintBoard(float cellSize, float titleHeight, float badge, float space, int row, int column, BoardCell** cells) {
    for (int i = 0; i < row; i++) {
        int top = titleHeight + badge + (i + 1) * space + i * cellSize;
        int bottom = titleHeight + badge + (i + 1) * (space + cellSize);
        for (int j = 0; j < column; j++) {
            int left = badge + (j + 1) * space + j * cellSize;
            int right = badge + (j + 1) * (space + cellSize);
            IMAGE* temp = getImageById(0, 0, 4);
            if (cells[i][j].cellState == CellState::Lock) {
                if (cells[i][j].cellTag == CellTag::Flag) {
                    temp = getImageById(1, 0, 4);
                }
                else if (cells[i][j].cellTag == CellTag::Doubt) {
                    temp = getImageById(2, 0, 4);
                }
                else if (cells[i][j].cellTag == CellTag::Empty) {
                    temp = getImageById(0, 0, 4);
                }
            }
            else if (cells[i][j].cellState == CellState::Unlock) {
                if (cells[i][j].cellType == CellType::Bomb) {
                    temp = getImageById(3, 0, 4);
                }
                else if (cells[i][j].cellType == CellType::Num) {
                    temp = getImageById(15 - cells[i][j].count, 0, 4);
                }
                else if (cells[i][j].cellType == CellType::Empty) {
                    temp = getImageById(15, 0, 4);
                }
            }
            Resize(temp, cellSize, cellSize);
            putimage(left, top, temp);
        }
    }
}

void PaintBoardResult(float cellSize, float titleHeight, float badge, float space, int row, int column, BoardCell** cells) {
    for (int i = 0; i < row; i++) {
        int top = titleHeight + badge + (i + 1) * space + i * cellSize;
        int bottom = titleHeight + badge + (i + 1) * (space + cellSize);
        for (int j = 0; j < column; j++) {
            int left = badge + (j + 1) * space + j * cellSize;
            int right = badge + (j + 1) * (space + cellSize);
            if (cells[i][j].cellState == CellState::Lock) {
                if (cells[i][j].cellTag == CellTag::Flag) {
                    putimage(left, top, getImageById(1, 0, 4));
                }
                else if (cells[i][j].cellTag == CellTag::Doubt) {
                    putimage(left, top, getImageById(1, 0, 4));
                }
            }
        }
    }
}

void DrawBoard(Board board, DrawParam drawParams) {
    int width = drawParams.badge * 2 + drawParams.cellSize * drawParams.column + drawParams.space * (drawParams.column + 1);
    int height = drawParams.titleHeight + drawParams.badge * 2 + drawParams.cellSize * drawParams.row + drawParams.space * (drawParams.row + 1);
    initgraph(width, height);
    SetWorkingImage();
    //setlinecolor(BLACK); 
    setfillcolor(DARKGRAY);
    floodfill(0, 0, BLACK, 1);
    //setbkcolor(LIGHTGRAY);
    BoardCell** cells = board.GetData();

    PaintBoard(drawParams.cellSize, drawParams.titleHeight, drawParams.badge, drawParams.space, drawParams.row, drawParams.column, cells);

    ExMessage m;
    while (true) {
        m = getmessage(EX_MOUSE | EX_KEY);
        pair<int, int> pos = GetPos(drawParams.cellSize, drawParams.titleHeight, drawParams.badge, drawParams.space, drawParams.row, drawParams.column, m.x, m.y);
        int pos_x = pos.first;
        int pos_y = pos.second;
        switch (m.message)
        {
        case WM_MOUSEMOVE:
            break;
        case WM_LBUTTONUP:
            board.Click(pos_x, pos_y, 0);
            PaintBoard(drawParams.cellSize, drawParams.titleHeight, drawParams.badge, drawParams.space, drawParams.row, drawParams.column, board.GetData());
            break;
        case WM_RBUTTONUP:
            board.Click(pos_x, pos_y, 1);
            PaintBoard(drawParams.cellSize, drawParams.titleHeight, drawParams.badge, drawParams.space, drawParams.row, drawParams.column, board.GetData());
            break;
        default:
            break;
        }
        if (board.Result()) {
            PaintBoardResult(drawParams.cellSize, drawParams.titleHeight, drawParams.badge, drawParams.space, drawParams.row, drawParams.column, board.GetData());
            break;
        }
    }
    getchar();
    closegraph();
}



int main() {
    float cellSize = 16;
    float titleHeight = 32;
    float badge = 0;
    float space = 2;
    int row = 9;
    int column = 9;
    int bomb = 9;
    DrawParam drawParams = DrawParam(cellSize, titleHeight, badge, space, row, column, bomb);

    loadBitmaps(drawParams.cellSize);

    Board board = Board(drawParams.row, drawParams.column, drawParams.bomb);
    DrawBoard(board, drawParams);


    /*for (int i = 0; i < 16; i++) {
        IMAGE temp;
        int srcWidth = 16;
        int srcHeight = 16;
        int srcX = 0;
        int srcY = i * srcHeight;
        string str = "block_4_" + to_string(i) + ".bmp";
        size_t slen = str.length() + 1;
        size_t blen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), slen, 0, 0);
        wchar_t* buffer = new wchar_t[blen];
        MultiByteToWideChar(CP_ACP, 0, str.c_str(), slen, buffer, blen);
        getimage(&temp, srcX, srcY, srcWidth, srcHeight);
        saveimage(buffer, &temp);
        delete buffer;
    }*/

    getchar();
    closegraph();
    return 0;
}