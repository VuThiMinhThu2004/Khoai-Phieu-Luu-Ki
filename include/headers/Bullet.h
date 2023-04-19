#pragma once

#include "RenderWindow.h"
#include "Entity.h"
#include "LevelPart.h"

using namespace std;

//lesson 26
const int DEFAULTBULLET_W = 64;
const int DEFAULTBULLET_H = 64;

const int BULLETSPEED = 3;

class Bullet : public Entity {
private:
	bool moving = false;
	int bulletType;
	SDL_Rect collision;
	float xVel = 7;
	
	static const int BULLET_ANIMATION_FRAMES = 6;
	SDL_Rect bulletClips[BULLET_ANIMATION_FRAMES];
	int bulletFrame = 0;
	const int delay = 5;//tính thời gian trễ giữa các frame trong animation.

public:
	enum bulletType {
		NONE = 0,
		NORMAL = 1,
	};
	Bullet(float p_x, float p_y, SDL_Texture* p_tex);

	void update(vector<LevelPart>& LevelPartList);
	void render(SDL_Rect& camera, SDL_Texture* p_tex);

	//getter
	int getType() const { return bulletType; }
	bool isMoving() const { return moving; }
	SDL_Rect getCollision() { return collision; }

	//setter: thay đổi các thuộc tính của đạn: loại, tình trạng di chuyeernm phần va chạm
	void setFlipType(SDL_RendererFlip p_PlayerflipType);
	void setType(const int& p_type) { bulletType = p_type; }
	void setMove(bool p_move) { moving = p_move; }
	void setWidthHeight(const int &p_width, const int &p_height, const int &p_playerX);
};
