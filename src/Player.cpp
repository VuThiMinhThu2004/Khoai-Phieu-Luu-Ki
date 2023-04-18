#include "Player.h"

void Player::handleInputActive(SDL_Event &events) {
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

//lesson 30
void Player::handleCamera(SDL_Rect& camera, float& camVel) {
	float acc = 0.001;
	// nếu camVel đã vượt quá một mức độ nhất định, tốc độ tăng sẽ giảm dần (acc giảm dần).	
	if (camVel > 4) acc = 0.0003;
	if (camVel > 5) acc = 0.00001;
	camVel += acc; - SCREEN_HEIGHT / 2;
/*giới hạn di chuyển camera: Camera sẽ không di chuyển quá giới hạn màn hình chơi.
nếu người chơi đến gần phía bên phải màn hình (từ đầu màn hình tới 2/3 chiều rộng của màn hình)
camera sẽ dừng di chuyển trên trục x và chỉ di chuyển theo trục y để giữ cho người chơi luôn nằm giữa màn hình
*/
	//Keep the camera in bounds
	if (getX() + PLAYER_WIDTH / 2 - camera.x >= SCREEN_WIDTH * 2 / 3) {
		camera.x = (getX() + PLAYER_WIDTH / 2) - SCREEN_WIDTH * 2 / 3;
	}
	camera.y = (getY() + PLAYER_HEIGHT / 2) - SCREEN_HEIGHT / 2;

	//Giới hạn: camera di chuyển quá xa về bên trái, bên trên, hoặc bên dưới màn hình, camera sẽ bị giới hạn lại để tránh hiển thị ra khỏi vùng chơi của game.
	if (camera.x < 0) {
		camera.x = 0;
	}

	if (camera.y < 0) {
		camera.y = 0;
	}
	
	if (camera.y > LEVEL_HEIGHT - camera.h) {
		camera.y = LEVEL_HEIGHT - camera.h;
	}
}
