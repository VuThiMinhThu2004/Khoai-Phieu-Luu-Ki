#include "Heart.h"

using namespace std;

Heart::Heart(float p_x, float p_y, SDL_Texture* p_tex) : Entity(p_x, p_y, p_tex) {
    //Initialize the offsets
    xPos = 0;
    yPos = 0;
    //Initialize the velocity
    xVel = 0;
    yVel = 0;
}

void Heart::update(Player& p_player, vector<LevelPart>& LevelPartList, Mix_Chunk* p_sfx[], SDL_Rect& camera){
    if (!beingHit) {
		flipType = SDL_FLIP_HORIZONTAL; //if (xVel < 0)
    }

    autoMovement(LevelPartList);

    if (getY() + HEART_HEIGHT/2 > LEVEL_HEIGHT || getX() - camera.x < 192 - 2*64) {
		dead = true;
		beingHit = false;
		Mix_PlayChannel(-1, p_sfx[0], 0);
	}
}

void Heart::autoMovement(vector<LevelPart>& LevelPartList) {
    if (xVel < 0)   xVel = -HEART_VEL * 0.5;
}

void Heart::render(SDL_Rect& p_camera) {
	commonFunc::renderTexture(heartTex, 0, 0, 1280, 720);
}