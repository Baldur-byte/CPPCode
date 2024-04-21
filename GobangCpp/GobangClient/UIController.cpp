//#include "UIController.h"
//#include "Game.h"
//#include <easyx.h>
//
//UIController::UIController()
//{
//}
//
//UIController::~UIController()
//{
//}
//
//
//void UIController::Draw() {
//    initgraph(drawParam.screenWidth, drawParam.screenHeight);
//}
//
//void UIController::DrawStartUI() {
//    mtx->lock();
//    setfillcolor(LIGHTGRAY);
//    floodfill(0, 0, BLACK, 1);
//    mtx->unlock();
//
//    RemoveAllButton();
//
//    int buttonWith = 60;
//    int buttonHeight = 20;
//
//    AddButton(drawParam.screenWidth / 2 - (buttonWith / 2), drawParam.screenHeight / 2 - 30, buttonWith, buttonHeight, "开始游戏", [](Game* curGame) {
//        //curPage = 1;
//        curGame->Start();
//        });
//    AddButton(drawParam.screenWidth / 2 - (buttonWith / 2), drawParam.screenHeight / 2 + 30, buttonWith, buttonHeight, "退出游戏", [](Game* curGame) {
//        curGame->Quit();
//        });
//
//    DrawButtons();
//}
//
//void UIController::DrawLobbyUI() {
//    mtx->lock();
//    setfillcolor(LIGHTGRAY);
//    floodfill(0, 0, BLACK, 1);
//    mtx->unlock();
//
//    RemoveAllButton();
//
//    int buttonWith = 100;
//    int buttonHeight = 45;
//    int halfspace = 10;
//
//    DrawButtons();
//}
//
//void UIController::DrawGameUI() {
//    mtx->lock();
//    setfillcolor(LIGHTGRAY);
//    floodfill(0, 0, BLACK, 1);
//    mtx->unlock();
//
//    RemoveAllButton();
//
//    // 开始游戏  重新开始  添加机器人  返回大厅  退出游戏
//
//    int buttonWith = 100;
//    int buttonHeight = 45;
//    int halfspace = 10;
//
//    AddButton(drawParam.screenWidth / 2 - buttonWith * 2 - halfspace * 3, 2 * halfspace, buttonWith, buttonHeight, "重新开始", [](Game* curGame) {
//        curGame->Restart();
//        });
//    AddButton(drawParam.screenWidth / 2 - buttonWith - halfspace, 2 * halfspace, buttonWith, buttonHeight, "添加机器人", [](Game* curGame) {
//        curGame->AddNetworkRobot();
//        });
//    AddButton(drawParam.screenWidth / 2 + halfspace, 2 * halfspace, buttonWith, buttonHeight, "返回大厅", [](Game* curGame) {
//        curGame->QuitToLobby();
//        });
//    AddButton(drawParam.screenWidth / 2 + buttonWith + halfspace * 2, 2 * halfspace, buttonWith, buttonHeight, "退出游戏", [](Game* curGame) {
//        curGame->QuitToStart();
//        });
//
//    DrawButtons();
//    DrawBoardFrame();
//    DrawChessBoard();
//}
//
//void UIController::DrawChess() {
//    mtx->lock();
//    ChessBoardCell** data = curGame.GetChessBoardData();
//
//    for (int i = 0; i < 15; i++) {
//        for (int j = 0; j < 15; j++) {
//            switch (data[i][j].cellType)
//            {
//            case CellType::Black:
//                setfillcolor(BLACK);
//                solidcircle(drawParam.badgeLeft + drawParam.dis * i, drawParam.badgeTop + drawParam.dis * j, drawParam.radius);
//                break;
//            case CellType::White:
//                setfillcolor(WHITE);
//                solidcircle(drawParam.badgeLeft + drawParam.dis * i, drawParam.badgeTop + drawParam.dis * j, drawParam.radius);
//                break;
//            default:
//                break;
//            }
//        }
//    }
//    mtx->unlock();
//    /*for (int i = 0; i < 15; i++) {
//        delete[] data[i];
//    }
//    delete[] data;*/
//}
//
//void UIController::DrawButtons() {
//    for (int i = 0; i < buttonIndex; i++) {
//        Rect rect = buttons[i].rect;
//        const char* text = buttons[i].GetText();
//        mtx->lock();
//        setfillcolor(LIGHTGRAY);
//        fillroundrect(rect.x, rect.y, rect.x + rect.width, rect.y + rect.height, 3, 3);
//        RECT r = { rect.x, rect.y, rect.x + rect.width, rect.y + rect.height };
//        size_t blen = MultiByteToWideChar(CP_ACP, 0, text, strlen(text) + 1, 0, 0);
//        wchar_t* buffer = new wchar_t[blen];
//        MultiByteToWideChar(CP_ACP, 0, text, strlen(text) + 1, buffer, blen);
//        drawtext(buffer, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
//        mtx->unlock();
//    }
//}
//
//void UIController::AddButton(int x, int y, int width, int height, const char* text, void (*action)(Game*)) {
//    if (buttonIndex < 10) {
//        buttons[buttonIndex].SetData(x, y, width, height, text, action);
//    }
//    buttonIndex++;
//}
//
//void UIController::RemoveAllButton() {
//    buttonIndex = 0;
//}
//
//void UIController::ShowMessage(const char* text) {
//    mtx->lock();
//    setfillcolor(LIGHTGRAY);
//    solidrectangle(0, 50, drawParam.screenWidth, 50);
//    mtx->unlock();
//    //settextcolor(BLACK);
//    RECT r = { 0, 50, drawParam.screenWidth, 50 };
//    size_t blen = MultiByteToWideChar(CP_ACP, 0, text, strlen(text) + 1, 0, 0);
//    wchar_t* buffer = new wchar_t[blen];
//    MultiByteToWideChar(CP_ACP, 0, text, strlen(text) + 1, buffer, blen);
//    drawtext(buffer, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
//}
//
//void UIController::ChangePage(int page) {
//    curPage = page;
//}
