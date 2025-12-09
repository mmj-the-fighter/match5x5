#ifndef _LEVEL_GENERATOR_H_
#define _LEVEL_GENERATOR_H_
#include <random>
#include <vector>
#include <algorithm>
#include "common.h"


static void LoadLevel1(std::vector<Tile>& level) {	
	std::vector<Tile> matchTiles;

	level.clear();
	Tile t;
	Tile m;
	std::random_device rd;
	std::mt19937 g(rd());
	std::uniform_int_distribution<int> colorDist(0, MAXCOLORS - 1);
	std::uniform_int_distribution<int> numberDist(0, MAXCOLORS - 1);

	for (int i = 0; i < MAXTILES / 2; i++) {
		t.color = colorDist(g);;
		t.number = numberDist(g);
		t.isMoving = false;
		t.isCleared = false;
		level.push_back(t);
		matchTiles.clear();
		matchTiles.push_back(t);
		for (int number = 0; number < MAXCOLORS; number++) {
			m.color = t.color;
			m.number = number;
			m.isMoving = false;
			m.isCleared = false;
			if (m.color == t.color && m.number == t.number) {
				continue;
			}
			matchTiles.push_back(m);
		}
		for (int color = 0; color < MAXCOLORS; color++) {
			m.color = color;
			m.number = t.number;
			m.isMoving = false;
			m.isCleared = false;
			if (m.color == t.color && m.number == t.number) {
				continue;
			}
			matchTiles.push_back(m);
		}
		std::uniform_int_distribution<int> matchDist(0, (int)(matchTiles.size()) - 1);
		int selection = matchDist(g);
		level.push_back(matchTiles[selection]);
	}
	
	if (MAXTILES % 2 != 0) {
		matchTiles.clear();
		for (const auto& genTile : level) {
			matchTiles.push_back(genTile);
			for (int number = 0; number < MAXCOLORS; number++) {
				m.color = genTile.color;
				m.number = number;
				m.isMoving = false;
				m.isCleared = false;
				if (m.color == genTile.color && m.number == genTile.number) {
					continue;
				}
				matchTiles.push_back(m);
			}
			for (int color = 0; color < MAXCOLORS; color++) {
				m.color = color;
				m.number = genTile.number;
				m.isMoving = false;
				m.isCleared = false;
				if (m.color == genTile.color && m.number == genTile.number) {
					continue;
				}
				matchTiles.push_back(m);
			}
		}
		std::uniform_int_distribution<int> matchDist(0, (int)(matchTiles.size()) - 1);
		int selection = matchDist(g);
		level.push_back(matchTiles[selection]);
	}

	std::shuffle(level.begin(), level.end(),g);
	/*for (const auto& tile : level) {
		int color = tile.color;
		int number = tile.number;
		std::cout << ColorName(color) << "\t" << number << "\n";
	}*/
}

static void LoadLevel2(std::vector<Tile>& level, std::vector<Tile>& genTiles) {
	std::vector<Tile> matchTiles;
	

	level.clear();
	Tile t;
	Tile m;
	std::random_device rd;
	std::mt19937 g(rd());
	std::uniform_int_distribution<int> colorDist(0, MAXCOLORS - 1);
	std::uniform_int_distribution<int> numberDist(0, MAXNUMBERS - 1);

	
	std::shuffle(genTiles.begin(), genTiles.end(), g);
	int k = 0;
	for (int i = 0; i < MAXTILES / 2; i++) {
		t = genTiles[k++];
		//std::cout << "gen " << ColorName(t.color) << " " << t.number + 1 << "\n";
		level.push_back(t);
		matchTiles.clear();
		matchTiles.push_back(t);
		for (int number = 0; number < MAXCOLORS; number++) {
			m.color = t.color;
			m.number = number;
			m.isMoving = false;
			m.isCleared = false;
			if (m.color == t.color && m.number == t.number) {
				continue;
			}
			matchTiles.push_back(m);
		}
		for (int color = 0; color < MAXCOLORS; color++) {
			m.color = color;
			m.number = t.number;
			m.isMoving = false;
			m.isCleared = false;
			if (m.color == t.color && m.number == t.number) {
				continue;
			}
			matchTiles.push_back(m);
		}
		std::uniform_int_distribution<int> matchDist(0, (int)(matchTiles.size()) - 1);
		int selection = matchDist(g);
		level.push_back(matchTiles[selection]);
	}

	if (MAXTILES % 2 != 0) {
		matchTiles.clear();
		for (const auto& genTile : level) {
			matchTiles.push_back(genTile);
			for (int number = 0; number < MAXCOLORS; number++) {
				m.color = genTile.color;
				m.number = number;
				m.isMoving = false;
				m.isCleared = false;
				if (m.color == genTile.color && m.number == genTile.number) {
					continue;
				}
				matchTiles.push_back(m);
			}
			for (int color = 0; color < MAXCOLORS; color++) {
				m.color = color;
				m.number = genTile.number;
				m.isMoving = false;
				m.isCleared = false;
				if (m.color == genTile.color && m.number == genTile.number) {
					continue;
				}
				matchTiles.push_back(m);
			}
		}
		std::uniform_int_distribution<int> matchDist(0, (int)(matchTiles.size()) - 1);
		int selection = matchDist(g);
		level.push_back(matchTiles[selection]);
	}

	std::shuffle(level.begin(), level.end(), g);
	/*for (const auto& tile : level) {
		int color = tile.color;
		int number = tile.number;
		std::cout << ColorName(color) << "\t" << number << "\n";
	}*/
}

static void LoadLevel3(std::vector<Tile>& level, std::vector<Tile>& genTiles) {
	std::random_device rd;
	std::mt19937 g(rd());
	level.clear();
	std::shuffle(genTiles.begin(), genTiles.end(), g);
	for(const auto& t: genTiles){
		level.push_back(t);
	}
}

static void LoadLevel(std::vector<Tile>& level, std::vector<Tile>& levelCopy, std::vector<Tile>& genTiles, int strategy=0) {
	switch (strategy) {
	case 0:
		LoadLevel1(level);
		break;
	case 1:
		LoadLevel2(level, genTiles);
		break;
	case 2:
		LoadLevel3(level, genTiles);
		break;
	default:
		LoadLevel1(level);
		break;
	}
	levelCopy.clear();
	for (const auto& t : level) {
		levelCopy.push_back(t);
	}
	//std::cout << "algorith :"<<strategy<<" is used to generate level\n";
}
#endif