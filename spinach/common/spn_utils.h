#ifndef _SPN_UTILS_H_
#define _SPN_UTILS_H_
#include <string>
namespace spn
{
	typedef struct _Rect
	{
		int left;
		int top;
		int width;
		int height;
	}Rect;
	void FindRectToRectIntersection(const Rect& r1, const Rect& r2, Rect& ir);
	std::string GetTimeBasedScreenShotFileName();
	static inline void UnpackRgb(
		unsigned int rgb,
		unsigned char& r,
		unsigned char& g,
		unsigned char& b
	) {
		r = (unsigned char)((rgb >> 16) & 0xFF);
		g = (unsigned char)((rgb >> 8) & 0xFF);
		b = (unsigned char)(rgb & 0xFF);
	}
	static inline void PackRgb(
		unsigned char r,
		unsigned char g,
		unsigned char b,
		unsigned int& rgb
	) {
		rgb = (r << 16) | (g << 8) | b;
	}
}

#endif

