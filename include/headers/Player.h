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
	
	SDL_Rect collision;//lưu trữ kích thước và vị trí của Player để xử lý va chạm.
	vector<Bullet*> bulletList;// lưu trữ danh sách các đạn mà Player bắn ra.
	Mix_Chunk* playerSFX[4];

	bool won = false;
public:
	Player(int p_x, int p_y, SDL_Texture* p_tex);//khởi tạo player với tọa đồ và hình ảnh tương ứng
	
	int maxHP = 100;

	enum SFX {//hằng số để đại diện cho các âm thanh và hiệu ứng hình ảnh được sử dụng trong trò chơi.
		hitSFX = 0,
		jumpSFX = 1,
		landSFX = 2,
		shootSFX = 3,
	};

	void handleInputActive(SDL_Event &events, Mix_Chunk* p_sfx[]);
	//cập nhật trạng thái của nhân vật người chơi, ví dụ như di chuyển, va chạm với các vật thể khác, và xử lý các sự kiện trong trò chơi.
	void update(vector<LevelPart>& LevelPartList, vector<Monster*> &skeletonList, Mix_Chunk* p_sfx[], SDL_Rect& camera);
	void jump();
	void gravity();//tính toán tác động của trọng lực đến nhân vật người chơi.
	void getHit(vector<Monster*> &skeletonList, Mix_Chunk* p_sfx[], SDL_Rect& camera);//xử lý khi nhân vật người chơi bị tấn công bởi quái vật.
	bool isDead() { return dead; }
	void knockBack();
	void handleCamera(SDL_Rect& camera, float& camVel);//xử lí camera trong trò chơi: lesson 30
	void render(SDL_Rect& p_camera);//vẽ player lên màn hình
	void resetPlayer() {//khởi tạo lại trạng thái ban đầu của nhân vật người chơi.
		
		//thiết lập các biến(các trạng thái) thành giá trị ban đầu 
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

	//getter
	vector<Bullet*> getBulletList() const { return bulletList; } //trả về danh sách đạn được bắn ra bởi nhân vật người chơi.
	SDL_Rect getCollision() const { return collision; } // trả về hình chữ nhật va chạm của nhân vật người chơi.
};