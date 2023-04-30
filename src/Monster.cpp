#include "Monster.h"
#include <cmath>

//lesson14
Monster::Monster(int p_x, int p_y, SDL_Texture* p_tex) : Entity(p_x, p_y, p_tex) {
	collision.x = getX() + MONSTER_WIDTH;
	collision.y = getY() +  MONSTER_HEIGHT;
	collision.w =  MONSTER_WIDTH - 12;
	collision.h =  MONSTER_HEIGHT - 2;

	for (int i = 0; i < WALKING_ANIMATION_FRAMES; i++) {
		walkingClips[i].x = i * (getFrame().w / 4);
		walkingClips[i].y = getFrame().h / 5;
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
		attackingClips[i].y = (getFrame().h / 5) * 3;
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


void Monster::update(Player& p_player, vector<LevelPart>& LevelPartList, Mix_Chunk* p_sfx[], SDL_Rect& camera) {	
	if (!beingHit) {
		if (xVel < 0) flipType = SDL_FLIP_HORIZONTAL;//trái
		if (xVel > 0) flipType = SDL_FLIP_NONE;//phải
	}

	gravity();//cap nhat yvel
	getHit(p_player, p_sfx, camera);//nếu bị tấn công: sẽ thực hiện hành động knockback, cập nhật giá trị của các biến beingHit, knockBackTime, knockBackDir và health.
	autoMovement(LevelPartList);
	moveToPlayer(p_player, LevelPartList);
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

		// if (getX() + 2 * MONSTER_HEIGHT > LEVEL_WIDTH) {//xem lai
		// 	xPos = LEVEL_WIDTH - 2 * MONSTER_HEIGHT;
		// 	collision.x = getX() + MONSTER_WIDTH;
		// 	xVel *= -1;
		// }

		if (commonFunc::touchesWood(collision, LevelPartList)) {//xem lai
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
	if (commonFunc::touchesWood(collision, LevelPartList, grounded, groundSTT, levelSTT)) {//chạm tường
		if (yVel > 0) {//đối tượng đang rơi
			yPos = LevelPartList.at(levelSTT).getTilesList().at(groundSTT)->getY() - 64 * 2;// tọa độ của ô cách đáy nhất mà đối tượng đang va chạm trên màn hình (lấy từ LevelPartList)
			if (falling) {//nếu trước đấy đang rơi
				grounded = true;//cho bt đối tượng đứng trên mặt đất
			}
		}
		else if (yVel < 0) {//đang nhảy lên
			yPos -= yVel;//để đt k còn bay lên
			yVel = 0;
		}
		collision.y = getY() + MONSTER_HEIGHT;
	}
	//else grounded = false;//xem lai	
}

void Monster::gravity() {
	if (!grounded) {
		yVel += GRAVITY;//tang toc do roi
		if (yVel > MAX_GRAVITY) yVel = MAX_GRAVITY;
	}
	else yVel = GRAVITY;
}

//lesson 29
double distance( float x1, float y1, float x2, float y2 ) {// tính khoảng cách của 2 điểm 
    float deltaX = x2 - x1;
    float deltaY = y2 - y1;
    return sqrt(deltaX*deltaX + deltaY*deltaY);
}

void Monster::moveToPlayer(Player& p_player, vector<LevelPart>& LevelPartList) {
	distanceToPlayer = distance(getX(),getY(),p_player.getX(),p_player.getY());
	if (!beingHit) {
		//trong tầm nhìn của Monster
		if ((p_player.getY() >= getY() - TILE_WIDTH && p_player.getY() <= getY() + TILE_WIDTH * 0.5) && distanceToPlayer <= TILE_WIDTH * 7) {
			if (p_player.getX() - getX() < 0) {
				if (LevelPartList.at(levelSTT).getTilesList().at(groundSTT - 1)->getType() > 84) 
					xVel = 0;
				else 
					xVel = -MONSTER_VEL;
			}
			else if (LevelPartList.at(levelSTT).getTilesList().at(groundSTT + 1)->getType() > 84) 
				xVel = 0;
			else 
				xVel = MONSTER_VEL;
		}
	}
	//Player nam trong vung tan cong cua monster
	if ( (distanceToPlayer <= TILE_WIDTH * 1.5 || (p_player.getY() >= getY() - TILE_WIDTH * 2.5 && p_player.getY() <= getY() - 64 && distanceToPlayer <= TILE_WIDTH * 2.5)) && !dead && !beingHit && grounded) 
        attacking = true;
	else 
		attacking = false;
}

void Monster::autoMovement(vector<LevelPart>& LevelPartList) {
	if (grounded && !beingHit) {
		//ô vuông bên phải và bên trái đều có loại ô vuông lớn hơn 84 (nghĩa là không phải mặt đất) thì Skeleton sẽ dừng lại
		if (LevelPartList.at(levelSTT).getTilesList().at(groundSTT + 1)->getType() > 84 && LevelPartList.at(levelSTT).getTilesList().at(groundSTT - 2)->getType() > 84) xVel = 0;		
		else if (LevelPartList.at(levelSTT).getTilesList().at(groundSTT + 1)->getType() > 84 && xVel > 0) xVel = -MONSTER_VEL * 0.5;
		else if (LevelPartList.at(levelSTT).getTilesList().at(groundSTT - 1)->getType() > 84 && xVel < 0) xVel = MONSTER_VEL * 0.5;
		else if (LevelPartList.at(levelSTT).getTilesList().at(groundSTT + 2)->getType() > 84 && LevelPartList.at(levelSTT).getTilesList().at(groundSTT - 2)->getType() > 84) xVel = 0;
		else if(getFlipType() == SDL_FLIP_NONE) xVel = MONSTER_VEL * 0.5;
		else if(getFlipType() == SDL_FLIP_HORIZONTAL) xVel = -MONSTER_VEL * 0.5;
	}
}

bool Monster::isAttacking() {
	if (attackingFrame / 7 >= 2) return true;
	return false;
}

//monster bi danh, ha guc
void Monster::getHit(Player& p_player, Mix_Chunk* p_sfx[], SDL_Rect& camera) {
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

	if (beingHit && beingHitFrame == 0) Mix_PlayChannel(-1, p_sfx[1], 0);

	if (beingHitFrame / 7 >= BEINGHIT_ANIMATION_FRAMES) {
		beingHit = false;
		beingHitFrame = 0;
	}
	
	//con quái vật di chuyển quá xa về phía trái, nó sẽ được coi là đã rời khỏi vùng chơi và bị xóa khỏi trò chơi.	
	if (maxHealth <= 0 || getY() + MONSTER_HEIGHT/2 > LEVEL_HEIGHT || getX() - camera.x < 192 - 2*64) {
		dead = true;
		beingHit = false;
		Mix_PlayChannel(-1, p_sfx[0], 0);
	}
}

void Monster::knockBack() {
	if (beingHit && beingHitFrame==0) {
		yVel = -3;
		if (getFlipType() == SDL_FLIP_NONE) xVel = -4;
		else if(getFlipType() == SDL_FLIP_HORIZONTAL ) xVel = 4;
	}
}

void Monster::render(SDL_Rect& p_camera) {
	if (walking) {
		commonFunc::renderAnimation(tex, xPos, yPos, walkingClips[walkFrame / 4], p_camera, 0, NULL, getFlipType());
		walkFrame++;
		if (walkFrame / 4 >= WALKING_ANIMATION_FRAMES) walkFrame = 0;
	}
	else walkFrame = 0;

	if (idling) {
		commonFunc::renderAnimation(tex, xPos, yPos, idlingClips[idleFrame / 6], p_camera, 0, NULL, getFlipType());
		idleFrame++;
		if (idleFrame / 6 >= IDLING_ANIMATION_FRAMES) idleFrame = 0;
	}
	else idleFrame = 0;

	if (falling) {
		commonFunc::renderAnimation(tex, xPos, yPos, fallingClips[fallingFrame / 4], p_camera, 0, NULL, getFlipType());
		fallingFrame++;
		if (fallingFrame / 4 >= FALLING_ANIMATION_FRAMES) fallingFrame = 0;
	}
	else fallingFrame = 0;

    if (attacking) {
		commonFunc::renderAnimation(tex, xPos, yPos, attackingClips[attackingFrame / 7], p_camera, 0, NULL, getFlipType());
		attackingFrame++;
		if (attackingFrame / 7 >= ATTACKING_ANIMATION_FRAMES) attackingFrame = 0;
	}
	else attackingFrame = 0;

	if (beingHit) {
		commonFunc::renderAnimation(tex, xPos, yPos, beingHitClips[beingHitFrame / 7], p_camera, 0, NULL, getFlipType());
		beingHitFrame++;
	}
	else beingHitFrame = 0;
}