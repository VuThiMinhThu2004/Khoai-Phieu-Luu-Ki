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
	int xPos, yPos;//tọa độ của Entity trên màn hình
	SDL_Rect currentFrame;//đại diện cho vùng hình(kich thuoc)
	SDL_Texture* tex = NULL;//đại diện cho texture của entity
	//lazyfoo_15
	SDL_RendererFlip flipType = SDL_FLIP_NONE;//đại diện cho kiểu lật của entity
public:
//hàm khởi tạo của lớp Entity, 
//được gọi khi tạo một đối tượng mới của lớp này.
//Hàm này nhận ba tham số là tọa độ x, y và texture của Entity.
	Entity(int p_x, int p_y, SDL_Texture* p_tex);
	
	int getX();//hàm getter (trả về giá trị của biến x).
	int getY();
	SDL_Rect getFrame();// biến curentFrame
	SDL_Texture* getTex();// biến tex
	SDL_RendererFlip getFlipType();// biến flipType
};