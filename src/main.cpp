#include <iostream>

#include "RenderWindow.h"
#include "Entity.h"
#include "Player.h"
#include "Monster.h"
#include "Timer.h"
#include "Tile.h"
#include "Game.h"

using namespace std;

SDL_Event event;

Game gameMain;


int main(int argc, char* argv[]) {
    
    srand(time(NULL));

    if (!gameMain.init()) return 0;
    else {
        if (!gameMain.loadMedia()) cout << "FAILED TO LOAD MEDIA!" << endl;
        else {
            if (!gameMain.createPlayer() || !gameMain.createMonster()) {
                cout << "FAILED TO LOAD TILE SET!" << endl;
                return 0;
            }
            gameMain.start_BredatorySFX();
            
            while (gameMain.isRunning()) {
                SDL_Event event;
                while (SDL_PollEvent(&event)) gameMain.handleGameInput(event);
                
                gameMain.playMusic();
                gameMain.render_update_Game();
           }
        }
    }
    gameMain.destroy();
    commonFunc::cleanUp();
    return 0;
}
