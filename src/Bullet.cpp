#include "Bullet.h"

Bullet::Bullet(float p_x, float p_y, SDL_Texture* p_tex) : Entity(p_x, p_y, p_tex) {
	collision.x = xPos;
	collision.y = yPos;
}

void Bullet::handelInput(SDL_Event& events) {

}

void Bullet::move() {
	if (flipType == SDL_FLIP_NONE) xPos -= BULLETSPEED;
	else xPos += BULLETSPEED;
	
	collision.x = xPos;
}

void Bullet::setFlipType(SDL_RendererFlip p_PlayerflipType) {
	flipType = p_PlayerflipType;
}

void Bullet::setWidthHeight(const int& p_width, const int& p_height) {
	collision.w = DEFAULTBULLET_W;
	collision.h = DEFAULTBULLET_H;
}

void Bullet::render(SDL_Rect &camera, SDL_Texture* p_tex) {
	SDL_Rect src = { 0, 0, DEFAULTBULLET_W, DEFAULTBULLET_H };
	commonFunc::renderAnimation(p_tex, getX(), getY(), src, camera);
}