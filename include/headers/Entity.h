#pragma once

#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <sstream>
#include <fstream>
#include <ctime>
#include <cstdlib>

using namespace std;

class Entity {
protected:
	int xPos, yPos;
	SDL_Rect currentFrame;//đại diện cho vùng hình
	SDL_Texture* tex = NULL;
	//lazyfoo_15
	SDL_RendererFlip flipType = SDL_FLIP_NONE;//đại diện cho kiểu lật của entity
public:
	Entity(int p_x, int p_y, SDL_Texture* p_tex);
	
	int getX();
	int getY();
	SDL_Rect getFrame();
	SDL_Texture* getTex();
	SDL_RendererFlip getFlipType();
};