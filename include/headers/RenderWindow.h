#pragma once

#include <iostream>

#include "Entity.h"
#include "LevelPart.h"
#include "Tile.h"

using namespace std;

class LevelPart;
class Tile;

const float GRAVITY = 0.35;//lực hấp dẫn
const float MAX_GRAVITY = 15;

//screen
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

//level
const int LEVEL_WIDTH = 1344;
const int LEVEL_HEIGHT = 1024;

//tile
const int TILE_WIDTH = 64;
const int TILE_HEIGHT = 64;


//các hằng số liên quan đến số lượng các phần, bản đồ và sprite trong mức chơi.
const int TOTAL_TILES = 336;
const int TOTAL_TILE_SPRITES = 187;
const int TOTAL_LEVEL_PART = 3;
const int TOTAL_MAP = 5;//xem lai

//các con trỏ tĩnh đến font, cửa sổ, trình kết xuất và bề mặt của SDL.
static TTF_Font* font = NULL;
static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static SDL_Surface* surface = NULL;


struct path_pos {
	const char* path;
	vector<float> monsterPos;
	path_pos(vector<float> p_monsterPos, const char* p_path) {
		path = p_path;
		monsterPos =  p_monsterPos;
	}
};

//tải và hiển thị texture, tạo văn bản, kiểm tra va chạm và các hàm khác.
namespace commonFunc {
	//Window
	void renderWindow(const char* p_title, int p_width, int p_height);

	//Texture: lazyfoo
	SDL_Texture* loadTexture(const char* p_filePath);
	void renderTexture(Entity& entity, SDL_Rect *rec = NULL, SDL_Rect *camera = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void renderTexture(SDL_Texture* p_tex, float p_x, float p_y, float p_w = 0, float p_h = 0, SDL_Rect* rec = NULL, SDL_Rect* camera = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void renderTile(Entity& entity, SDL_Rect& rec, SDL_Rect& camera);
	void renderPlayer(Entity& entity, SDL_Rect& camera);
	void renderAnimation(SDL_Texture* p_tex, float p_x, float p_y, SDL_Rect& p_clip, SDL_Rect& p_camera, double p_angle = 0.0, SDL_Point* p_center = NULL, SDL_RendererFlip p_flip = SDL_FLIP_NONE);

	//lesson 27: Collision boxes are a standard way to check collision between two objects
	//ls28: per-pixer-collision-detection
	bool checkCollision(SDL_Rect a, SDL_Rect b);//Box collision detector
	bool touchesWood(SDL_Rect& box, vector<LevelPart>& LevelPartList);
	bool touchesWood(SDL_Rect& box, vector<LevelPart>& LevelPartList,bool& grounded, int& groundSTT, int& levelSTT);

	//Font
	bool loadFont(const char* filePath);
	SDL_Texture* createText(string p_text, SDL_Color p_textColor);

	void Color();
	void clearRenderer();
	void renderPresent();
	void cleanUp();
}
	
