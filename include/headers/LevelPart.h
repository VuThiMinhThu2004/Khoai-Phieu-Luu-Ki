#pragma once

#include "RenderWindow.h"
#include "Tile.h"

using namespace std;
//ls 31

class Tile;

class LevelPart {
private:
	float x, y;
	vector<Tile*> tilesList;//danh sách các Tile trong LevelPart.
	vector<float> monsterPos;//danh sách vị trí 
public:
	LevelPart(float p_x, float p_y, const char* p_path, SDL_Texture* p_tileTex);
	void render(SDL_Rect p_TileClips[], SDL_Rect& p_camera);

	void setLevelX(LevelPart& p_level);//Thiết lập tọa độ x cho đối tượng LevelPart
	void setLevelX(float p_x);
	void setTilesType(const char* p_path);
	void setMonsterPos(vector<float>& p_monsterPos) { monsterPos = p_monsterPos; }

	vector<Tile*> getTilesList() const { return tilesList; }
	vector<float> getMonsterPos() const { return monsterPos; }
	
	int getX() const { return x; }
	int getY() const { return y; }
};