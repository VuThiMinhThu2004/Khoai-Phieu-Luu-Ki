#include "Player.h"

//lesson 14
Player::Player(int p_x, int p_y, SDL_Texture* p_tex) : Entity(p_x, p_y, p_tex) {
	
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

//lazyfoo 26
void Player::handleInputActive(SDL_Event &events, Mix_Chunk* p_sfx[]) {
	if (!isDead()) {
		//nhấn phím và không lặp lại, ta sẽ kiểm tra mã phím được nhấn và xử lý tương ứng với từng trường hợp
		if (events.type == SDL_KEYDOWN && events.key.repeat == 0) {//&& events.key.repeat == 0
			switch (events.key.keysym.sym) {
			case SDLK_a:
				xVel -= PLAYER_VEL;//xvel la van toc dua vao day quyet dinh flipType
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
		//nhả phím và không lặp lại, ta cũng kiểm tra mã phím và xử lý tương ứng với từng trường hợp.
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
		//bấm chuột và không lặp lại, ta kiểm tra xem nút chuột được bấm có phải là nút trái hay không
		else if (events.type == SDL_MOUSEBUTTONDOWN && events.key.repeat == 0) {//lesson 17
			if (events.button.button == SDL_BUTTON_LEFT) {
				//là nút trái, ta tạo một đối tượng đạn mới, đặt vị trí xuất phát của đạn ở phía trên bên phải của nhân vật
				// và thêm đạn vào danh sách đạn. Ta cũng phát âm thanh tương ứng.

				//tạo một đối tượng đạn mới bằng cách khởi tạo một con trỏ đối tượng đạn.
				// tọa độ x của đạn, được tính bằng cách lấy tọa độ x của lớp người chơi (getCollision().x) cộng với chiều rộng của người chơi (PLAYER_WIDTH) nhân với một hằng số (1.25).
				//Tham số thứ hai là tọa độ y của đạn, được lấy từ tọa độ y của người chơi (getCollision().y).
				//Tham số thứ ba là một hình ảnh đạn được truyền vào dưới dạng một con trỏ hình ảnh. Trong trường hợp này, giá trị được truyền là NULL
				Bullet* bullet = new Bullet(getCollision().x + PLAYER_WIDTH * 1.25, getCollision().y, NULL);
				
				//âm thanh bắn súng.
				/*
				Tham số đầu tiên là một số nguyên đại diện cho kênh âm thanh sẽ được sử dụng (-1 là kênh mặc định).
				Tham số thứ hai là con trỏ tới âm thanh bắn súng.
				Tham số thứ ba là mức độ lặp lại của âm thanh.
				*/
				Mix_PlayChannel(-1, p_sfx[shootSFX], 0);
				// đặt kiểu lật của đạn bằng kiểu lật của lớp người chơi (getFlipType()).
				bullet->setFlipType(getFlipType());
				//câu lệnh đặt kích thước và vị trí cho đạn.
				bullet->setWidthHeight(DEFAULTBULLET_W, DEFAULTBULLET_H, getX());
				bullet->setType(Bullet::NORMAL);//đặt loại đạn
				bullet->setMove(true);// đặt thuộc tính "move" của đối tượng viên đạn là "true", cho phép viên đạn di chuyển trên màn hình.
				bulletList.push_back(bullet);//xử lý việc cập nhật vị trí của các viên đạn và xóa chúng khi chúng chạm vào các vật cản hoặc ra khỏi màn hình.
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

	//cập nhật hướng quay của nhân vật (flipType) nếu nhân vật đang di chuyển theo trục x.
	if (!beingHit) {
		if (xVel < 0) flipType = SDL_FLIP_HORIZONTAL;
		if (xVel > 0) flipType = SDL_FLIP_NONE;
	}

	//lesson 26
	//hàm này di chuyển nhân vật theo trục x và y nếu nhân vật chưa chết.
	//Nó kiểm tra xem nhân vật có chạm tường hay không và điều chỉnh vị trí của nhân vật nếu cần.
	//move x
	if (!dead) {
		xPos += 1.1*xVel;//xem lai
		collision.x = getX() + PLAYER_WIDTH;


		//nhân vật đã đi quá màn hình sang trái, nó sẽ được đặt lại về vị trí ban đầu
		// và giới hạn di chuyển của nhân vật sẽ được giữ nguyên.
		if (getX() + PLAYER_WIDTH < 0) {
			xPos = -PLAYER_WIDTH;
			collision.x = getX() + PLAYER_WIDTH;
		}

		//kiểm tra nv có va chạm với các tường trong danh sách LevelPartList
		if (commonFunc::touchesWood(collision, LevelPartList)) {
			xPos -= 1.1*xVel;//xem lai
			collision.x = getX() + PLAYER_WIDTH;
			//va chạm, vị trí của nhân vật sẽ được điều chỉnh để ngăn không cho nhân vật đi xuyên qua các tường.
			//va chạm với các tường, biến collision.x được cập nhật với vị trí mới của nhân vật theo trục x để kiểm tra va chạm với tường trong lần cập nhật kế tiếp
		}
	}

	//move y
	yPos += yVel;//xem lai
	collision.y = getY() + PLAYER_HEIGHT;
	//kiểm tra va chạm với cạnh trên của màn hình
	if (getY() + PLAYER_HEIGHT < 0) {
		yPos = -PLAYER_HEIGHT*0.5;//xem lai
		collision.y = getY() + PLAYER_HEIGHT;
	}

	//kiểm tra va chạm với một bức tường hoặc đất
	if (commonFunc::touchesWood(collision, LevelPartList, grounded, groundSTT, levelSTT)) {
		
		if (yVel > 0) {
			//đặt lại vị trí y của Player để nằm trên bức tường hoặc đất
			yPos = LevelPartList.at(levelSTT).getTilesList().at(groundSTT)->getY() - 64 * 2;
			//nếu đang rơi
			if (falling) {
				grounded = true;
				Mix_PlayChannel(-1, p_sfx[landSFX], 0);
			}
		}
		//nếu đang nhảy lên
		else if (yVel < 0) {
			//đặt lại vị trí y của Player để nằm trên bức tường hoặc đất
			yPos -= yVel;//xem lai
			yVel = 0;//dừng việc nhảy
		}
		collision.y = getY() + PLAYER_HEIGHT;
	}
}


void Player::jump() {
	if (grounded) {//đang đứng trên mặt đất
		yVel -= 11;// tốc độ theo trục y của nhân vật: nếu yval < 0 thì làm cho nhân vật nhảy cao hơn: những lần nhảy liên tiếp
		grounded = false;//nv k ở trên mặt đất = đang nhảy
	}
}

void Player::gravity() {
	if (!grounded) {// đang k đứng trên mặt đất
		yVel += GRAVITY;// để biểu thị lực hút lên nhân vật
		if (yVel > MAX_GRAVITY) yVel = MAX_GRAVITY;//vượt quá tốc độ rơi max 
	}
	else yVel = GRAVITY;//nv ở trên mặt đất -> để đảm bảo nv k bay lên khi đang đứng yên
}


//lesson 30:copy lazyfoo
void Player::handleCamera(SDL_Rect& camera, float& camVel) {
	
	if(!isDead()) camera.x += camVel;
	
	float acc = 0.001;
	// nếu camVel đã vượt quá một mức độ nhất định, tốc độ tăng sẽ giảm dần (acc giảm dần).	
	if (camVel > 4) acc = 0.0003;
	if (camVel > 5) acc = 0.00001;

	camVel += acc;
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
	if (getY() + PLAYER_HEIGHT >= LEVEL_HEIGHT || getX() - camera.x <= 192 - 2*64) {
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
		if (getFlipType() == SDL_FLIP_NONE) xPos -= 100;
		else if (getFlipType() == SDL_FLIP_HORIZONTAL) xPos += 10;
	}
}

//lazyfoo_14: hiển thị hình ảnh của nhân vật trong game
void Player::render(SDL_Rect &p_camera) {
	if (running) {
	//sử dụng các sprite từ mảng walkingClips để tạo hiệu ứng chạy. 
		commonFunc::renderAnimation(tex, xPos, yPos, walkingClips[walkFrame / 4], p_camera, 0, NULL, getFlipType());//hiển thị hình ảnh của nhân vật tương ứng với trạng thái đó.
		walkFrame++;////Go to next frame

		////Cycle animation
		//Mỗi frame của sprite sẽ được hiển thị trong 4 lần lặp của vòng lặp để tạo hiệu ứng chuyển động.
		if (walkFrame / 4 >= WALKING_ANIMATION_FRAMES) walkFrame = 0; // đặt lại khung trở lại 0 để hình ảnh động bắt đầu lại.
	}

	if (idling) {
		commonFunc::renderAnimation(tex, xPos, yPos, idlingClips[idleFrame/6], p_camera, 0, NULL, getFlipType());
		idleFrame++;
		//Mỗi frame của sprite sẽ được hiển thị trong 6 lần lặp của vòng lặp để tạo hiệu ứng chuyển động.
		if (idleFrame / 6 >= IDLING_ANIMATION_FRAMES) idleFrame = 0;
	}
	else idleFrame = 0;
	/*
	người chơi không đứng yên, mà sau đó chuyển sang trạng thái khác (ví dụ: chạy hoặc nhảy), 
	thì hoạt hình mới sẽ bắt đầu từ frame đầu tiên của trạng thái đó thay vì frame hiện tại của trạng thái đứng yên.
	 Nếu không reset biến idleFrame về 0, các frame cũ của trạng thái đứng yên sẽ tiếp tục được hiển thị khi chuyển sang trạng thái khác.
	*/

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
