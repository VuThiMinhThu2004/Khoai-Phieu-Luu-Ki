#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <sstream>

#include "RenderWindow.h"
#include "Entity.h"
#include "Player.h"


void Player::handleInput(SDL_Event &events) {
	if (events.type == SDL_KEYDOWN && events.key.repeat == 0) {
		switch (events.key.keysym.sym) {
		case SDLK_s:
			yVel += PLAYER_VEL;
			break;
		case SDLK_a:
			xVel -= PLAYER_VEL;
			break;
		case SDLK_d:		
			xVel += PLAYER_VEL;
			break;
		case SDLK_SPACE:
			if (grounded) {
				jump();
			}
			break;
		default:
			break;
		}
	}
	else if (events.type == SDL_KEYUP && events.key.repeat == 0) {
		switch (events.key.keysym.sym) {
		case SDLK_s:
			yVel -= PLAYER_VEL;
			break;
		case SDLK_a:
			xVel += PLAYER_VEL;
			break;
		case SDLK_d:
			xVel -= PLAYER_VEL;
			break;
		default:
			break;
		}
	}
	else if (events.type == SDL_MOUSEBUTTONDOWN && events.key.repeat == 0) {

	}
	else if (events.type == SDL_MOUSEBUTTONUP && events.key.repeat == 0) {

	}
}

void Player::update() {
	//move x
	xPos += xVel;
	if (xPos < 0) xPos = 0;
	if (xPos > LEVEL_WIDTH - currentFrame.w) xPos = LEVEL_WIDTH - currentFrame.w;

	//move y
	yPos += yVel;
	if (yPos < 0) yPos = 0;
	if (yPos > LEVEL_HEIGHT - currentFrame.h) yPos = LEVEL_HEIGHT - currentFrame.h;

	//Gravity
	if (!grounded) {
		yVel += GRAVITY;
	}
}

bool Player::jump() {
	if (grounded) {
		yVel -= 5;
		grounded = false;
		return true;
	}
	return false;
}


void Player::handleCamera() {
	//Di chuyển camera theo nhân vật
	camera.x = (getX() + getCurrentFrame().w / 2) - SCREEN_WIDTH / 2;
	camera.y = (getY() + getCurrentFrame().h / 2) - SCREEN_HEIGHT / 2;

	//Giới hạn 
	if (camera.x < 0) {
		camera.x = 0;
	}
	if (camera.y < 0) {
		camera.y = 0;
	}
	if (camera.x > LEVEL_WIDTH - camera.w) {
		camera.x = LEVEL_WIDTH - camera.w;
	}
	if (camera.y > LEVEL_HEIGHT - camera.h) {
		camera.y = LEVEL_HEIGHT - camera.h;
	}
}

SDL_Rect Player::getCamera() {
	return camera;
}