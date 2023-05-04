#include "Player.h"

//lesson 14
Player::Player(int p_x, int p_y, SDL_Texture* p_tex) : Entity(p_x, p_y, p_tex) {
		
	collision.x = getX() + PLAYER_WIDTH;
	collision.y = getY() + PLAYER_HEIGHT;
	collision.w = PLAYER_WIDTH-12;
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

//lazyfoo 26
void Player::handleInputActive(SDL_Event &events, Mix_Chunk* p_sfx[]) {
	if (!isDead()) {
		if (events.type == SDL_KEYDOWN && events.key.repeat == 0) {
			switch (events.key.keysym.sym) {
			case SDLK_a:
				xVel -= PLAYER_VEL;
				break;
			case SDLK_d:
				xVel += PLAYER_VEL;
				break;
			case SDLK_w:
				if (grounded) {
					jump();
					Mix_PlayChannel(-1, p_sfx[jumpSFX], 0);
				}
				break;
			default:
				break;
			}
		}
		else if (events.type == SDL_KEYUP && events.key.repeat == 0) {
			switch (events.key.keysym.sym) {
			case SDLK_a:
				xVel += PLAYER_VEL;
				break;
			case SDLK_d:
				xVel -= PLAYER_VEL;
				break;
			case SDLK_w:
				if (!grounded && jumping) {
					yVel *= .5f;
				}
				break;
			default:
				break;
			}
		}
		else if (events.type == SDL_MOUSEBUTTONDOWN && events.key.repeat == 0) {//lesson 17
			if (events.button.button == SDL_BUTTON_LEFT) {
				Bullet* bullet = new Bullet(getCollision().x + PLAYER_WIDTH * 1.25, getCollision().y, NULL);
				
				Mix_PlayChannel(-1, p_sfx[shootSFX], 0);
				bullet->setFlipType(getFlipType());
				bullet->setWidthHeight(DEFAULTBULLET_W, DEFAULTBULLET_H, getX());
				bullet->setType(Bullet::NORMAL);//đặt loại đạn
				bullet->setMove(true);//move=true->cho phep dan dichuyen
				bulletList.push_back(bullet);
			}
			
		}
		else if (events.type == SDL_MOUSEBUTTONUP && events.key.repeat == 0) {

		}
	}
}


void Player::update(vector<LevelPart>& LevelPartList, vector<Monster*> &monsterList, Mix_Chunk* p_sfx[], SDL_Rect& camera) {
	gravity();
	if(!dead) getHit(monsterList, p_sfx, camera);

	// set trạng thái Player
	if (xVel == 0 && grounded && !dead) idling = true;
	else idling = false;

	if (xVel != 0 && grounded && !dead) running = true;
	else running = false;

	if (yVel > 0 && !grounded && !dead) falling = true;
	else falling = false;

	if (yVel <= 0 && !grounded && !dead) jumping = true;
	else jumping = false;

	if (!beingHit) {
		if (xVel < 0) flipType = SDL_FLIP_HORIZONTAL;
		if (xVel > 0) flipType = SDL_FLIP_NONE;
	}

	//lesson 26: di chuyển nhân vật theo trục x và y nếu nhân vật chưa chết.
	//move x
	if (!dead) {
		xPos += xVel;//xem lai
		collision.x = getX() + PLAYER_WIDTH;


		if (getX() + PLAYER_WIDTH < 0) {
			xPos = -PLAYER_WIDTH;
			collision.x = getX() + PLAYER_WIDTH;
		}

		if (commonFunc::touchesWood(collision, LevelPartList)) {
			xPos -= xVel;
			collision.x = getX() + PLAYER_WIDTH;
		}
	}

	//move y
	yPos += yVel;
	collision.y = getY() + PLAYER_HEIGHT;

	if (getY() + PLAYER_HEIGHT < 0) {
		yPos = -PLAYER_HEIGHT;
		collision.y = getY() + PLAYER_HEIGHT;
	}

	if (commonFunc::touchesWood(collision, LevelPartList, grounded, groundSTT, levelSTT)) {
		
		if (yVel > 0) {
			//đặt lại vị trí y của Player để nằm trên bức tường hoặc đất
			yPos = LevelPartList.at(levelSTT).getTilesList().at(groundSTT)->getY() - 64 * 2;
			if (falling) {
				grounded = true;
				Mix_PlayChannel(-1, p_sfx[landSFX], 0);
			}
		}
		else if (yVel < 0) {
			yPos -= yVel;
			yVel = 0;//dừng việc nhảy
		}
		collision.y = getY() + PLAYER_HEIGHT;
	}
}


void Player::jump() {
	if (grounded) {
		yVel -= 11;
		grounded = false;//đang nhảy
	}
}

void Player::gravity() {
	if (!grounded) {
		yVel += GRAVITY;
		if (yVel > MAX_GRAVITY) yVel = MAX_GRAVITY;
	}
	else yVel = GRAVITY;//nv k bay lên khi đang đứng yên
}


//lesson 30:copy lazyfoo
void Player::handleCamera(SDL_Rect& camera, float& camVel) {
	
	if(!isDead()) camera.x += camVel;
	
	float acc = 0.002;
	// nếu camVel đã vượt quá một mức độ nhất định, tốc độ tăng sẽ giảm dần
	if (camVel > 4) acc = 0.0003;
	if (camVel > 5) acc = 0.00002;

	camVel += acc;

	//Keep the camera in bounds
	if (getX() + PLAYER_WIDTH / 2 - camera.x >= SCREEN_WIDTH * 2 / 3) {
		camera.x = (getX() + PLAYER_WIDTH / 2) - SCREEN_WIDTH * 2 / 3;
	}
	camera.y = (getY() + PLAYER_HEIGHT / 2) - SCREEN_HEIGHT / 2;

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

void Player::getHit(vector<Monster*> &monsterList, Mix_Chunk* p_sfx[], SDL_Rect& camera) {
	for (int i = 0; i < monsterList.size(); i++) {
		if(monsterList.at(i) != NULL) {
			if ((monsterList.at(i)->getDistance() <= TILE_WIDTH * 1.5 && monsterList.at(i)->isAttacking() && getY() >= monsterList.at(i)->getY() - TILE_WIDTH && getY() <= monsterList.at(i)->getY() + TILE_WIDTH * 0.5)) {
				maxHP--;		
				yVel--;
				break;
			}
			cerr << maxHP << endl;
		}
	}
	/*
	nếu Player đang ở trên màn hình và chạm đáy màn hình hoặc đi quá xa về bên trái (vượt quá giới hạn của camera), 
	Player cũng sẽ chết và phát âm thanh hitSFX.
	*/
	if (getY() + PLAYER_HEIGHT >= LEVEL_HEIGHT || getX() - camera.x <= 64) {
		maxHP -= 10;
	}

	if (maxHP <= 0) {
		dead = true;
		Mix_PlayChannel(-1, p_sfx[hitSFX], 0);
	}
}

void Player::knockBack() {
	if (beingHit && beingHitFrame == 0) {//da bi tan cong
		yVel -= 4;
		if (getFlipType() == SDL_FLIP_NONE) xPos -= 10;//xem lai
		else if (getFlipType() == SDL_FLIP_HORIZONTAL) xPos += 10;
	}
}

//lazyfoo_14
void Player::render(SDL_Rect &p_camera) {
	if (running) {
	//sử dụng các sprite từ mảng walkingClips để tạo hiệu ứng chạy. 
		commonFunc::renderAnimation(tex, xPos, yPos, walkingClips[walkFrame / 4], p_camera, 0, NULL, getFlipType());//hiển thị hình ảnh của nhân vật tương ứng với trạng thái đó.
		walkFrame++;

		////Cycle animation
		if (walkFrame / 4 >= WALKING_ANIMATION_FRAMES) walkFrame = 0; 
	}

	if (idling) {
		commonFunc::renderAnimation(tex, xPos, yPos, idlingClips[idleFrame/6], p_camera, 0, NULL, getFlipType());
		idleFrame++;

		if (idleFrame / 6 >= IDLING_ANIMATION_FRAMES) idleFrame = 0;
	}
	else idleFrame = 0;

	if (jumping) {
		commonFunc::renderAnimation(tex, xPos, yPos, jumpingClips[jumpFrame / 5], p_camera, 0, NULL, getFlipType());
		jumpFrame++;
		
		if (jumpFrame / 5 >= JUMPING_ANIMATION_FRAMES) jumpFrame = 0;
	}
	else jumpFrame = 0;

	if (falling) {
		commonFunc::renderAnimation(tex, xPos, yPos, fallingClips[fallingFrame / 4], p_camera, 0, NULL, getFlipType());
		fallingFrame++;
		
		if (fallingFrame / 4 >= FALLING_ANIMATION_FRAMES) fallingFrame = 0;
	}
	else fallingFrame = 0;
	
	if (beingHit) {
		commonFunc::renderAnimation(tex, xPos, yPos, idlingClips[idleFrame/6], p_camera, 0, NULL, getFlipType());
		beingHitFrame++;
	}
	else beingHitFrame = 0;

	if (dead) {
		commonFunc::renderAnimation(tex, xPos, yPos, deathClips[deathFrame / 5], p_camera, 0, NULL, getFlipType());
		if(deathFrame / 5 < DEATH_ANIMATION_FRAMES-1) deathFrame++;
	}
	else deathFrame = 0;
}
