#pragma once

#include "Entity.h"
#include "RenderWindow.h"
#include "Player.h"
#include "Monster.h"
#include "Timer.h"
#include "Tile.h"
#include "Menu.h"
#include "LevelPart.h"

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

	//The music that will be played: lesson 21
	Mix_Music* backgroundMusic = NULL;
	//The sound effects that will be used
	Mix_Chunk* BredatorySFX = NULL;
	Mix_Chunk* playerSFX[4] = { NULL };
	Mix_Chunk* monsterSFX[2] = { NULL };

	//The frames per second timer
	LTimer fpsTimer;//ls 24
	//In memory text stream
	stringstream timeText;
	stringstream hpText;
	stringstream scoreText;
	stringstream highscoreText;

	int score = 0;
	int highscore = 0;
	//Start counting frames per second
	int countedFrames = 0;//theo dõi số lượng khung được hiển thị.

	SDL_Rect gTileClips[TOTAL_TILE_SPRITES];
	SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_Rect Bredatory[8];

	float camVel = 1.5;	
	int exCounter = 0;

	bool gameRunning = true;
	
	vector<Monster*> MonsterList;
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
	void renderScore();//hiển thị điểm số trên màn hình.

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

	void update_hight_score();

	void render_update_Game();
	void render_mainMenu();
	void resetGame();

	void handleGameInput(SDL_Event& event);
	bool isRunning() { return gameRunning; }

	bool isRunning() { return gameRunning; }//trả về trạng thái trò chơi
	vector<Menu> getMenuList() { return menuList; }
	void setSDL_Rect();// thiết lập vị trí và kích thước của các đối tượng SDL_Rect 
	
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