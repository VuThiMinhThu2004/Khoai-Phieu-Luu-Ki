#pragma once

#include <iostream>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "RenderWindow.h"
#include "Entity.h"
using namespace std;

class Tile : public Entity {
private:
	int tileType;//kiểu của ô gạch
	SDL_Rect collision;//vùng va chạm của ô gạch
public:
// tạo một đối tượng Tile mới với các giá trị ban đầu được cung cấp.
	Tile(float p_x, float p_y, SDL_Texture* p_tex, int p_type);
	int getType();//trả về kiểu ô gạch
	void setX(int p_x);//đặt gt x mới cho ô gạch
	void setType(int p_type) { tileType = p_type; }//kiểu gt mới cho ô gạch
	SDL_Rect getCollision();//trả về vùng va chạm của ô gạch
};

