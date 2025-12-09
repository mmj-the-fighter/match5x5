#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <iostream>
#include <functional>
#include <string>

#include "../spinach/core/spn_canvas.h"
#include "../spinach/core/spn_image.h"

enum {
	BTN_PRESS,
	BTN_RELEASE,
	BTN_HOVER
};

	class Button
	{
	public:
		void SetCallback(std::function<void(void)> onClickCallback = nullptr){
			onClickFn = onClickCallback;
		}
		void SetSize(int width, int height){
			w = width;
			h = height;
		}
		void SetPosition(int ax, int ay){
			x = ax;
			y = ay;
		}
		void SetCString(const char* t) {
			text = t;
		}
		void SetString(std::string&& s) {
			text = s;
		}
		
		bool OnLmbDown(int mx, int my) {
			if (IsPointInsideBtn(mx, my)) {
				state = BTN_PRESS;
				return true;
			}
			return false;
		}

		bool OnMouseOver(int mx, int my) {
			if (IsPointInsideBtn(mx, my)) {
				if (state == BTN_RELEASE)
				{
					state = BTN_HOVER;
				}
				return true;
			}
			else {
				state = BTN_RELEASE;
			}
			return false;
		}

		bool OnLmbUp(int mx, int my) {
			if(IsPointInsideBtn(mx, my)){
				if (onClickFn != nullptr) {
					onClickFn();
				}
				state = BTN_RELEASE;
				return true;
			}
			return false;
		}
		void Display(spn::Canvas* canvas){
			canvas->SaveColors();
			switch (state)
			{
			case BTN_PRESS:
				canvas->SetPrimaryColor(128, 128, 128);
				canvas->DrawRectangularRegion(x, y, x + w, y + h);
				canvas->DrawFilledRectangularRegion(x + 2, y + 2, x + w - 2, y + h - 2);
				canvas->SetPrimaryColor(0, 0, 0);
				canvas->DrawCString(text.c_str(), x + 12, y + 1);
				break;
			case BTN_HOVER:
				canvas->SetPrimaryColor(128, 0, 0);
				canvas->DrawRectangularRegion(x, y, x + w, y + h);
				canvas->DrawFilledRectangularRegion(x + 2, y + 2, x + w - 2, y + h - 2);
				canvas->SetPrimaryColor(255, 255, 255);
				canvas->DrawCString(text.c_str(), x + 8, y + 2);
				break;
			case BTN_RELEASE:
				canvas->SetPrimaryColor(0, 0, 128);
				canvas->DrawRectangularRegion(x, y, x + w, y + h);
				canvas->DrawFilledRectangularRegion(x + 2, y + 2, x + w - 2, y + h - 2);
				canvas->SetPrimaryColor(255, 255, 255);
				canvas->DrawCString(text.c_str(), x + 8, y + 2);
				break;
			}
			canvas->RestoreColors();
		}
	private:
		bool IsPointInsideBtn(int mx, int my) {
			return (mx >= x) && 
				(mx <= x + w) && 
				(my >= y) && 
				(my <= y + h);
		}
		
		std::function<void(void)> onClickFn;
		int x = 0;
		int y = 0;
		int w = 0;
		int h = 0;
		int state = BTN_RELEASE;
		std::string text;
	};





#endif 
