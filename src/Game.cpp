#include "Game.h"
using namespace std;


bool Game::init() {//lazyfoo
    //Initialization flag
    bool success = true;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }

    if (!(IMG_Init(IMG_INIT_PNG))) {
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
        success = false;
    }

    if (!(IMG_Init(IMG_INIT_JPG))) {
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
        success = false;
    }

    if (TTF_Init() == -1) {
        printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    if (success == true) commonFunc::renderWindow("KHOAI PHIEU LUU KI", SCREEN_WIDTH, SCREEN_HEIGHT);

    return success;
}

bool Game::loadMedia() {//lazyfoo 
    bool success = true;


    if (!commonFunc::loadFont("res/fonts/segoeprb.ttf")) success = false;

    //Load sprite sheet texture
    potatoTex = commonFunc::loadTexture("res/images/Potato.png");
    if (potatoTex == NULL) success = false;

    tileTex = commonFunc::loadTexture("res/images/TileMap.png");
    if (tileTex == NULL) success = false;

    bulletTex = commonFunc::loadTexture("res/images/Bullet.png");
    if (bulletTex == NULL) success = false;

    monsterTex = commonFunc::loadTexture("res/images/Monster.png");
    if (monsterTex == NULL) success = false;

    bredatoryTex = commonFunc::loadTexture("res/images/Bredatory.png");
    if (bredatoryTex == NULL) success = false;

    backgroundTex = commonFunc::loadTexture("res/images/Background.jpg");
    if (backgroundTex == NULL) success = false;

    buttonTex = commonFunc::loadTexture("res/images/Button.png");
    if (buttonTex == NULL) success = false;

    circleOfLightTex = commonFunc::loadTexture("res/images/Circle of light.png");
    if (circleOfLightTex == NULL) success = false;

    dieTex = commonFunc::loadTexture("res/images/die.png");
    if (dieTex == NULL) success = false;

    namegameTex = commonFunc::loadTexture("res/images/name.png");
    if (namegameTex == NULL) success = false;

    backgroundMusic = Mix_LoadMUS("res/sfx/bg.mp3");
    if (backgroundMusic == NULL) {
        printf( "Failed to load bg Music! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    bredatorySFX = Mix_LoadWAV("res/sfx/bredatory.wav");
    if (bredatorySFX == NULL) {
        printf( "Failed to load explosion sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    playerSFX[0] = Mix_LoadWAV("res/sfx/playeHit.wav"); 
    if (playerSFX[0] == NULL) {
        printf( "Failed to load player sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    playerSFX[1] = Mix_LoadWAV("res/sfx/playerJump.wav");
    if (playerSFX[1] == NULL) {
        printf( "Failed to load player sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    playerSFX[2] = Mix_LoadWAV("res/sfx/playerLanding.wav"); 
    if (playerSFX[2] == NULL) {
        printf( "Failed to load player sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    playerSFX[3] = Mix_LoadWAV("res/sfx/playerLaser.wav");
    if (playerSFX[3] == NULL) {
        printf( "Failed to load player sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    monsterSFX[0] = Mix_LoadWAV("res/sfx/shooting.wav"); 
    if (monsterSFX[0] == NULL) {
        printf( "Failed to load monster sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    monsterSFX[1] = Mix_LoadWAV("res/sfx/damageHit.wav"); 
    if (monsterSFX[1] == NULL) {
        printf( "Failed to load monster sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    if (!success) cout << "FAILED TO LOAD MEDIA: " << SDL_GetError() << endl;
    
    return success;
}

//copy lesson 24: Calculate and correct fps
void Game::FPSCounter() {
    int avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
    if (avgFPS > 2000000) {
        avgFPS = 0;
    }
    //Set text to be rendered
    timeText.str("");
    timeText << "FPS: " << avgFPS;

    SDL_Color yellowColor = { 252,226,5,255 };
    SDL_Texture* textTex = commonFunc::createText(timeText.str().c_str(), yellowColor);
    Entity text(64*18, 0, textTex);
    commonFunc::renderTexture(text);
    ++countedFrames;
}

void Game::HpCounter() {
    hp = playerList.at(0).maxHP;
    hpText.str("");
    hpText << "HP: " << hp;

    SDL_Color redColor = { 255,0,10,0 };
    SDL_Texture* textTex = commonFunc::createText(hpText.str().c_str(), redColor);
    Entity text(64*10, 0, textTex);
    commonFunc::renderTexture(text);
}

void Game::renderScore() {
    if (score < playerList.at(0).getX() / TILE_WIDTH) score = playerList.at(0).getX() / TILE_WIDTH;
    scoreText.str("");
    scoreText << "Score: " << score << "m";
    SDL_Color yellowColor = { 252,226,5,255 };


    ifstream readFile;
    readFile.open("res/highscore.txt");
    if (readFile.fail()) update_hight_score();
    readFile >> highscore;
    highscoreText.str("");
    highscoreText << "High Score: " << highscore << "m";


    SDL_Texture* scoreTex = commonFunc::createText(scoreText.str().c_str(), yellowColor);
    SDL_Texture* highscoreTex = commonFunc::createText(highscoreText.str().c_str(), yellowColor);

    Entity current_score(64*3, 30, scoreTex);
    Entity high_score(64*3, 0, highscoreTex);

    commonFunc::renderTexture(current_score);
    commonFunc::renderTexture(high_score);
}

bool Game::createMap() {
    path_pos Map1({ 6,6,9,6 }, "res/map/map1.map");
    mapList.push_back(Map1);
    path_pos Map2({5,5,10,3,15,5 }, "res/map/map2.map");
    mapList.push_back(Map2);
    path_pos Map3({ 5,5,15,6,19,7 }, "res/map/map3.map");
    mapList.push_back(Map3);
    path_pos Map4({ 6,6,7,9,15,4}, "res/map/map4.map");
    mapList.push_back(Map4);
    path_pos MapSpawn({ }, "res/map/map_spawn.map");
    mapList.push_back(MapSpawn);
    if (mapList.size() < TOTAL_MAP) {
        cout << "FAILED TO LOAD MAP!" << endl;
        return false;
    }
    return true;
}

bool Game::createLevel() {

    for (int i = 0; i < TOTAL_LEVEL_PART; i++) {
        int random = rand() % (TOTAL_MAP - 1);
        if (i == 0) random = TOTAL_MAP - 1;

        LevelPart level(i * LEVEL_WIDTH, 0, mapList.at(random).path, tileTex);

       level.setMonsterPos(mapList.at(random).monsterPos);
       LevelPartList.push_back(level);

    }
    if (LevelPartList.size() < TOTAL_LEVEL_PART) {
        cout << "FAILED TO CREATE LEVEL!" << endl;
        return false;
    }
    return true;
}

bool Game::createMonster() {

    for (int i = 0; i < LevelPartList.size(); i++) {
        if (LevelPartList.at(i).getMonsterPos().size() > 0) {
            for (int j = 0; j < LevelPartList.at(i).getMonsterPos().size() - 1; j += 2) {

                Monster* monster = new Monster(LevelPartList.at(i).getMonsterPos().at(j) * TILE_WIDTH + LevelPartList.at(i).getX(), LevelPartList.at(i).getMonsterPos().at(j + 1) * TILE_WIDTH + LevelPartList.at(i).getY(), monsterTex);
                monsterList.push_back(monster);
            }
        }
    }
    return true;
}

bool Game::createPlayer() {
    Player potato(64 * 3, LEVEL_HEIGHT - TILE_HEIGHT * 5, potatoTex);
    playerList.push_back(potato);
    if (playerList.size() < 0) return false;
    return true;
}

bool Game::createMenu() {
    Menu gameMenu(buttonTex, backgroundTex, backgroundTex);
    menuList.push_back(gameMenu);

    Mix_FadeInMusic(backgroundMusic, -1, 1000);
    Mix_VolumeMusic(600);

    if (menuList.size() < 0) return false;
    return true;
}

void Game::start_BredatorySFX() {
    Mix_PlayChannel(-1, bredatorySFX, -1);
    Mix_VolumeChunk(bredatorySFX, 0);
}

void Game::playMusic() {
    Mix_VolumeChunk(bredatorySFX, 80);
    if (Mix_PlayingMusic() == 0) {
        Mix_FadeInMusic(backgroundMusic, -1, 1000);
        Mix_VolumeMusic(600);
    }
    else if (Mix_PausedMusic() == 1) Mix_ResumeMusic();
    else if (playerList.at(0).isDead()) Mix_HaltMusic();
}

void Game::pauseMusicTime() {
    Mix_VolumeChunk(bredatorySFX, 0);
    if (Mix_PlayingMusic() == 1) {
        Mix_PauseMusic();
    }
    fpsTimer.pause();
}

void Game::render_update_LevelPart() {
    //update LevelPart
    if (LevelPartList.at(0).getX() - camera.x <= -LEVEL_WIDTH) {
        int random = rand() % (TOTAL_MAP - 1);
        LevelPartList.at(0).setTilesType(mapList.at(random).path);
        LevelPartList.at(0).setLevelX(LevelPartList.at(LevelPartList.size() - 1));
        LevelPartList.at(0).setMonsterPos(mapList.at(random).monsterPos);

        if (LevelPartList.at(0).getMonsterPos().size() > 0) {
            for (int j = 0; j < LevelPartList.at(0).getMonsterPos().size() - 1; j += 2) {
                Monster* monster = new Monster(LevelPartList.at(0).getMonsterPos().at(j) * TILE_WIDTH + LevelPartList.at(0).getX(), LevelPartList.at(0).getMonsterPos().at(j + 1) * TILE_WIDTH + LevelPartList.at(0).getY(), monsterTex);
                monsterList.push_back(monster);
            }
        }
        LevelPart level = LevelPartList.at(0);
        LevelPartList.erase(LevelPartList.begin());
        LevelPartList.push_back(level);
    }

    //render levelPart
    for (int i = 0; i < LevelPartList.size(); i++) {
        LevelPartList.at(i).render(gTileClips, camera);
    }
}

void Game::render_update_bullet() {
    vector<Bullet*> bulletList = playerList.at(0).getBulletList();
    for (int i = 0; i < playerList.at(0).getBulletList().size(); i++) {
        if (bulletList.at(i) != NULL) {
            if (bulletList.at(i)->isMoving()) {
                bulletList.at(i)->render(camera, bulletTex);
                bulletList.at(i)->update(LevelPartList);
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

void Game::render_update_player() {
    playerList.at(0).update(LevelPartList, monsterList, playerSFX, camera);
    playerList.at(0).handleCamera(camera, camVel);
    playerList.at(0).render(camera);
    HpCounter();

}

void Game::render_update_monster() {
    for (int i = 0; i < monsterList.size(); i++) {
        if (monsterList.at(i) != NULL) {
            if (!monsterList.at(i)->isDead()) {
                monsterList.at(i)->render(camera);
                monsterList.at(i)->update(playerList.at(0), LevelPartList, monsterSFX, camera);
            }
            else {
                delete monsterList.at(i);
                monsterList.at(i) = NULL;
                monsterList.erase(monsterList.begin() + i);
            }
        }
    }
}

void Game::render_update_bredatory() {
    commonFunc::renderAnimation(bredatoryTex, camera.x - 64, 0, bredatory[exCounter / 4], camera);
    exCounter++;
    if (exCounter / 4 >= 8) exCounter = 12;
}

void Game::update_hight_score() {
    ofstream outFile;
    outFile.open("res/highscore.txt");
    outFile.clear();
    outFile << highscore;
    outFile.close();
}

void Game::render_update_Game() {
    //start timer
    fpsTimer.start();//ls24
    commonFunc::clearRenderer();// xóa bộ trình chiếu để chuẩn bị cho khung hình tiếp theo.
    render_update_LevelPart();
    render_update_bullet();
    render_update_player();
    render_update_monster();
    render_update_bredatory();
    //render_update_heart();

    FPSCounter();
   // HeartCounter();
    renderScore();
    
    if (playerList.at(0).isDead()) {
        commonFunc::renderTexture(dieTex, SCREEN_WIDTH/2 - 540/2, SCREEN_HEIGHT/6, 540, 143);

        menuList.at(0).renderRetryMenu();
        if (score > highscore) highscore = score;
        update_hight_score();
    }
    fpsTimer.unpause();

    if (menuList.at(0).need_reseting()) resetGame();
    commonFunc::renderPresent();
}

void Game::render_mainMenu() {
    commonFunc::clearRenderer();
    getMenuList().at(0).renderMainMenu();

    commonFunc::renderTexture(namegameTex, SCREEN_WIDTH/2 - 982/2, SCREEN_HEIGHT/6, 982, 133);

    commonFunc::renderPresent();
}

void Game::resetGame() {
    playerList.at(0).resetPlayer();
    camera.x = 0;
    camera.y = 0;
    camVel = 1.5;

    //xóa monster
    if(!monsterList.empty())
        for (int i = monsterList.size() - 1; i >= 0; i--) {
            delete monsterList.at(i);
            monsterList.at(i) = NULL;
            monsterList.erase(monsterList.begin() + i);
        }
    //di chuyển level về (0,0)
    for (int i = 0; i < LevelPartList.size(); i++) {
        int random = rand() % (TOTAL_MAP - 1);
        if (i == 0) {
            random = TOTAL_MAP - 1;
            LevelPartList.at(i).setLevelX(0);
        }
        else LevelPartList.at(i).setLevelX(LevelPartList.at(i - 1));
        LevelPartList.at(i).setTilesType(mapList.at(random).path);
        LevelPartList.at(i).setMonsterPos(mapList.at(random).monsterPos);
    }

    createMonster();
    menuList.at(0).set_reset(false);
    fpsTimer.stop();
    fpsTimer.start();
    countedFrames = 0;
    score = 0;
}

void Game::handleGameInput(SDL_Event& event) {
    if (event.type == SDL_QUIT) gameRunning = false;
    menuList.at(0).handleInputActive(event, gameRunning, playerList.at(0));
    if (!menuList.at(0).isMenu() && !menuList.at(0).isPaused()) playerList.at(0).handleInputActive(event, playerSFX);
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

    for (int i = 0; i < 8; i++) {
        bredatory[i].x = i * 256;
        bredatory[i].y = 0;
        bredatory[i].w = 256;
        bredatory[i].h = 1280;
    }
}

