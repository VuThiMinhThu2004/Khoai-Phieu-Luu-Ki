#include "Bullet.h"

//lzayfoo_11
Bullet::Bullet(int p_x, int p_y, SDL_Texture* p_tex) : Entity(p_x, p_y, p_tex) {
	collision.x = xPos;
	collision.y = yPos;

	for (int i = 0; i < BULLET_ANIMATION_FRAMES; i++) {
		bulletClips[i].x = i * DEFAULTBULLET_W;
		bulletClips[i].y = 0;
		bulletClips[i].w = DEFAULTBULLET_W;
		bulletClips[i].h = DEFAULTBULLET_H;
	}
}

void Bullet::update(vector<LevelPart>& LevelPartList) {
	if (bulletFrame/delay < 5) {
		xVel += BULLETSPEED;
		
		if (getFlipType() == SDL_FLIP_HORIZONTAL) xPos -= xVel;
		else if (getFlipType() == SDL_FLIP_NONE) xPos += xVel;

		collision.x = getX();

		if (getX() < 0) {
			xPos = 0;
			collision.x = getX();
		}
		
		if (commonFunc::touchesWood(collision, LevelPartList)) {
			if (getFlipType() == SDL_FLIP_HORIZONTAL) xPos += xVel;
			else if (getFlipType() == SDL_FLIP_NONE) xPos -= xVel;
			collision.x = getX();
		}
	}
	if (bulletFrame/delay == BULLET_ANIMATION_FRAMES) setMove(false);
}


void Bullet::setFlipType(SDL_RendererFlip p_PlayerflipType) {
	flipType = p_PlayerflipType;
}

void Bullet::setWidthHeight(const int& p_width, const int& p_height, const int& p_playerX) {
	if (getFlipType() == SDL_FLIP_HORIZONTAL) {
		xPos = p_playerX - (DEFAULTBULLET_W / 5);
	}

	collision.w = DEFAULTBULLET_W;
	collision.h = DEFAULTBULLET_H-1;
}

void Bullet::render(SDL_Rect &camera, SDL_Texture* p_tex) {
	commonFunc::renderAnimation(p_tex, getX(), getY(), bulletClips[bulletFrame/5], camera, 0, NULL, getFlipType());
	bulletFrame++;
}