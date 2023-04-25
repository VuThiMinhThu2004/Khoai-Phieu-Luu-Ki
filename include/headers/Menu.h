#pragma once

#include <iostream>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "RenderWindow.h"
#include "Player.h"
#include "Entity.h"
#include "Timer.h"

using namespace std;

class Menu {
private:
	const int BUTTON_WIDTH = 620;
	const int BUTTON_HEIGHT = 60;

	static const int TYPE_OF_BUTTON = 3;
	
	SDL_Rect playButtonClips[TYPE_OF_BUTTON];
	SDL_Rect exitButtonClips[TYPE_OF_BUTTON];
	SDL_Rect retryButtonClips[TYPE_OF_BUTTON];

	SDL_Point button1 = { SCREEN_WIDTH/2 - BUTTON_WIDTH/2, SCREEN_HEIGHT/2 };
	SDL_Point button2 = { SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2, SCREEN_HEIGHT/2 + BUTTON_HEIGHT + 20 };

	bool menu = true;
	bool retry = false;
	bool paused = false;
	bool reset = false;

	bool selected[4] = { false };
	bool pressed[4] = { false };

	SDL_Texture* buttonTex = NULL;
	SDL_Texture* mainMenuBGTex = NULL;
	SDL_Texture* retryBGTex = NULL;
public:
	Menu(SDL_Texture* buttonTex, SDL_Texture* mainMenuBGTex, SDL_Texture* retryBGTex);
	void handleInputActive(SDL_Event& event, bool& p_gameRunning, Player& p_player);//lesson 17	
	void renderMainMenu();
	void renderRetryMenu();

	void resetGame(Player& p_player);
	bool checkMouseHover(const int x, const int y);

	bool isMenu() const { return menu; }
	bool isPaused() const { return paused; }
	bool need_reseting() const { return reset; }
	void set_reset(bool p_reset) { reset = p_reset; }
};