#pragma once

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <math.h>

#include "RenderWindow.h"
#include "Player.h"
#include "Entity.h"
#include "LevelPart.h"
using namespace std;


//The dot that will move around on the screen
class Heart : public Entity {
    private:
        //The dimensions of the dot
        static const int HEART_WIDTH = 64;
        static const int HEART_HEIGHT = 64;

        //Maximum axis velocity of the dot
        static const int HEART_VEL = 5;

        //int mPosX, mPosY; entity
        //The velocity of the dot
        float xVel = 0, yVel = 0;
        float distanceToPlayer;
        SDL_Rect collision;

        bool beingHit = false, dead = false;

        SDL_Texture* heartTex = NULL;

        
    public:
        

        //Initializes the variables
        Heart(float p_x, float p_y, SDL_Texture* p_tex);

        //Takes key presses and adjusts the dot's velocity
        void update(Player& p_player, vector<LevelPart>& LevelPartList, Mix_Chunk* p_sfx[], SDL_Rect& camera);

        //Moves the heart
        void autoMovement(vector<LevelPart>& LevelPartList);

        //Shows the heart on the screen
        void render();
        void render(SDL_Rect& p_camera);
};
