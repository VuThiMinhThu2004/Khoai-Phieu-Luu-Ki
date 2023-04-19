#include "Game.h"

bool Game::init() {
    bool success = true;
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        cout << "SDL_Init HAS FAILED. SDL_ERROR: " << SDL_GetError() << endl;
        success = false;
    }
    if (!(IMG_Init(IMG_INIT_PNG))) {
        cout << "IMG_Init HAS FAILED. SDL_ERROR: " << IMG_GetError() << endl;
        success = false;
    }
    if (!(IMG_Init(IMG_INIT_JPG))) {
        cout << "IMG_Init HAS FAILED. SDL_ERROR: " << IMG_GetError << endl;
        success = false;
    }
    if (TTF_Init() == -1) {
        cout << "TTF_Init HAS FAILED. SDL_ERROR: " << TTF_GetError() << endl;
        success = false;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << endl;
        success = false;
    }

    if (success == true) commonFunc::renderWindow("Escape the Dungeon", SCREEN_WIDTH, SCREEN_HEIGHT);

    return success;
}

bool Game::loadMedia() {
    bool success = true;
    if (!commonFunc::loadFont("res/lazy.ttf")) success = false;

    PotatoTex = commonFunc::loadTexture("res/images/Potato.png");
    if (PotatoTex == NULL) success = false;

    TileTex = commonFunc::loadTexture("res/images/TileMap.png");
    if (TileTex == NULL) success = false;

    BulletTex = commonFunc::loadTexture("res/images/Bullet.png");
    if (BulletTex == NULL) success = false;

    MonsterTex = commonFunc::loadTexture("res/images/Monster.png");
    if (MonsterTex == NULL) success = false;

    BredatoryTex = commonFunc::loadTexture("res/images/Bredatory.png");
    if (BredatoryTex == NULL) success = false;

    BackgroundTex = commonFunc::loadTexture("res/images/Doge.jpg");
    if (BackgroundTex == NULL) success = false;

    ButtonTex = commonFunc::loadTexture("res/images/Button.png");
    if (ButtonTex == NULL) success = false;


    if (!success) cout << "FAILED TO LOAD MEDIA: " << SDL_GetError() << endl;
    return success;
}

void Game::FPSCounter() {
    float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
    if (avgFPS > 2000000) {
        avgFPS = 0;
    }
    timeText.str("");
    timeText << "FPS: " << avgFPS;

    SDL_Color textColor = { 0,0,0,255 };
    SDL_Texture* textTex = commonFunc::createText(timeText.str().c_str(), textColor);
    Entity text(0, 0, textTex);
    commonFunc::renderTexture(text);
    ++countedFrames;
}



bool Game::createPlayer() {
    Player Potato(64 * 3, LEVEL_HEIGHT - TILE_HEIGHT * 5, PotatoTex);
    playerList.push_back(Potato);
    if (playerList.size() < 0) return false;
    return true;
}


void Game::render_update_bullet() {
    //render & update bullet
    vector<Bullet*> bulletList = playerList.at(0).getBulletList();
    for (int i = 0; i < playerList.at(0).getBulletList().size(); i++) {
        if (bulletList.at(i) != NULL) {
            if (bulletList.at(i)->isMoving()) {
                bulletList.at(i)->render(camera, BulletTex);
                bulletList.at(i)->update();
            }
            else {
                delete bulletList.at(i);
                bulletList.at(i) = NULL;
                bulletList.erase(bulletList.begin() + i);
                playerList.at(0).setBulletList(bulletList);
            }
        }
    }
}




void Game::render_update_Bredatory() {
    //render explosion
    commonFunc::renderAnimation(BredatoryTex, camera.x - 64, 0, Bredatory[exCounter / 4], camera);
    exCounter++;
    if (exCounter / 4 >= 8) exCounter = 24;
}

void Game::render_update_Game() {
    commonFunc::clearRenderer();
    render_update_LevelPart();
    render_update_bullet();
    render_update_player();
    render_update_skeleton();
    render_update_Bredatory();
    commonFunc::renderPresent();
}


void Game::setSDL_Rect() {
    int m = 0, n = 0;
    for (int i = 0; i < TOTAL_TILE_SPRITES; i++) {
        gTileClips[i].x = n;
        gTileClips[i].y = m;
        gTileClips[i].w = TILE_WIDTH;
        gTileClips[i].h = TILE_HEIGHT;
        n += TILE_WIDTH;
        if (n >= 17 * TILE_WIDTH) {
            n = 0;
            m += TILE_HEIGHT;
        }
    }

    for (int i = 0; i < 15; i++) {
        Bredatory[i].x = i * 256;
        Bredatory[i].y = 0;
        Bredatory[i].w = 256;
        Bredatory[i].h = 1280;
    }
}