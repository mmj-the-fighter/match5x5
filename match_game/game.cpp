#include <iostream>
#include <fstream>
#include <filesystem>

#include "game.h"
#include "sound.h"
#include "level_generator.h"

Game::Game() {
	Init(); 
	LoadOrInitializeSettings();

	SetLevelGenMethod(settings.algorithm);
	//std::cout << "Audio " << settings.audioOn << "\n";
	if (settings.audioOn) {
		EnableAudio();
		checkbox.SetChecked(true);
		initiallySoundWasOff = false;
		PlaySoundLooped("../res/bgm/aguado-op03n03.wav");
	}
	else {
		DisableAudio();
		initiallySoundWasOff = true;
		checkbox.SetChecked(false);
	}
	
	checkbox.SetCallback(std::bind(&Game::OnCheckBoxChanged,
		this,
		std::placeholders::_1));
	

	button.SetCallback([=]() {GenerateNextLevl();});
	button2.SetCallback([=]() {RestartLevel(); });
	button.SetCString("Next Level");
	button2.SetCString("Restart");
	dropdown.AddOption("Algorithm 0");
	dropdown.AddOption("Algorithm 1");
	dropdown.AddOption("Algorithm 2");
	dropdown.SetOption(settings.algorithm);
	dropdown.SetCallback([=](int selectedIndex) {
		settings.algorithm = selectedIndex;
		SetLevelGenMethod(selectedIndex);
	});
}
Game::~Game() {
	if (initialSettings.algorithm != settings.algorithm
		|| initialSettings.audioOn != settings.audioOn)
	{
		SaveSettings();
	}
	
	StopAllSounds();
	ShutdownSoundEngine();
	Destroy();
}

void Game::OnCheckBoxChanged(bool isChecked) {
	if (isChecked) {
		EnableAudio();
		if (initiallySoundWasOff) {
			initiallySoundWasOff = false;
			PlaySoundLooped("../res/bgm/aguado-op03n03.wav");
		}
		else {
			ResumeBGM();
		}
		settings.audioOn = true;
	}
	else {
		PauseBGM();
		DisableAudio();
		settings.audioOn = false;
	}
}

void Game::UpdateAndRender(spn::Canvas* canvas) {
	static int waitFrames = 45;

	if (!isLevelUp && IsLevelCleared()) {
		++score;
		scoreText = "Levels Won " + std::to_string(score);
		isLevelUp = true;
		PlaySoundOnce("../res/sfx/levelUp.wav");
	}

	canvas->Clear();

	if (isLevelUp) {
		canvas->SaveColors();
		canvas->SetPrimaryColor(255, 255, 255);
		canvas->DrawCString("Level Up", cellW * 6, 100);
		canvas->DrawCString("Loading Next Level ...", cellW * 6, 120);
		canvas->DrawString(scoreText, cellW * 6, 140);
		canvas->RestoreColors();
		--waitFrames;
		if (waitFrames <= 0) {
			waitFrames = 45;
			LoadLevel(level, levelCopy, genTiles, levelGenMethod);
			isLevelUp = false;
		}
		return;
	}

	/*for (const auto& spr : sprites) {
		spr->Display(canvas);
	}*/

	int k = 0;
	for (const auto& tile : level) {

		int col = k % MAXCOLORS;
		int row = k / MAXCOLORS;
		int x = startX + (cellW + 2) * col;
		int y = startY + (cellH + 2) * row;
		if (tile.isMoving || tile.isCleared) {
			canvas->DrawImage(bgImage, x, y);
			++k;
			continue;
		}
		int color = tile.color;
		int number = tile.number;
		int index = color * MAXCOLORS + number;
		spn::Image* img = tileImages[index];
		canvas->DrawImage(img, x, y);
		++k;
	}
	spr.Display(canvas);
	if (isGameOver == false && !CanLevelBeCleared()) {
		isGameOver = true;
		PlaySoundOnce("../res/sfx/gameOver.wav");
	}

	if (isGameOver) {
		canvas->SaveColors();
		canvas->SetPrimaryColor(255, 255, 255);
		canvas->DrawCString("GameOver!", cellW * 6, 100);
		canvas->DrawCString("Click Restart / Next Level", cellW * 6, 120);
		canvas->RestoreColors();
	}
	canvas->SaveColors();
	canvas->SetPrimaryColor(255, 255, 255);
	canvas->DrawString(scoreText, cellW * 6, 140);
	canvas->RestoreColors();
	checkbox.SetPosition(cellW * 6, 170+2);
	checkbox.Display(canvas);
	canvas->SaveColors();
	canvas->SetPrimaryColor(255, 255, 255);
	canvas->DrawCString("Audio On / Off", cellW * 6 + 19, 170);
	canvas->RestoreColors();

	button.SetPosition(cellW * 6, 192 + 2);
	button.SetSize(cellW*2, 24);
	button.Display(canvas);

	button2.SetPosition(cellW * 6, 250+2);
	button2.SetSize(cellW * 2, 24);
	button2.Display(canvas);

	dropdown.SetPosition(cellW * 6, 224);
	dropdown.SetSize(cellW * 2, 24);
	dropdown.Display(canvas);

}

void Game::HandleInput(const SDL_Event* e)
{
	Vec2 b;
	Vec2 w;
	int id;
	Tile t;
	float x, y, dx, dy;
	static float prvX = 0, prvY = 0;
	int c1 = -1, c2 = -1, n1 = -1, n2 = -1;
	int color=0;
	int number=0;
	int index=0;

	switch (e->type)
	{
	case SDL_EVENT_MOUSE_BUTTON_DOWN:
		if (!isGameOver && e->button.button == SDL_BUTTON_LEFT)
		{

			mouseInvolved = true;
			x = e->button.x;
			y = e->button.y;
			button.OnLmbDown(x, y);
			button2.OnLmbDown(x, y);
			//dropdown.OnLmbDown(x, y);
			prvX = x;
			prvY = y;
			b = WorldToBoardCoords(x, y);
			eliminated = false;
			if (b.x >= 0 && b.x <= 4 && b.y >= 0 && b.y <= 4) {
				w = BoardToWorldCoords(b.x, b.y);
				id = b.y * 5 + b.x;
				if (false == level[id].isCleared) {
					alteredTileIndex = id;
					level[id].isMoving = true;
					color = level[id].color;
					number = level[id].number;
					index = color * MAXCOLORS + number;
					spn::Image* img = tileImages[index];
					spr.Set(img);
					spr.SetPosition(w.x, w.y);
					spr.OnLmbDown(x, y);
				}
				else {
					alteredTileIndex = -1;
				}
			}
		}
		break;

	case SDL_EVENT_MOUSE_MOTION:
		x = e->motion.x;
		y = e->motion.y;

		button.OnMouseOver(x, y);
		button2.OnMouseOver(x, y);
		dropdown.OnMouseOver(x, y);

		// Correct way to check if LMB is held
		if (!isGameOver && e->motion.state & SDL_BUTTON_LMASK && alteredTileIndex >= 0)
		{
			x = e->motion.x;
			y = e->motion.y;
			//button.OnMouseOver(x, y);
			b = WorldToBoardCoords(x, y);
			if (b.x >= 0 && b.x <= 4 && b.y >= 0 && b.y <= 4) {
				//w = BoardToWorldCoords(b.x, b.y);
				id = b.y * 5 + b.x;
				if (alteredTileIndex != id && level[id].isCleared == false) {
					c1 = level[alteredTileIndex].color;
					n1 = level[alteredTileIndex].number;
					c2 = level[id].color;
					n2 = level[id].number;
					if (n2 == n1 || c1 == c2) {
						level[id].isCleared = true;
						eliminated = true;
						PlaySoundOnce("../res/sfx/pickupCoin.wav");
						if (n2 == n1) {
							//std::cout << "number: " << n1 + 1 << ", " << n2 + 1 << ":";
						}
						if (c1 == c2) {
							//std::cout << "color: " << ColorName(c1) << ", " << ColorName(c2);

						}
						//std::cout << "\n";

					}
				}
			}

			dx = x - prvX;//e->motion.xrel;
			dy = y - prvY;//e->motion.yrel;
			prvX = x;
			prvY = y;
			spr.OnMouseMoved(x, y, dx, dy);
		}
		break;

	case SDL_EVENT_MOUSE_BUTTON_UP:
		if (e->button.button == SDL_BUTTON_LEFT) {
			x = e->button.x;
			y = e->button.y;
			checkbox.OnLmbUp(x, y);
			button.OnLmbUp(x, y);
			button2.OnLmbUp(x, y);
			dropdown.OnLmbUp(x, y);
			if (!isGameOver && alteredTileIndex >= 0)
			{
				spr.OnLmbUp();
				if (eliminated) {
					level[alteredTileIndex].isCleared = true;
				}
				eliminated = false;
				level[alteredTileIndex].isMoving = false;
				spr.image = NULL;
				mouseInvolved = false;
			}
		}
		break;

	case SDL_EVENT_KEY_DOWN:
		switch (e->key.key)
		{
		case SDLK_R:
			RestartLevel();
			break;
		case SDLK_M:
		{
			bool state = !checkbox.GetChecked();
			checkbox.SetChecked(state);
			OnCheckBoxChanged(state);
		}
		break;
		case SDLK_KP_0:
		case SDLK_0://0 is pressed
			SetLevelGenMethod(0);
			dropdown.SetOption(0);
			break;
		case SDLK_KP_1:
		case SDLK_1://1 is pressed
			SetLevelGenMethod(1);
			dropdown.SetOption(1);
			break;
		case SDLK_KP_2:
		case SDLK_2://2 is pressed 
			SetLevelGenMethod(2);
			dropdown.SetOption(2);
			break;
		case SDLK_SPACE:
			GenerateNextLevl();
			break;
		case SDLK_RIGHT: break;
		case SDLK_UP:    break;
		case SDLK_DOWN:  break;
		}
		break;
	}
}
void Game::RestartLevel() {
	level.clear();
	for (const auto& t : levelCopy) {
		level.push_back(t);
	}
	isLevelUp = false;
	isGameOver = false;
} 

Vec2 Game::WorldToBoardCoords(int x, int y) {
	Vec2 b;
	b.x = (x - startX) / cellW;
	b.y = (y - startY) / cellH;
	return b;
}

Vec2 Game::BoardToWorldCoords(int x, int y) {
	Vec2 w;
	w.x = startX + x * cellW;
	w.y = startY + y * cellH;
	return w;
}

bool Game::IsLevelCleared() {
	for (const auto& tile : level) {
		if (tile.isCleared == false) {
			return false;
		}
	}
	return true;
}

void Game::GenerateNextLevl() {
	isLevelUp = false;
	isGameOver = false;
	score = 0;
	scoreText = "Levels Won " + std::to_string(score);
	LoadLevel(level,levelCopy, genTiles, levelGenMethod);
}

bool Game::CanLevelBeCleared() {
	if (mouseInvolved) {
		return true;
	}
	int visibleTiles = 0;
	for (const auto& tile : level) {

		if (tile.isCleared) {
			continue;
		}
		++visibleTiles;
	}
	if (visibleTiles == 0) {
		return true;
	}

	if (visibleTiles == 1) {
		return false;
	}
	
	int size = level.size();
	for (int i = 0; i < size; i++) {
		if (level[i].isCleared) {
			continue;
		}
		bool tileCanBeCleared = false;
		int colorToBeCleared = level[i].color;
		int numberToBeCleared = level[i].number;
		for (int j = 0; j < size; j++) {
			if (i == j || level[j].isCleared) {
				continue;
			}
			if (level[j].color == colorToBeCleared ||
				level[j].number == numberToBeCleared) {
				tileCanBeCleared = true;
				break;
			}
		}
		if (!tileCanBeCleared && !(i == alteredTileIndex && eliminated)) {
			//std::cout << "i " << i << '\n';
			//std::cout << ColorName(colorToBeCleared) <<": "<< numberToBeCleared+1 << " can't be cleared \n";
			int kk = 0;
			for (const auto& l : level) {

				if (!l.isCleared) {
					////std::cout << kk << '\t' << ColorName(l.color) << '\t' << (int)(l.number) + 1 << '\n';
				}
				++kk;
			}
			return false;
		}
	}
	return true;
}

void Game::Init()
{
	for (int i = 0; i < MAXCOLORS; i++) {
		for (int j = 0; j < MAXNUMBERS; j++) {
			Tile tile;
			tile.color = i;
			tile.number = j;
			tile.isMoving = false;
			tile.isCleared = false;
			genTiles.push_back(tile);
		}
	}

	int k = 0;
	for (int color = 0; color < MAXCOLORS; color++) {
		std::string cname = ColorName(color);
		for (int number = 0; number < MAXNUMBERS; number++) {
			std::string path =
				"../res/tiles/" + cname + "/" + std::to_string(number + 1) + ".svg";
			tileImages[k] = new spn::Image();
			tileImages[k]->CreateFromSvg(path, 100);
			++k;
		}
	}
	bgImage = new spn::Image();
	bgImage->CreateFromSvg("../res/tiles/bg.svg", 100);
	LoadLevel(level,levelCopy, genTiles, levelGenMethod);
	cellW = tileImages[0]->GetCanvas()->GetWidth();
	cellH = tileImages[0]->GetCanvas()->GetHeight();
	spr.image = NULL;
	spr.SetPosition(-10, -10);
	score = 0;
	scoreText = "Levels Won " + std::to_string(score);
}

void Game::SetLevelGenMethod(int algorithmType) {
	if (algorithmType < 0) {
		levelGenMethod = 0;
	}
	else if (algorithmType > 2) {
		levelGenMethod = 2;
	}
	else {
		levelGenMethod = algorithmType;
	}
}
void Game::Destroy() 
{
	for (int i = 0; i < MAXCOLORS * MAXNUMBERS; i++) {
		delete tileImages[i];
	}
	delete bgImage;

}

//This function is generated by ChatGPT
void Game::MakeDefaultSettings() {
	settings.audioOn = true;
	settings.algorithm = 0;
}

//This function is generated by ChatGPT
bool Game::SaveSettings() {
	std::ofstream ofs(settingsPath, std::ios::binary | std::ios::trunc);
	if (!ofs) {
		std::cerr << "Failed to write file: " << settingsPath << '\n';
		return false;
	}
	ofs.write(reinterpret_cast<const char*>(&settings), sizeof(Settings));
	return static_cast<bool>(ofs);
}

//This function is generated by ChatGPT
void Game::LoadOrInitializeSettings() {
	std::ifstream ifs(settingsPath, std::ios::binary);

	if (!ifs) {
		MakeDefaultSettings();
		SaveSettings();
	}

	ifs.read(reinterpret_cast<char*>(&settings), sizeof(Settings));
	if (!ifs) {
		MakeDefaultSettings();
		SaveSettings();
	}
	initialSettings = settings;
}
