#include "GameStart.h"
#include <easyx.h>

GameStart::GameStart() {
    drawParam = BoardDrawParam(20, 1, 4, 2, 3, 100);
    mtx = nullptr;
}

GameStart::~GameStart() {
}

void GameStart::Start(mutex* mtx) {
    this->mtx = mtx;

    curPage = 0;
    isRunning = true;
    MainLoop();
}

void GameStart::Draw() {
    initgraph(drawParam.screenWidth, drawParam.screenHeight);
}

void GameStart::DrawStartUI() {
    mtx->lock();
    setfillcolor(LIGHTGRAY);
    floodfill(0, 0, BLACK, 1);
    mtx->unlock();

    RemoveAllButton();

    int buttonWith = 60;
    int buttonHeight = 20;

    AddButton(drawParam.screenWidth / 2 - (buttonWith / 2), drawParam.screenHeight / 2 - 30, buttonWith, buttonHeight, "开始游戏", [] (Game* curGame) {
        //curPage = 1;
        curGame->Start();
    });
    AddButton(drawParam.screenWidth / 2 - (buttonWith / 2), drawParam.screenHeight / 2 + 30, buttonWith, buttonHeight, "退出游戏", [](Game* curGame) {
        curGame->Quit();
        });

    DrawButtons();
}

void GameStart::DrawLobbyUI() {
    mtx->lock();
    setfillcolor(LIGHTGRAY);
    floodfill(0, 0, BLACK, 1);
    mtx->unlock();

    RemoveAllButton();

    int buttonWith = 100;
    int buttonHeight = 45;
    int halfspace = 10;

    DrawButtons();
}

void GameStart::DrawGameUI() {
    mtx->lock();
    setfillcolor(LIGHTGRAY);
    floodfill(0, 0, BLACK, 1);
    mtx->unlock();

    RemoveAllButton();

    // 开始游戏  重新开始  添加机器人  返回大厅  退出游戏

    int buttonWith = 100;
    int buttonHeight = 45;
    int halfspace = 10;

    AddButton(drawParam.screenWidth / 2 - buttonWith * 2 - halfspace * 3, 2 * halfspace, buttonWith, buttonHeight, "重新开始", [](Game* curGame) {
        curGame->Restart();
        });
    AddButton(drawParam.screenWidth / 2 - buttonWith - halfspace, 2 * halfspace, buttonWith, buttonHeight, "添加机器人", [](Game* curGame) {
        curGame->AddNetworkRobot();
        });
    AddButton(drawParam.screenWidth / 2 + halfspace, 2 * halfspace, buttonWith, buttonHeight, "返回大厅", [](Game* curGame) {
        curGame->QuitToLobby();
        });
    AddButton(drawParam.screenWidth / 2 + buttonWith + halfspace * 2, 2 * halfspace, buttonWith, buttonHeight, "退出游戏", [](Game* curGame) {
        curGame->QuitToStart();
        });

    DrawButtons();
    DrawBoardFrame();
    DrawChessBoard();
}

void GameStart::DrawChess() {
    mtx->lock();
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
                setfillcolor(WHITE);
                solidcircle(drawParam.badgeLeft + drawParam.dis * i, drawParam.badgeTop + drawParam.dis * j, drawParam.radius);
                break;
            default:
                break;
            }
        }
    }
    mtx->unlock();
    /*for (int i = 0; i < 15; i++) {
        delete[] data[i];
    }
    delete[] data;*/
}

void GameStart::DrawButtons() {
    for (int i = 0; i < buttonIndex; i++) {
        Rect rect = buttons[i].rect;
        const char* text = buttons[i].GetText();
        mtx->lock();
        setfillcolor(LIGHTGRAY);
        fillroundrect(rect.x, rect.y, rect.x + rect.width, rect.y + rect.height, 3, 3);
        RECT r = { rect.x, rect.y, rect.x + rect.width, rect.y + rect.height };
        size_t blen = MultiByteToWideChar(CP_ACP, 0, text, strlen(text) + 1, 0, 0);
        wchar_t* buffer = new wchar_t[blen];
        MultiByteToWideChar(CP_ACP, 0, text, strlen(text) + 1, buffer, blen);
        drawtext(buffer, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        mtx->unlock();
    }
}

void GameStart::AddButton(int x, int y, int width, int height, const char* text, void (*action)(Game*)) {
    if (buttonIndex < 10) {
        buttons[buttonIndex].SetData(x, y, width, height, text, action);
    }
    buttonIndex++;
}

void GameStart::RemoveAllButton() {
    buttonIndex = 0;
}

void GameStart::ShowMessage(const char* text) {
    mtx->lock();
    setfillcolor(LIGHTGRAY);
    solidrectangle(0, 50, drawParam.screenWidth, 50);
    mtx->unlock();
    //settextcolor(BLACK);
    RECT r = { 0, 50, drawParam.screenWidth, 50 };
    size_t blen = MultiByteToWideChar(CP_ACP, 0, text, strlen(text) + 1, 0, 0);
    wchar_t* buffer = new wchar_t[blen];
    MultiByteToWideChar(CP_ACP, 0, text, strlen(text) + 1, buffer, blen);
    drawtext(buffer, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void GameStart::ChangePage(int page) {
    curPage = page;
}

void GameStart::MainLoop() {
    initgraph(drawParam.screenWidth, drawParam.screenHeight);
    curGame.Enter(this);
    ExMessage m;
    while (isRunning) {
        m = getmessage(EX_MOUSE | EX_KEY | EX_WINDOW);
        switch (m.message)
        {
        case WM_LBUTTONDOWN:
            Click(m.x, m.y);
            break;
        case WM_KEYDOWN:
            if (m.vkcode == VK_ESCAPE) {
                curGame.Quit();
                isRunning = false;
            }
            break;
        case WM_ACTIVATE:
            if (m.wParam == 0) {

            }
            if (m.lParam == 0) {

            }
        default:
            break;
        }

        if (curPage != -1) {
            cleardevice();
            if (curPage == 0) {
                DrawStartUI();
                curPage = -1;
            }
            else if (curPage == 1) {
                DrawLobbyUI();
                curPage = -1;
            }
            else if (curPage == 2) {
                DrawGameUI();
                curPage = -1;
            }
        }
    }
    closegraph();
}

void GameStart::DrawBoardFrame() {
    mtx->lock();
    setlinecolor(BLACK);
    setlinestyle(PS_SOLID | PS_ENDCAP_SQUARE, drawParam.frameThickness);
    line(drawParam.left, drawParam.top, drawParam.right - drawParam.frameThickness / 2, drawParam.top);
    line(drawParam.right, drawParam.top, drawParam.right, drawParam.bottom - drawParam.frameThickness / 2);
    line(drawParam.right, drawParam.bottom, drawParam.left + drawParam.frameThickness / 2, drawParam.bottom);
    line(drawParam.left, drawParam.bottom, drawParam.left, drawParam.top + drawParam.frameThickness / 2);
    mtx->unlock();
}

void GameStart::DrawChessBoard() {
    mtx->lock();
    setlinecolor(BLACK);
    setlinestyle(PS_SOLID | PS_ENDCAP_SQUARE, drawParam.lineThickness);

    for (int i = 0; i < 15; i++) {
        line(drawParam.badgeLeft, drawParam.badgeTop + drawParam.dis * i, drawParam.badgeRight, drawParam.badgeTop + drawParam.dis * i);
        line(drawParam.badgeLeft + drawParam.dis * i, drawParam.badgeTop, drawParam.badgeLeft + drawParam.dis * i, drawParam.badgeBottom);
    }
    mtx->unlock();
}

void GameStart::Click(int x, int y) {
    for (int i = 0; i < buttonIndex; i++) {
        if (buttons[i].TryTrigger(x, y, &curGame)) {
            return;
        }
    }

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
    if (pos_x != -1 && pos_y != -1) {
        curGame.Click(pos_x, pos_y);
    }
    //DrawChess();
}
