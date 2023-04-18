#pragma once

#include "RenderWindow.h"

using namespace std;

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

	bool grounded = true;
	float xVel = 0, yVel = 0;//vận tốc theo trục x và y của Player
	
	SDL_Rect collision;//lưu trữ kích thước và vị trí của Player để xử lý va chạm.
//	float camVel = 1.5;
public:
	Player(float p_x, float p_y, SDL_Texture* p_tex);//khởi tạo player với tọa đồ và hình ảnh tương ứng
	

	void handleInputActive(SDL_Event &events);
	void update();
	bool jump();
	void gravity();//tính toán tác động của trọng lực đến nhân vật người chơi.

	void render(SDL_Rect& p_camera);//vẽ player lên màn hình
	void handleCamera(SDL_Rect& camera, float& camVel);//xử lí camera trong trò chơi: lesson 30

	SDL_Rect getCollision() const { return collision; } // trả về hình chữ nhật va chạm của nhân vật người chơi.

};