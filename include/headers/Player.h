#pragma once
#include <iostream>

#include "Entity.h"
#include "RenderWindow.h"
#include "Bullet.h"
#include "Monster.h"
#include "LevelPart.h"

using namespace std;

class Monster;

class Player : public Entity {
private:
	const int PLAYER_WIDTH = 64;
	const int PLAYER_HEIGHT = 64;	
	const int PLAYER_VEL = 6;//vận tốc

	//lazyfoo_14
	//animation
	static const int WALKING_ANIMATION_FRAMES = 8;
	static const int JUMPING_ANIMATION_FRAMES = 4;
	static const int FALLING_ANIMATION_FRAMES = 4;
	static const int IDLING_ANIMATION_FRAMES = 4;	
	static const int DEATH_ANIMATION_FRAMES = 4;


	SDL_Rect walkingClips[WALKING_ANIMATION_FRAMES];
	SDL_Rect jumpingClips[JUMPING_ANIMATION_FRAMES];
	SDL_Rect fallingClips[FALLING_ANIMATION_FRAMES];
	SDL_Rect idlingClips[IDLING_ANIMATION_FRAMES];	
	SDL_Rect deathClips[DEATH_ANIMATION_FRAMES];

	
	int idleFrame = 0;
	int walkFrame = 0;
	int jumpFrame = 0;
	int fallingFrame = 0;
	int deathFrame = 0;
	int beingHitFrame = 0;


	bool grounded = false; 
	bool running = false; 
	bool idling = true;
	bool jumping = false; 
	bool falling = true; 
	bool dead = false;
	bool beingHit = false;

	
	float xVel = 0, yVel = 0;//vận tốc theo trục x và y của Player
	int groundSTT = 1; //stt của block đang đứng trên
	int levelSTT = 1;
	
	SDL_Rect collision;
	vector<Bullet*> bulletList;
	Mix_Chunk* playerSFX[4];

	bool won = false;
public:
	Player(int p_x, int p_y, SDL_Texture* p_tex);
	
	int maxHP = 100;

	enum SFX {
		hitSFX = 0,
		jumpSFX = 1,
		landSFX = 2,
		shootSFX = 3,
	};

	void handleInputActive(SDL_Event &events, Mix_Chunk* p_sfx[]);
	void update(vector<LevelPart>& LevelPartList, vector<Monster*> &skeletonList, Mix_Chunk* p_sfx[], SDL_Rect& camera);
	void jump();
	void gravity();
	void getHit(vector<Monster*> &skeletonList, Mix_Chunk* p_sfx[], SDL_Rect& camera);
	bool isDead() { return dead; }
	void knockBack();
	void handleCamera(SDL_Rect& camera, float& camVel);//xử lí camera trong trò chơi: lesson 30
	void render(SDL_Rect& p_camera);
	void resetPlayer() {
		xPos = 64 * 3; 
		yPos = LEVEL_HEIGHT - TILE_HEIGHT * 5;
		xVel = 0;
		yVel = 0;
		dead = false;
		maxHP = 100;
		flipType = SDL_FLIP_NONE;
	}

	// thiết lập danh sách đạn được bắn ra bởi nhân vật người chơi.
	void setBulletList(vector<Bullet*> bulletList) { this->bulletList = bulletList; }

	//getterw
	vector<Bullet*> getBulletList() const { return bulletList; } 
	SDL_Rect getCollision() const { return collision; } 
};