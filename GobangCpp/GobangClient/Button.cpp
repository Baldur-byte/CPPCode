#include "Button.h"
#include <stdexcept>

Button::Button() : rect(0, 0, 0, 0){
	clickEvent = nullptr;
}

void Button::SetData(int x, int y, int width, int height, const char* text, void (*action)(Game*)){
	rect.x = x;
	rect.y = y;
	rect.width = width;
	rect.height = height;
	//memset(this->text, 0, strlen(text));
	memcpy(&(this->text), text, strlen(text));
	//this->text = text;
	clickEvent = action;
}

const char* Button::GetText() {
	return this->text;
}

bool Button::TryTrigger(int x, int y, Game* curGame) {
	if (x < rect.x || x > rect.x + rect.width && y < rect.y && y > rect.y + rect.height) {
		return false;
	}
	try {
		//eventMap.at(_btnMsg)(); //如果不存在_btnMsg这个key，抛出out_of_range异常
		if (clickEvent != nullptr) {
			clickEvent(curGame);
			return true;
		}
		else {
			return false;
		}
	}
	catch (std::exception e) {
		return false;
	}
}