#pragma once
#include <iostream>

#include "Entity.h"
#include "RenderWindow.h"
#include "Player.h"
#include "LevelPart.h"

using namespace std;

class Player;
//copy tu player sang
class Monster : public Entity {
private:
	const int MONSTER_WIDTH = 64;
	const int MONSTER_HEIGHT = 64;
	const int MONSTER_VEL = 4;

	static const int WALKING_ANIMATION_FRAMES = 4;
	static const int IDLING_ANIMATION_FRAMES = 4;
	static const int FALLING_ANIMATION_FRAMES = 4;
	static const int ATTACKING_ANIMATION_FRAMES = 4;
	static const int BEINGHIT_ANIMATION_FRAMES = 4;

	SDL_Rect walkingClips[WALKING_ANIMATION_FRAMES];
	SDL_Rect idlingClips[IDLING_ANIMATION_FRAMES];
	SDL_Rect fallingClips[FALLING_ANIMATION_FRAMES];
	SDL_Rect attackingClips[FALLING_ANIMATION_FRAMES];
	SDL_Rect beingHitClips[BEINGHIT_ANIMATION_FRAMES];

	int idleFrame = 0;
	int walkFrame = 0;
	int fallingFrame = 0;
	int attackingFrame = 0;
	int beingHitFrame = 0;

	bool grounded = true;
	bool walking = false;
	bool idling = true;
	bool falling = false;
	bool attacking = false;
	bool beingHit = false;
	bool dead = false;

	float xVel = 0, yVel = 0;
	int maxHealth = 3;
	int groundSTT = 1; //số thứ tự của block đang đứng trên
	int levelSTT = 1;

	float distanceToPlayer;
	SDL_Rect collision;
	
public:

	Monster(float p_x, float p_y, SDL_Texture* p_tex);

	//ls27+28
	void update(Player& p_player, vector<LevelPart>& LevelPartList, Mix_Chunk* p_sfx[], SDL_Rect& camera);
	
	void gravity();
	void autoMovement(vector<LevelPart>& LevelPartList);
	void moveToPlayer(Player& p_player, vector<LevelPart>& LevelPartList);
	
	bool isDead() { return dead; }
	bool isAttacking();
	void getHit(Player& p_player, Mix_Chunk* p_sfx[], SDL_Rect& camera);//bị player tấn công

	void knockBack();
	void render(SDL_Rect& p_camera);

	SDL_Rect getCollision() const { return collision; }
	float getDistance() { return distanceToPlayer; }
};

/*
y tuong: Hàm "moveToPlayer" được sử dụng để di chuyển đối tượng Monster đến vị trí của người chơi trong game. 
Hàm này nhận đối số là một đối tượng người chơi và một vector chứa danh sách các phần của màn chơi. 
Đối tượng Monster sẽ tính toán khoảng cách từ chính nó đến người chơi và di chuyển đến vị trí của người chơi 
nếu khoảng cách đó nhỏ hơn một ngưỡng nhất định.
*/