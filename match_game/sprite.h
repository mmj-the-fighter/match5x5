#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <iostream>

#include "../spinach/core/spn_canvas.h"
#include "../spinach/core/spn_image.h"

	class Sprite
	{
	public:
		Sprite();
		void Set(spn::Image* img);

		bool OnLmbDown(float x, float y);
		bool OnMouseMoved(float x, float y, float relx, float rely);
		bool OnLmbUp();
		void Display(spn::Canvas* canvas);
		void SetPosition(int x, int y);
		int sprX;
		int sprY;
		int sprW;
		int sprH;
		spn::Image* image;
	private:
		bool dragging;
		int dragStartX;
		int dragStartY;
		bool IsPointInsideSprite(int x, int y);
	};





#endif 
