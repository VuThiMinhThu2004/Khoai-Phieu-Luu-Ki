#pragma once

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <vector>
#include <fstream>

#include "RenderWindow.h"
#include "Tile.h"

using namespace std;
//lesson 31

class Tile;

class LevelPart {
private:
	int x, y;
	vector<Tile*> tilesList;
	vector<int> monsterPos;
	 
public:
	LevelPart(int p_x, int p_y, const char* p_path, SDL_Texture* p_tileTex);
	void render(SDL_Rect p_TileClips[], SDL_Rect& p_camera);

	void setLevelX(LevelPart& p_level);//Thiết lập tọa độ x cho đối tượng LevelPart
	void setLevelX(int p_x);
	void setTilesType(const char* p_path);
	void setMonsterPos(vector<int>& p_monsterPos) { monsterPos = p_monsterPos; }

	vector<Tile*> getTilesList() const { return tilesList; }
	vector<int> getMonsterPos() const { return monsterPos; }
	
	int getX() const { return x; }
	int getY() const { return y; }
};