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

//Để tải nhạc, chúng tôi gọi mix_loadmus và để tải hiệu ứng âm thanh, chúng tôi gọi là mix_loadwav.

    backgroundMusic = Mix_LoadMUS("res/sfx/background.mp3");
    if (backgroundMusic == NULL) {
        printf( "Failed to load bg Music! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    BredatorySFX = Mix_LoadWAV("res/sfx/Bredatory.wav");
    if (BredatorySFX == NULL) {
        printf( "Failed to load explosion sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    playerSFX[0] = Mix_LoadWAV("res/sfx/player_hit.wav"); 
    if (playerSFX[0] == NULL) {
        printf( "Failed to load player sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    playerSFX[1] = Mix_LoadWAV("res/sfx/player_jump.wav"); //jumping
    if (playerSFX[1] == NULL) {
        printf( "Failed to load player sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    playerSFX[2] = Mix_LoadWAV("res/sfx/player(landing).wav"); //landing
    if (playerSFX[2] == NULL) {
        printf( "Failed to load player sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    playerSFX[3] = Mix_LoadWAV("res/sfx/bullet.wav"); //shooting
    if (playerSFX[3] == NULL) {
        printf( "Failed to load player sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    monsterSFX[0] = Mix_LoadWAV("res/sfx/monster.wav"); //died
    if (monsterSFX[0] == NULL) {
        printf( "Failed to load monster sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    monsterSFX[1] = Mix_LoadWAV("res/sfx/monster1.wav"); //hit
    if (monsterSFX[1] == NULL) {
        printf( "Failed to load monster sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

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

void Game::start_BredatorySFX() {//phát âm thanh khi xảy ra vụ nổ
    Mix_PlayChannel(-1, BredatorySFX, -1);//phát âm thanh explosionSFX trong kênh âm thanh tùy ý, Tham số thứ 3 (-1) cho biết số lần lặp lại của âm thanh, trong trường hợp này là vô hạn.
    Mix_VolumeChunk(BredatorySFX, 0);//đặt âm lượng của âm thanh explosionSFX là 0 (không có âm thanh), điều này có nghĩa là âm thanh được tắt ngay sau khi được phát.
}

void Game::playMusic() {
    Mix_VolumeChunk(BredatorySFX, 80);//đặt âm lượng của explosionSFX (hiệu ứng âm thanh nổ) là 80.
    if (Mix_PlayingMusic() == 0) {//nếu không phát nhạc nền
        Mix_FadeInMusic(backgroundMusic, -1, 1000);//phát nhạc nền
        Mix_VolumeMusic(50);//âm lượng
    }
    else if (Mix_PausedMusic() == 1) Mix_ResumeMusic();//bài nhạc tạm dừng ->tiếp tục phát nhạc bằng hàm Mix_ResumeMusic().
    else if (playerList.at(0).isDead()) Mix_HaltMusic();//chết->phương thức sẽ dừng phát nhạc bằng hàm Mix_HaltMusic().
}

void Game::pauseMusicTime() {
    Mix_VolumeChunk(BredatorySFX, 0);//đặt âm lượng nổ = 0 để tạm dừng các âm thanh khác
    if (Mix_PlayingMusic() == 1) {// nếu đang phát nhạc
        Mix_PauseMusic();//dừng nhạc
    }
    fpsTimer.pause();//dừng đồng hồ đếm fps
}

void Game::render_update_bullet() {
    //render & update bullet
    vector<Bullet*> bulletList = playerList.at(0).getBulletList();
    for (int i = 0; i < playerList.at(0).getBulletList().size(); i++) {
        if (bulletList.at(i) != NULL) {
            if (bulletList.at(i)->isMoving()) {
                bulletList.at(i)->render(camera, BulletTex);
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
    if (exCounter / 4 >= 8) exCounter = 12;
}

void Game::render_update_Game() {
    //start timer
    fpsTimer.start();//ls24

    commonFunc::clearRenderer();
    render_update_LevelPart();
    render_update_bullet();
    render_update_player();
    render_update_monster();
    render_update_Bredatory();

    FPSCounter();
    
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