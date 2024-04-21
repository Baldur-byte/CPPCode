//#pragma once
//#include "Button.h"
//#include <mutex>
//
//struct BoardDrawParam {
//    int gridWidth;
//    int lineThickness;
//    int frameThickness;
//    int space;
//    int badge;
//    int titleHeight;
//
//    int screenWidth;
//    int screenHeight;
//
//    int left;
//    int right;
//    int top;
//    int bottom;
//
//    int badgeLeft;
//    int badgeRight;
//    int badgeTop;
//    int badgeBottom;
//
//    int dis;
//
//    int radius;
//
//    BoardDrawParam(int gridWidth = 20, int lineThickness = 1, int frameThickness = 4, int space = 2, int badge = 3, int titleHeight = 100) {
//        this->gridWidth = gridWidth;
//        this->lineThickness = lineThickness;
//        this->frameThickness = frameThickness;
//        this->space = space;
//        this->badge = badge;
//        this->titleHeight = titleHeight;
//
//        this->screenWidth = badge * 2 + frameThickness * 2 + space * 2 + lineThickness * 15 + gridWidth * 14;
//        this->screenHeight = screenWidth + titleHeight;
//
//        this->left = badge + frameThickness / 2;
//        this->right = left + frameThickness + space * 2 + lineThickness * 15 + gridWidth * 14;
//        this->top = titleHeight + badge + frameThickness / 2;
//        this->bottom = top + frameThickness + space * 2 + lineThickness * 15 + gridWidth * 14;
//
//        this->badgeLeft = left + space + frameThickness / 2 + lineThickness / 2;
//        this->badgeRight = badgeLeft + gridWidth * 14 + lineThickness * 14;
//        this->badgeTop = top + space + frameThickness / 2 + lineThickness / 2;
//        this->badgeBottom = badgeTop + gridWidth * 14 + lineThickness * 14;
//
//        this->dis = gridWidth + lineThickness;
//
//        this->radius = gridWidth / 2 - 1;
//    }
//};
//
//
//class UIController
//{
//public:
//	UIController();
//	~UIController();
//    void Draw();
//    void DrawStartUI();
//    void DrawLobbyUI();
//    void DrawGameUI();
//    void DrawChess();
//    void DrawButtons();
//    void AddButton(int x, int y, int width, int height, const char* text, void (*action)(Game*));
//    void RemoveAllButton();
//    void ShowMessage(const char* text);
//    void ChangePage(int page);
//private:
//    void DrawBoardFrame();
//    void DrawChessBoard();
//
//    BoardDrawParam drawParam;
//    mutex* mtx;
//    Button buttons[10];
//    int buttonIndex = 0;
//    int curPage = 0; // 0  开始界面   1  房间选择   2  游戏界面
//};