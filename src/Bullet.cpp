#include "Bullet.h"

//ạo một đối tượng đạn mới với vị trí ban đầu p_x, p_y và hình ảnh được sử dụng để vẽ đạn là p_tex
Bullet::Bullet(float p_x, float p_y, SDL_Texture* p_tex) : Entity(p_x, p_y, p_tex) {
	collision.x = xPos;
	collision.y = yPos;

	//lzayfoo_11
	//để khởi tạo các khung hình của đạn cho việc phát hiện và xử lý va chạm sau này.
	//chia nhỏ hình ảnh đạn thành các khung hình nhỏ hơn để dễ dàng xử lý trong việc vẽ đạn khi nó di chuyển.
	for (int i = 0; i < BULLET_ANIMATION_FRAMES; i++) {
		bulletClips[i].x = i * DEFAULTBULLET_W;
		bulletClips[i].y = 0;
		bulletClips[i].w = DEFAULTBULLET_W;
		bulletClips[i].h = DEFAULTBULLET_H;
	}
}

void Bullet::update(Tile* tile[]) {
	if (bulletCounter/delay < 3) {
		xVel += BULLETSPEED;
		if (getFlipType() == SDL_FLIP_HORIZONTAL) xPos -= xVel;
		else if (getFlipType() == SDL_FLIP_NONE) xPos += xVel;
		collision.x = getX();
		if (getX()< 0) {
			xPos = 0;
			collision.x = getX();
		}
		if (getX() + DEFAULTBULLET_W > LEVEL_WIDTH) {
			xPos = LEVEL_WIDTH - DEFAULTBULLET_W;
			collision.x = getX();
		}
		if (commonFunc::touchesWood(collision, tile)) {
			if (getFlipType() == SDL_FLIP_HORIZONTAL) xPos += xVel;
			else if (getFlipType() == SDL_FLIP_NONE) xPos -= xVel;
			collision.x = getX();
		}
	}
	if (bulletCounter/delay == BULLET_ANIMATION_FRAMES) setMove(false);
}

void Bullet::move() {
	if (flipType == SDL_FLIP_NONE) xPos += BULLETSPEED;
	else xPos -= BULLETSPEED;
	
	collision.x = xPos;
}

void Bullet::setFlipType(SDL_RendererFlip p_PlayerflipType) {
	flipType = p_PlayerflipType;
}

//cập nhật kích thước và vị trí của đối tượng đạn 
void Bullet::setWidthHeight(const int& p_width, const int& p_height, const int& p_playerX) {
	// kiểm tra hướng quay của đối tượng người chơi (Player) để xác định xem đạn sẽ được vẽ ở vị trí nào trên trục X.
	if (getFlipType() == SDL_FLIP_HORIZONTAL) {
		xPos = p_playerX - (DEFAULTBULLET_W / 5);
	}

	//Tiếp theo, hàm cập nhật chiều rộng và chiều cao của hình chữ nhật va chạm (collision) của đạn. 
	collision.w = DEFAULTBULLET_W;
	collision.h = DEFAULTBULLET_H-1;//để đảm bảo rằng đạn sẽ không bị va chạm với các đối tượng mà nó không cần chạm vào
}

void Bullet::render(SDL_Rect &camera, SDL_Texture* p_tex) {
	commonFunc::renderAnimation(p_tex, getX(), getY(), bulletClips[bulletCounter/5], camera, 0, NULL, getFlipType());
	bulletCounter++;
}