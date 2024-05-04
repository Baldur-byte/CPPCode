#pragma once
#include <map>

struct Rect {
    int x;
    int y;
    int width;
    int height;
    Rect(int _x, int _y, int _width, int _height) {
        this->x = _x;
        this->y = _y;
        this->width = _width;
        this->height = _height;
    }
};

enum class BUTTON_MSG : short{
    None = 0,
    Down,
    Up,
    In,
};

class Game;

class Button {
public:
    Button();
    void SetData(int x, int y, int width, int height, const char* text, void (*action)(Game*, int));
    const char* GetText();
    // BindEvent();
    bool TryTrigger(int x, int y, Game*, int param);
    Rect rect;
private:
    char text[20];
    void(*clickEvent) (Game*, int);
    std::map<BUTTON_MSG, void(*)()> eventMap;
};