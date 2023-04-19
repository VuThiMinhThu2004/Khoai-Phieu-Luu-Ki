#include "Monster.h"

//lesson14
Monster::Monster(float p_x, float p_y, SDL_Texture* p_tex) : Entity(p_x, p_y, p_tex) {
	collision.x = getX() + MONSTER_WIDTH;
	collision.y = getY() +  MONSTER_HEIGHT;
	collision.w =  MONSTER_WIDTH;
	collision.h =  MONSTER_HEIGHT;

	for (int i = 0; i < WALKING_ANIMATION_FRAMES; i++) {
		walkingClips[i].x = i * (getFrame().w / 4);
		if (i >= 4) {
			walkingClips[i].x = (i - 4) * (getFrame().w / 4);
			walkingClips[i].y = (getFrame().h / 5) * 2;
		}
		else walkingClips[i].y = getFrame().h / 5;
		walkingClips[i].w = getFrame().w / 4;
		walkingClips[i].h = getFrame().h / 5;
	}
	for (int i = 0; i < IDLING_ANIMATION_FRAMES; i++) {
		idlingClips[i].x = i * (getFrame().w / 4);
		idlingClips[i].y = 0;
		idlingClips[i].w = getFrame().w / 4;
		idlingClips[i].h = getFrame().h / 5;
	}
	for (int i = 0; i < FALLING_ANIMATION_FRAMES; i++) {
		fallingClips[i].x = i * (getFrame().w / 4);
		fallingClips[i].y = (getFrame().h / 5) * 4;
		fallingClips[i].w = getFrame().w / 4;
		fallingClips[i].h = getFrame().h / 5;
	}
	for (int i = 0; i < ATTACKING_ANIMATION_FRAMES; i++) {
		attackingClips[i].x = i * (getFrame().w / 4);
		attackingClips[i].y = (getFrame().h / 5) * 4;
		attackingClips[i].w = getFrame().w / 4;
		attackingClips[i].h = getFrame().h / 5;
	}
}

void Monster::move(Player& p_player) {
	float distance = sqrt(pow(p_player.getX() - getX(), 2) + pow(p_player.getY() - getY(), 2));
	if ((p_player.getY() >= getY() - 64 && p_player.getY() <= getY() + 64) && distance <= 64 * 2) {
		xVel = MONSTER_VEL;
	}
	else xVel = 0;
}

void Monster::update(Tile* tile[]) {
	// set trạng thái Monster
	if (xVel == 0 && yVel == 0 && grounded) idling = true;
	else idling = false;

	if (xVel != 0 && grounded) walking = true;
	else walking = false;

	if (xVel < 0) flipType = SDL_FLIP_HORIZONTAL;
	if (xVel > 0) flipType = SDL_FLIP_NONE;

	if (yVel > 0 && !grounded) falling = true;
	else falling = false;

	//move x
	xPos += xVel;
	collision.x = getX() + MONSTER_WIDTH;

	//move y
	yPos += yVel;
	collision.y = getY() + MONSTER_HEIGHT;
}

void Monster::render(SDL_Rect& p_camera) {
	if (walking) {
		commonFunc::renderAnimation(tex, xPos, yPos, walkingClips[walkCounter / 4], p_camera, 0, NULL, getFlipType());
		walkCounter++;
		if (walkCounter / 4 >= WALKING_ANIMATION_FRAMES) walkCounter = 0;
	}
	else walkCounter = 0;

	if (idling) {
		commonFunc::renderAnimation(tex, xPos, yPos, idlingClips[idleCounter / 6], p_camera, 0, NULL, getFlipType());
		idleCounter++;
		if (idleCounter / 6 >= IDLING_ANIMATION_FRAMES) idleCounter = 0;
	}
	else idleCounter = 0;

	if (falling) {
		commonFunc::renderAnimation(tex, xPos, yPos, fallingClips[fallingCounter / 4], p_camera, 0, NULL, getFlipType());
		fallingCounter++;
		if (fallingCounter / 4 >= FALLING_ANIMATION_FRAMES) fallingCounter = 0;
	}
	else fallingCounter = 0;
}