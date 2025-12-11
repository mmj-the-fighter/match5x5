#ifndef _COMMON_H_
#define _COMMON_H_

enum Color {
	ORANGE,
	BROWN,
	GREEN,
	BLUE,
	YELLOW,
};
#define	MAXCOLORS 5
#define	MAXNUMBERS 5
#define	MAXCOLUMNS 5
#define	MAXROWS 5
#define MAXTILES 25

struct Vec2 {
	int x;
	int y;
};

struct RowCol {
	int r;
	int c;
};

struct Tile {
	unsigned char number;
	unsigned char color;
	bool isMoving;
	bool isCleared;
};

static std::string ColorName(int i) {
	switch (i) {
	case 0:
		return "orange";
	case 1:
		return "brown";
	case 2:
		return "green";
	case 3:
		return "blue";
	case 4:
		return "yellow";
	}
	return "";
}

#endif