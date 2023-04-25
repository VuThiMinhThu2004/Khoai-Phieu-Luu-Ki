#include "Entity.h"

// Hàm khởi tạo các biến x, y, tex và currentFrame
Entity::Entity(float p_x, float p_y, SDL_Texture* p_tex) 
		: xPos(p_x), yPos(p_y), tex(p_tex)
{
	currentFrame.x = 0;
	currentFrame.y = 0;

	//kích thước w, h được lấy từ texture p_tex bằng SDL_QueryTexture.
	SDL_QueryTexture(p_tex, NULL, NULL, &currentFrame.w, &currentFrame.h);
}

float Entity::getX() {
	return xPos;
}

float Entity::getY() {
	return yPos;
}

//hàm getter trả về con trỏ tới texture của Entity.
SDL_Texture* Entity::getTex() {
	return tex;
}

SDL_Rect Entity::getFrame() {//lesson 14
	return currentFrame;
}

SDL_RendererFlip Entity::getFlipType() {//lesson 15
	return flipType;
}