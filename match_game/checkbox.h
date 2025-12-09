#ifndef _CHECKBOX_H_
#define _CHECKBOX_H_

#include <iostream>
#include <functional>

#include "../spinach/core/spn_canvas.h"
#include "../spinach/core/spn_image.h"

	class Checkbox
	{
	public:
		void SetCallback(std::function<void(bool isChecked)> onCheckboxChangedCallback = nullptr){
			onCheckboxChangedFn = onCheckboxChangedCallback;
		}
		void SetPosition(int ax, int ay){
			x = ax;
			y = ay;
		}
		void SetChecked(bool checked) {
			isChecked = checked;
		}
		bool GetChecked() {
			return isChecked;
		}
		bool OnLmbUp(int mx, int my) {
			if(IsPointInsideCb(mx, my)){
				isChecked = !isChecked;
				if (onCheckboxChangedFn != nullptr) {
					onCheckboxChangedFn(isChecked);
				}
				return true;
			}
			return false;
		}
		void Display(spn::Canvas* canvas){
			//canvas->SaveColors();
			canvas->DrawRectangularRegion(x, y, x+sqSize, y+sqSize);
			if(isChecked) {
				canvas->DrawFilledRectangularRegion(x+2, y+2, x+sqSize-2, y+sqSize-2);
			}
			//canvas->RestoreColors();
		}
	private:
		bool IsPointInsideCb(int mx, int my) {
			return (mx >= x) && 
				(mx <= x + sqSize) && 
				(my >= y) && 
				(my <= y + sqSize);
		}
		
		std::function<void(bool isChecked)> onCheckboxChangedFn;
		bool isChecked = false;
		int sqSize = 14;
		int x = 0;
		int y = 0;
	};





#endif 
