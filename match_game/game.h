#ifndef _GAME_H_
#define _GAME_H_
#include <vector>
#include <string>
#include <SDL3/SDL.h>

#include "../spinach/core/spn_canvas.h"
#include "../spinach/core/spn_core.h"
#include "../spinach/core/spn_image.h"
#include "sprite.h"
#include "common.h"
#include "checkbox.h"
#include "button.h"
#include "dropdown.h"
struct Settings {
	bool audioOn;     // true = audio enabled
	int algorithm;    // algorithm number
};

class Game
{
public:
	Game();
	~Game();
	void UpdateAndRender(spn::Canvas* canvas);
	void HandleInput(const SDL_Event* e);
	void OnCheckBoxChanged(bool isChecked);
private:
	void Init();
	void Destroy();
	Vec2 WorldToBoardCoords(int x, int y);
	Vec2 BoardToWorldCoords(int x, int y);
	bool IsLevelCleared();
	bool CanLevelBeCleared();
	void GenerateNextLevl();
	void RestartLevel();
	void SetLevelGenMethod(int algorithmType);
	// Default settings
	void MakeDefaultSettings();
	bool SaveSettings();
	void LoadOrInitializeSettings();
private:
	int cellH = 0;
	int cellW = 0;
	int startX = 10;
	int startY = 10;
	std::vector<Tile> level;
	std::vector<Tile> levelCopy;
	bool mouseInvolved = false;
	std::string scoreText;
	int score = 0;
	bool isGameOver = false;
	bool isLevelUp = false;
	int alteredTileIndex = -1;
	bool eliminated = false;
	spn::Image* tileImages[MAXTILES];
	std::vector<Tile> genTiles;
	spn::Image* bgImage;
	Sprite spr;
	Checkbox checkbox;
	Button button;
	Button button2;
	Dropdown dropdown;
	int levelGenMethod = 0;
	std::string settingsPath = "settings.txt";
	Settings settings;
	Settings initialSettings;
	bool initiallySoundWasOff = true;
};


#endif