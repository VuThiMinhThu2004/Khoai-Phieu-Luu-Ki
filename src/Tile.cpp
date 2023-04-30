#include "Tile.h"

//khởi tạo đối tượng tile có vị trí px,p7 và texture ptex và loại tile ptype
Tile::Tile(int p_x, int p_y, SDL_Texture* p_tex, int p_type) : Entity(p_x, p_y, p_tex) {
	//cập nhật collision cho tile
	collision.x = p_x;
	collision.y = p_y;
	collision.w = TILE_WIDTH;
	collision.h = TILE_HEIGHT;

	tileType = p_type;
}

int Tile::getType() {
	return tileType;
}

SDL_Rect Tile::getCollision() {
	return collision;
}

//cập nhật vị trí x của Tile và cập nhật lại giá trị của collision
void Tile::setX(int p_x) {
	 xPos = p_x;
	 collision.x = getX();
}