#include "Bullet.h"
#include "Entity.h"
Bullet::Bullet(float p_x, float p_y, SDL_Texture* p_tex) : Entity(p_x, p_y, p_tex) {
	collision.x = xPos;
	collision.y = yPos;
	for (int i = 0; i < BULLET_ANIMATION_FRAMES; i++) {
		bulletClips[i].x = i * DEFAULTBULLET_W;
		bulletClips[i].y = 0;
		bulletClips[i].w = DEFAULTBULLET_W;
		bulletClips[i].h = DEFAULTBULLET_H;
	}
}


void Bullet::handelInput(SDL_Event& events) {

}

void Bullet::move() {
	if (flipType == SDL_FLIP_NONE) xPos += BULLETSPEED;
	else xPos -= BULLETSPEED;
	
	collision.x = xPos;
}
void Bullet::setFlipType(SDL_RendererFlip p_PlayerflipType) {
	flipType = p_PlayerflipType;
	 
}
void Bullet::setWidthHeight(const int& p_width, const int& p_height, const int& p_playerX) {
	if (getFlipType() == SDL_FLIP_HORIZONTAL) {
		xPos = p_playerX - (DEFAULTBULLET_W / 4);
	}
	collision.w = DEFAULTBULLET_W;
	collision.h = DEFAULTBULLET_H;
}
void Bullet::render(SDL_Rect &camera, SDL_Texture* p_tex) {
	commonFunc::renderAnimation(p_tex, getX(), getY(), bulletClips[bulletCounter/5], camera, 0, NULL, getFlipType());
	bulletCounter++;
}