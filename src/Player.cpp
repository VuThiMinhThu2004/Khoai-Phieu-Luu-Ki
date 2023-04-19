#include "Player.h"

Player::Player(float p_x, float p_y, SDL_Texture* p_tex) : Entity(p_x, p_y, p_tex) {
	
	//khởi tạo tọa độ, texture và collision của Player, cũng như các frame cho animation.
	
	collision.x = getX() + PLAYER_WIDTH;
	collision.y = getY() + PLAYER_HEIGHT;
	collision.w = PLAYER_WIDTH-12; //cho vừa với chân nhân vật
	collision.h = PLAYER_HEIGHT;


	for (int i = 0; i < WALKING_ANIMATION_FRAMES; i++) {
		walkingClips[i].x = i * (getFrame().w/4);
		if (i >= 4) {
			walkingClips[i].x = (i-4) * (getFrame().w/4);
			walkingClips[i].y = (getFrame().h/6)*2;
		}
		else walkingClips[i].y = getFrame().h/6;
		walkingClips[i].w = getFrame().w/4;
		walkingClips[i].h = getFrame().h/6;
	}

	for (int i = 0; i < IDLING_ANIMATION_FRAMES; i++) {
		idlingClips[i].x = i * (getFrame().w/4);
		idlingClips[i].y = 0;
		idlingClips[i].w = getFrame().w/4;
		idlingClips[i].h = getFrame().h/6;
	}

	for (int i = 0; i < JUMPING_ANIMATION_FRAMES; i++) {
		jumpingClips[i].x = i * (getFrame().w / 4);
		jumpingClips[i].y = (getFrame().h/6) * 3;
		jumpingClips[i].w = getFrame().w / 4;
		jumpingClips[i].h = getFrame().h / 6;
	}

	for (int i = 0; i < FALLING_ANIMATION_FRAMES; i++) {
		fallingClips[i].x = i * (getFrame().w / 4);
		fallingClips[i].y = (getFrame().h / 6) * 4;
		fallingClips[i].w = getFrame().w / 4;
		fallingClips[i].h = getFrame().h / 6;
	}
	
	for (int i = 0; i < DEATH_ANIMATION_FRAMES; i++) {
		deathClips[i].x = i * (getFrame().w / 4);
		deathClips[i].y = (getFrame().h / 6) * 5;
		deathClips[i].w = getFrame().w / 4;
		deathClips[i].h = getFrame().h / 6;
	}
}

//lazyfoo
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
