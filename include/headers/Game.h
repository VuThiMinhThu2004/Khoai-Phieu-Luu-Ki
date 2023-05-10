#pragma once

#include <iostream>

#include "Entity.h"
#include "RenderWindow.h"
#include "Player.h"
#include "Monster.h"
#include "Timer.h"
#include "Tile.h"
#include "Menu.h"
#include "LevelPart.h"
#include "Heart.h"

using namespace std;


class Game {
private:
	SDL_Texture* potatoTex = NULL;
	SDL_Texture* monsterTex = NULL;
	SDL_Texture* tileTex = NULL;
	SDL_Texture* bulletTex = NULL;
	SDL_Texture* bredatoryTex = NULL;
	SDL_Texture* backgroundTex = NULL;
	SDL_Texture* buttonTex = NULL;
	SDL_Texture* circleOfLightTex = NULL;
	SDL_Texture* heartTex = NULL;
	SDL_Texture* dieTex = NULL;
	SDL_Texture* namegameTex = NULL;

	//The music that will be played: lesson 21
	Mix_Music* backgroundMusic = NULL;
	//The sound effects that will be used
	Mix_Chunk* bredatorySFX = NULL;
	Mix_Chunk* playerSFX[4] = { NULL };
	Mix_Chunk* monsterSFX[2] = { NULL };

	//The frames per second timer
	LTimer fpsTimer;//ls 24
	//In memory text stream
	stringstream timeText;
	stringstream hpText;
	stringstream scoreText;
	stringstream highscoreText;

	int hp = 0;
	int score = 0;
	int highscore = 0;
	//Start counting frames per second
	int countedFrames = 0;//theo dõi số lượng khung được hiển thị.

	SDL_Rect gTileClips[TOTAL_TILE_SPRITES];
	SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_Rect bredatory[8];

	float camVel = 1.5;	
	int exCounter = 0;

	bool gameRunning = true;
	
	vector<Monster*> monsterList;
	vector<Player> playerList;

	vector<path_pos> mapList;
	vector<LevelPart> LevelPartList;
	vector<Menu> menuList;
	
public:
	Game() {
		setSDL_Rect();
	}
	bool init();
	bool loadMedia();
	void FPSCounter();
	void HpCounter();
	void renderScore();

	bool createMap();
	bool createLevel();
	bool createMonster();
	bool createPlayer();
	bool createMenu();
	bool createHeart();

	void start_BredatorySFX();
	void playMusic();
	void pauseMusicTime();
	
	void render_update_LevelPart();
	void render_update_bullet();
	void render_update_player();
	void render_update_monster();
	void render_update_bredatory();
	void update_hight_score();
	void render_update_heart();

	void render_update_Game();
	void render_mainMenu();
	void resetGame();

	void handleGameInput(SDL_Event& event);
	bool isRunning() { return gameRunning; }
	
	vector<Menu> getMenuList() { return menuList; }
	void setSDL_Rect();// thiết lập vị trí và kích thước của các đối tượng SDL_Rect 
	
	void destroy() {
		SDL_DestroyTexture(potatoTex);
		SDL_DestroyTexture(monsterTex);
		SDL_DestroyTexture(tileTex);
		SDL_DestroyTexture(bulletTex);
		SDL_DestroyTexture(bredatoryTex);
		SDL_DestroyTexture(backgroundTex);
		SDL_DestroyTexture(buttonTex);
		SDL_DestroyTexture(circleOfLightTex);
		SDL_DestroyTexture(dieTex);
		SDL_DestroyTexture(namegameTex);		
	}
};