#pragma once

#include "Entity.h"
#include "RenderWindow.h"
#include "Bullet.h"
#include "Monster.h"

using namespace std;

class Monster;

class Player : public Entity {
private:
	const int PLAYER_WIDTH = 64;
	const int PLAYER_HEIGHT = 64;	
	const int PLAYER_VEL = 6;//vận tốc

	//layzfoo_14
	//animation: khai báo số lượng ảnh cần load của mỗi loại
	static const int WALKING_ANIMATION_FRAMES = 8;
	static const int JUMPING_ANIMATION_FRAMES = 4;
	static const int FALLING_ANIMATION_FRAMES = 4;
	static const int IDLING_ANIMATION_FRAMES = 4;	
	static const int DEATH_ANIMATION_FRAMES = 4;

	//mảng chứa các clip hình ảnh được sử dụng để tạo hiệu ứng hoạt hình cho Player.
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

	//lưu trữ trạng thái của Player: va chạm, đạn bắn ra, âm thanh và hiệu ứng hình ảnh.
	bool grounded = false; //đứng trên mặt đất hay không.
	bool running = false; 
	bool idling = true; //đang đứng yên hay không.
	bool jumping = false; 
	bool falling = true; 
	bool dead = false;
	bool beingHit = false;

	
	float xVel = 0, yVel = 0;//vận tốc theo trục x và y của Player
	int groundSTT = 1; //số thứ tự của block đang đứng trên
	int levelSTT = 1;

	vector<Bullet*> bulletList;
	SDL_Rect collision;//lưu trữ kích thước và vị trí của Player để xử lý va chạm.
	Mix_Chunk* playerSFX[4];
	bool won = false;
public:
	Player(float p_x, float p_y, SDL_Texture* p_tex);//khởi tạo player với tọa đồ và hình ảnh tương ứng
	
	enum SFX {
		hitSFX = 0,
		jumpSFX = 1,
		landSFX = 2,
		shootSFX = 3,
	};

	void handleInputActive(SDL_Event &events, Mix_Chunk* p_sfx[]);
	void update(vector<LevelPart>& LevelPartList, vector<Monster*> &monsterList, Mix_Chunk* p_sfx[], SDL_Rect& camera);
	
	void jump();
	void gravity();//tính toán tác động của trọng lực đến nhân vật người chơi.
	void getHit(vector<Monster*> &monsterList, Mix_Chunk* p_sfx[], SDL_Rect& camera);
	bool isDead() { return dead; }

	bool Won() { return won; }

	void knockBack();
	void render(SDL_Rect& p_camera);//vẽ player lên màn hình
	void handleCamera(SDL_Rect& camera, float& camVel);//xử lí camera trong trò chơi: lesson 30

	SDL_Rect getCollision() const { return collision; } // trả về hình chữ nhật va chạm của nhân vật người chơi.
	void setBulletList(vector<Bullet*> bulletList) { this->bulletList = bulletList; }
	vector<Bullet*> getBulletList() const { return bulletList; } //trả về danh sách đạn được bắn ra bởi nhân vật người chơi.
    
	void resetPlayer() {
		xPos = 64 * 3; 
		yPos = LEVEL_HEIGHT - TILE_HEIGHT * 4;
		xVel = 0;
		yVel = 0;
		won = false;
		dead = false;
	}

};