#include "GameStart.h"
#include <easyx.h>

GameStart::GameStart() {
    drawParam = BoardDrawParam(20, 1, 4, 2, 3, 100);
    //curGame = Game();
}

GameStart::~GameStart() {

}

void GameStart::Start() {
    Draw();

    curGame.Start();

    StartListen();

    closegraph();
}


void GameStart::Click(int x, int y) {
    int pos_x = -1;
    int pos_y = -1;
    for (int i = 0; i < 15; i++) {
        if (x > drawParam.badgeLeft + drawParam.dis * i - drawParam.dis / 2 && x < drawParam.badgeLeft + drawParam.dis * i + drawParam.dis / 2) {
            pos_x = i;
        }
        if (y > drawParam.badgeTop + drawParam.dis * i - drawParam.dis / 2 && y < drawParam.badgeTop + drawParam.dis * i + drawParam.dis / 2) {
            pos_y = i;
        }
    }
    if (pos_x == -1 || pos_y == -1) {
        return;
    }
    curGame.Click(pos_x, pos_y);
    DrawChess();
}

void GameStart::DrawBoardFrame() {
    setlinecolor(BLACK);
    setlinestyle(PS_SOLID | PS_ENDCAP_SQUARE, drawParam.frameThickness);
    line(drawParam.left, drawParam.top, drawParam.right - drawParam.frameThickness / 2, drawParam.top);
    line(drawParam.right, drawParam.top, drawParam.right, drawParam.bottom - drawParam.frameThickness / 2);
    line(drawParam.right, drawParam.bottom, drawParam.left + drawParam.frameThickness / 2, drawParam.bottom);
    line(drawParam.left, drawParam.bottom, drawParam.left, drawParam.top + drawParam.frameThickness / 2);
}

void GameStart::DrawChessBoard() {
    setlinecolor(BLACK);
    setlinestyle(PS_SOLID | PS_ENDCAP_SQUARE, drawParam.lineThickness);

    for (int i = 0; i < 15; i++) {
        line(drawParam.badgeLeft, drawParam.badgeTop + drawParam.dis * i, drawParam.badgeRight, drawParam.badgeTop + drawParam.dis * i);
        line(drawParam.badgeLeft + drawParam.dis * i, drawParam.badgeTop, drawParam.badgeLeft + drawParam.dis * i, drawParam.badgeBottom);
    }
}

void GameStart::DrawChess() {
    ChessBoardCell** data = curGame.GetChessBoardData();

    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            switch (data[i][j].cellType)
            {
            case CellType::Black:
                setfillcolor(BLACK);
                solidcircle(drawParam.badgeLeft + drawParam.dis * i, drawParam.badgeTop + drawParam.dis * j, drawParam.radius);
                break;
            case CellType::White:
                break;
                setfillcolor(WHITE);
                solidcircle(drawParam.badgeLeft + drawParam.dis * i, drawParam.badgeTop + drawParam.dis * j, drawParam.radius);
            default:
                break;
            }
        }
    }

    /*for (int i = 0; i < 15; i++) {
        delete[] data[i];
    }
    delete[] data;*/
}

void GameStart::Draw() {
    initgraph(drawParam.screenWidth, drawParam.screenHeight);
    setfillcolor(LIGHTGRAY);
    floodfill(0, 0, BLACK, 1);

    DrawBoardFrame();
    DrawChessBoard();
}

void GameStart::StartListen() {
    ExMessage m;
    while (true) {
        m = getmessage(EX_MOUSE | EX_KEY);
        switch (m.message)
        {
        case WM_LBUTTONDOWN:
            Click(m.x, m.y);
            break;
        default:
            break;
        }
    }

}
