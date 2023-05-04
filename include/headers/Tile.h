#pragma once

#include "RenderWindow.h"
#include "Entity.h"
using namespace std;

//lesson 39
class Tile : public Entity {
private:
	int tileType;//kiểu của ô gạch
	SDL_Rect collision;
public:
	Tile(int p_x, int p_y, SDL_Texture* p_tex, int p_type);
	int getType();
	void setX(int p_x);
	void setType(int p_type) { tileType = p_type; }
	SDL_Rect getCollision();
};

