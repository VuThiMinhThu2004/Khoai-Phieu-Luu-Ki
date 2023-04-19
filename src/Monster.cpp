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

    for (int i = 0; i < BEINGHIT_ANIMATION_FRAMES; i++) {
		beingHitClips[i].x = i * (getFrame().w / 4);
		beingHitClips[i].y = (getFrame().h / 5) * 2;
		beingHitClips[i].w = getFrame().w / 4;
		beingHitClips[i].h = getFrame().h / 5;
	}
}


void Monster::update(Player& p_player, Tile* tile[]) {
	if (!beingHit) {
		if (xVel < 0) flipType = SDL_FLIP_HORIZONTAL;//trái
		if (xVel > 0) flipType = SDL_FLIP_NONE;//phải
	}

	gravity();
	getHit(p_player);//nếu bị tấn công: sẽ thực hiện hành động knockback, cập nhật giá trị của các biến beingHit, knockBackTime, knockBackDir và health.
	autoMovement(tile);
	moveToPlayer(p_player, tile);
	knockBack();// lùi lại khi bị tấn công
	
	//update trạng thái monster
	if (xVel == 0 && grounded && !attacking && !dead && !beingHit) idling = true;
	else idling = false;

	if (xVel != 0 && grounded && !attacking && !dead && !beingHit) walking = true;
	else walking = false;

	if (yVel > 0 && !grounded && !dead && !beingHit) falling = true;
	else falling = false;

    //ls 27
	//move x
	if (!attacking) {
		xPos += xVel;
		collision.x = getX() + MONSTER_WIDTH;//cập nhật va chạm collision

		if (getX() + MONSTER_WIDTH < 0) {//vượt ra khỏi biên độ màn hình bên trái 
			xPos = -MONSTER_WIDTH;//đặt x = vị trí ngoài cùng bên phải của màn hình
			collision.x = getX() + MONSTER_WIDTH;
			xVel *= -1;//đổi chiều di chuyển
		}
		if (getX() + 2 * MONSTER_HEIGHT > LEVEL_WIDTH) {
			xPos = LEVEL_WIDTH - 2 * MONSTER_HEIGHT;
			collision.x = getX() + MONSTER_WIDTH;
			xVel *= -1;
		}
		if (commonFunc::touchesWood(collision, tile)) {
			xPos -= xVel;
			collision.x = getX() + MONSTER_WIDTH;
			xVel *= -1;
		}
	}

	//move y
	yPos += yVel;
	collision.y = getY() + MONSTER_HEIGHT;
	if (getY() + MONSTER_HEIGHT < 0) {//đối tượng không quá phần trên màn hình
		yPos = -MONSTER_HEIGHT;
		collision.y = getY() + MONSTER_HEIGHT;
	}
	if (commonFunc::touchesWood(collision, tile, groundSTT)) {
		if (yVel > 0) {//đối tượng đang rơi
			yPos = tile[groundSTT]->getY() - 64 * 2 - 0.1 + 2;
			if (falling) {//nếu trước đấy đang rơi
				grounded = true;
			}
		}
		else if (yVel < 0) {//đang nhảy lên
			yPos -= yVel;//để đt k còn bay lên
			yVel = 0;
		}
		collision.y = getY() + MONSTER_HEIGHT;
	}
	else grounded = false;
	
}

void Monster::gravity() {
	if (!grounded) {
		yVel += GRAVITY;
		if (yVel > MAX_GRAVITY) yVel = MAX_GRAVITY;
	}
	else yVel = GRAVITY;
}

void Monster::moveToPlayer(Player& p_player, Tile* tile[]) {
	distanceToPlayer = sqrt(pow(p_player.getX() - getX(), 2) + pow(p_player.getY() - getY(), 2));
	if (!beingHit) {
		//trong tầm nhìn của Monster
		if ((p_player.getY() >= getY() - TILE_WIDTH && p_player.getY() <= getY() + TILE_WIDTH * 0.5) && distanceToPlayer <= TILE_WIDTH * 7) {
			if (p_player.getX() - getX() < 0) {
				if (tile[groundSTT - 1]->getType() > 84) xVel = 0;
				else xVel = -MONSTER_VEL;
			}
			else if (tile[groundSTT + 1]->getType() > 84) xVel = 0;
			else xVel = MONSTER_VEL;
		}
	}
	if ( (distanceToPlayer <= TILE_WIDTH * 1.5 || (p_player.getY() >= getY() - TILE_WIDTH * 2.5 && p_player.getY() <= getY() - 64 && distanceToPlayer <= TILE_WIDTH * 2.5)) && !dead && !beingHit && grounded) 
        attacking = true;
	else attacking = false;
}

void Monster::autoMovement(Tile* tile[]) {
	if (grounded && !beingHit) {
		if (tile[groundSTT + 1]->getType() > 84 && tile[groundSTT - 2]->getType() > 84) xVel = 0;
		else if (tile[groundSTT + 1]->getType() > 84 && xVel > 0) xVel = -MONSTER_VEL * 0.5;
		else if (tile[groundSTT - 1]->getType() > 84 && xVel < 0) xVel = MONSTER_VEL * 0.5;
		else if (tile[groundSTT + 2]->getType() > 84 && tile[groundSTT - 2]->getType() > 84) xVel = 0;
		else if(getFlipType() == SDL_FLIP_NONE) xVel = MONSTER_VEL * 0.5;
		else if(getFlipType() == SDL_FLIP_HORIZONTAL) xVel = -MONSTER_VEL * 0.5;
	}
}

bool Monster::isAttacking() {
	if (attackingCounter / 7 >= 2) return true;
	return false;
}

void Monster::getHit(Player& p_player) {
	for (int i = 0; i < p_player.getBulletList().size(); i++) {
		if (p_player.getBulletList().at(i) != NULL) {
			if (commonFunc::checkCollision(p_player.getBulletList().at(i)->getCollision(), getCollision())) {
				if (p_player.getBulletList().at(i)->getX() >= getX() + MONSTER_WIDTH && p_player.getBulletList().at(i)->getX() <= getX()+ MONSTER_WIDTH*1.5) {
					beingHit = true;
					maxHealth--;
					p_player.getBulletList().at(i)->setMove(false);
				}
			}
		}
	}
	if (beingHitCounter / 7 >= BEINGHIT_ANIMATION_FRAMES) {
		beingHit = false;
		beingHitCounter = 0;
	}
	if (maxHealth <= 0) dead = true;
}

void Monster::knockBack() {
	if (beingHit && beingHitCounter==0) {
		yVel = -3;
		if (getFlipType() == SDL_FLIP_NONE) xVel = -4;
		else if(getFlipType() == SDL_FLIP_HORIZONTAL ) xVel = 4;
	}
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

    if (attacking) {
		commonFunc::renderAnimation(tex, xPos, yPos, attackingClips[attackingCounter / 7], p_camera, 0, NULL, getFlipType());
		attackingCounter++;
		if (attackingCounter / 7 >= ATTACKING_ANIMATION_FRAMES) attackingCounter = 0;
	}
	else attackingCounter = 0;

	if (beingHit) {
		commonFunc::renderAnimation(tex, xPos, yPos, beingHitClips[beingHitCounter / 7], p_camera, 0, NULL, getFlipType());
		beingHitCounter++;
	}
	else beingHitCounter = 0;
}