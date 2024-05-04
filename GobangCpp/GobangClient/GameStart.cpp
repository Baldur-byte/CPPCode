#include "GameStart.h"
#include <easyx.h>

GameStart::GameStart() {
    drawParam = BoardDrawParam(20, 1, 4, 2, 3, 100);
    mtx = nullptr;
    isRunning = false;
    for (int i = 0; i < 5; i++) {
        refreshPages[i] = nullptr;
    }
}

GameStart::~GameStart() {
}

void GameStart::Start(mutex* mtx) {
    this->mtx = mtx;

    curPage = 0;
    isRunning = true;
    MainLoop();
}

void GameStart::DrawStartUI() {
    mtx->lock();
    setfillcolor(LIGHTGRAY);
    floodfill(0, 0, BLACK, 1);
    mtx->unlock();

    RemoveAllButton();

    int buttonWith = 60;
    int buttonHeight = 20;

    AddButton(drawParam.screenWidth / 2 - (buttonWith / 2), drawParam.screenHeight / 2 - 30, buttonWith, buttonHeight, "开始游戏", [](Game* curGame, int param) {
        curGame->GoToLobby();
        });
    AddButton(drawParam.screenWidth / 2 - (buttonWith / 2), drawParam.screenHeight / 2 + 30, buttonWith, buttonHeight, "退出游戏", [](Game* curGame, int param) {
        curGame->Quit();
        });

    DrawButtons();
}

void GameStart::DrawLobbyUI() {
    mtx->lock();
    setfillcolor(LIGHTGRAY);
    floodfill(0, 0, BLACK, 1);
    mtx->unlock();


    refreshPages[0] = [](GameStart* gameStart) {
        gameStart->RemoveAllButton();

        int buttonWith = 100;
        int buttonHeight = 45;
        int halfspace = 10;

        gameStart->AddButton(gameStart->drawParam.screenWidth / 2 - buttonWith - 30, halfspace * 5, buttonWith, buttonHeight, "获取房间信息", [](Game* curGame, int param) {
            curGame->GetRoomList();
            });
        gameStart->AddButton(gameStart->drawParam.screenWidth / 2  + 30, halfspace * 5, buttonWith, buttonHeight, "退出游戏", [](Game* curGame, int param) {
            curGame->QuitToStart();
            });
        gameStart->DrawRooms();
        gameStart->DrawButtons();
        };
}

void GameStart::DrawGameUI() {
    mtx->lock();
    setfillcolor(LIGHTGRAY);
    floodfill(0, 0, BLACK, 1);
    setlinecolor(BLACK);
    setlinestyle(PS_SOLID | PS_ENDCAP_SQUARE, drawParam.lineThickness);
    line(10, drawParam.titleHeight - drawParam.lineThickness, drawParam.screenWidth - 10, drawParam.titleHeight - drawParam.lineThickness);
    mtx->unlock();

    RemoveAllButton();

    // 开始游戏  重新开始  添加机器人  返回大厅  退出游戏

    int buttonWith = 70;
    int buttonHeight = 22;
    int halfspace = 3;

#pragma region 第一行按钮
    AddButton(drawParam.screenWidth / 2 - buttonWith * 2 - halfspace * 3, halfspace, buttonWith, buttonHeight, "准备开始", [](Game* curGame, int param) {
        curGame->ReadyToStart();
        });

    AddButton(drawParam.screenWidth / 2 - buttonWith - halfspace, halfspace, buttonWith, buttonHeight, "重新开始", [](Game* curGame, int param) {
        curGame->RestartRequest();
        });
    AddButton(drawParam.screenWidth / 2 + halfspace, halfspace, buttonWith, buttonHeight, "添加机器人", [](Game* curGame, int param) {
        curGame->AddNetworkRobot();
        });
    AddButton(drawParam.screenWidth / 2 + buttonWith + halfspace * 3, halfspace, buttonWith, buttonHeight, "返回大厅", [](Game* curGame, int param) {
        curGame->QuitToLobby();
        });
#pragma endregion

#pragma region 第二行按钮
    AddButton(drawParam.screenWidth / 2 - buttonWith * 2 - halfspace * 3, buttonHeight + halfspace * 3, buttonWith, buttonHeight, "退出游戏", [](Game* curGame, int param) {
        curGame->QuitToStart();
        });
    AddButton(drawParam.screenWidth / 2 - buttonWith - halfspace, buttonHeight + halfspace * 3, buttonWith, buttonHeight, "空按钮1", [](Game* curGame, int param) {
        });
    AddButton(drawParam.screenWidth / 2 + halfspace, buttonHeight + halfspace * 3, buttonWith, buttonHeight, "空按钮2", [](Game* curGame, int param) {
        });
    AddButton(drawParam.screenWidth / 2 + buttonWith + halfspace * 3, buttonHeight + halfspace * 3, buttonWith, buttonHeight, "空按钮3", [](Game* curGame, int param) {
        });
#pragma endregion

    std::string text = "加入房间 : ";
    text += to_string(curGame.RoomID());
    ShowMessage(text.c_str());

    DrawButtons();
    DrawBoardFrame();

    DrawChessBoard();
    refreshPages[0] = [](GameStart* gameStart) {
        };
}

void GameStart::DrawBoardFrame() {
    mtx->lock();
    setfillcolor(LIGHTGRAY);
    solidrectangle(drawParam.left - drawParam.badge - drawParam.frameThickness / 2, drawParam.top - drawParam.badge - drawParam.frameThickness / 2, drawParam.right + drawParam.badge + drawParam.frameThickness / 2, drawParam.bottom + drawParam.badge + drawParam.frameThickness / 2);
    //floodfill(0, 0, BLACK, 1);

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

void GameStart::AddButton(int x, int y, int width, int height, const char* text, void (*action)(Game*, int)) {
    if (buttonIndex < 20) {
        buttons[buttonIndex].SetData(x, y, width, height, text, action);
        buttonIndex++;
    }
}

void GameStart::RemoveAllButton() {
    for (int i = 0; i < buttonIndex; i++) {
        buttons[i].SetData(0, 0, 0, 0, "", nullptr);
    }
    buttonIndex = 0;
}

void GameStart::ShowMessage(const char* text) {
    mtx->lock();
    setfillcolor(LIGHTGRAY);
    solidrectangle(0, 70, drawParam.screenWidth, 90);
    mtx->unlock();
    //settextcolor(BLACK);
    RECT r = { 0, 70, drawParam.screenWidth, 90 };
    size_t blen = MultiByteToWideChar(CP_ACP, 0, text, strlen(text) + 1, 0, 0);
    wchar_t* buffer = new wchar_t[blen];
    MultiByteToWideChar(CP_ACP, 0, text, strlen(text) + 1, buffer, blen);
    drawtext(buffer, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void GameStart::ChangePage(int page) {
    curPage = page;
}

void GameStart::Quit() {
    isRunning = false;
}

void GameStart::MainLoop() {
    initgraph(drawParam.screenWidth, drawParam.screenHeight, EX_NOCLOSE | EX_NOMINIMIZE);
    curGame.Init(this);
    ExMessage m;
    int delay = 0;
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
            for (int i = 0; i < 5; i++) {
                refreshPages[i] = nullptr;
            }
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
            delay = 3;
        }
        //经常刷新的界面
        if (delay >= 3) {
            for (int i = 0; i < 5; i++) {
                if (refreshPages[i] != nullptr) {
                    refreshPages[i](this);
                }
            }
        }
        else {
            delay++;
        }
    }
    closegraph();
    exit(0);
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

void GameStart::DrawRooms() {
    int** roomList = curGame.RoomList();
    int space = 20;
    int roomCellWidth = (drawParam.screenWidth - space * 2 - space) / 2;
    int roomCellHeight = (drawParam.screenHeight - drawParam.titleHeight - space * 2 - space * 5) / 6;
    for (int i = 0; i < 12; i++) {
        mtx->lock();
        setfillcolor(LIGHTGRAY);

        RECT r = {
            space + (i % 2) * (roomCellWidth + space),
            drawParam.titleHeight + space + (i / 2) * (roomCellHeight + space),
            space + (i % 2) * (roomCellWidth + space) + roomCellWidth,
            drawParam.titleHeight + space + (i / 2) * (roomCellHeight + space) + roomCellHeight };


        std::string textStr = "";
        textStr += to_string(roomList[i][0]) + " : " + to_string(roomList[i][1]);

        const char* text = textStr.c_str();
        AddButton(r.left, r.top, roomCellWidth, roomCellHeight, text, [](Game* curGame, int param) {
            int roomId = 0;
            roomId = curGame->RoomList()[param - 2][0];
            curGame->JoinRoom(roomId);
            });
        //fillroundrect(r.left, r.top, r.right, r.bottom, 3, 3);
        /*size_t blen = MultiByteToWideChar(CP_ACP, 0, text, strlen(text) + 1, 0, 0);
        wchar_t* buffer = new wchar_t[blen];
        MultiByteToWideChar(CP_ACP, 0, text, strlen(text) + 1, buffer, blen);
        drawtext(buffer, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);*/
        mtx->unlock();
    }
}

void GameStart::Click(int x, int y) {
    for (int i = 0; i < buttonIndex; i++) {
        if (buttons[i].TryTrigger(x, y, &curGame, i)) {
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
        curGame.ClickChessBoard(pos_x, pos_y);
    }
    //DrawChess();
}
