#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <sstream>
#include <fstream>
#include <ctime>
#include <cstdlib>

#include "RenderWindow.h"
#include "Entity.h"
#include "Player.h"
#include "Monster.h"
#include "Timer.h"
#include "Tile.h"
using namespace std;


class Game {
private:
	SDL_Texture* PotatoTex = NULL;
	SDL_Texture* MonsterTex = NULL;
	SDL_Texture* TileTex = NULL;
	SDL_Texture* BulletTex = NULL;
	SDL_Texture* BredatoryTex = NULL;
	SDL_Texture* BackgroundTex = NULL;
	SDL_Texture* ButtonTex = NULL;

	Mix_Music* backgroundMusic = NULL;
	Mix_Chunk* BredatorySFX = NULL;
	Mix_Chunk* playerSFX[4] = { NULL };
	Mix_Chunk* monsterSFX[2] = { NULL };

	//FPScounter
	LTimer fpsTimer;
	stringstream timeText;
	int countedFrames = 0;

	SDL_Rect gTileClips[TOTAL_TILE_SPRITES];
	SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	float camVel = 1.5;
	SDL_Rect Bredatory[8];
	int exCounter = 0;

	bool gameRunning = true;
	
	vector<Monster*> MonsterList;
	vector<Player> playerList;
public:
	Game() {
		setSDL_Rect();
	}
	bool init();
	bool loadMedia();
	void FPSCounter();

	bool createMap();
	bool createLevel();
	bool createMonster();
	bool createPlayer();
	bool createMenu();

	void start_BredatorySFX();
	void playMusic();
	void pauseMusicTime();//dừng phát nhạc khi trò chơi tạm dừng
	
	void render_update_LevelPart();
	void render_update_bullet();
	void render_update_player();
	void render_update_monster();
	void render_update_Bredatory();
	void render_update_Game();
	void render_mainMenu();
	void handleGameInput(SDL_Event& event);
	bool isRunning() { return gameRunning; }

	void setSDL_Rect();
	
	void destroy() {
		SDL_DestroyTexture(PotatoTex);
		SDL_DestroyTexture(MonsterTex);
		SDL_DestroyTexture(TileTex);
		SDL_DestroyTexture(BulletTex);
		SDL_DestroyTexture(BredatoryTex);
		SDL_DestroyTexture(BackgroundTex);
		SDL_DestroyTexture(ButtonTex);
	}
};