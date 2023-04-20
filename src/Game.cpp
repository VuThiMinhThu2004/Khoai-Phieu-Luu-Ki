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
    if (!commonFunc::loadFont("res/ariblk.ttf")) success = false;

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

    BackgroundTex = commonFunc::loadTexture("res/images/Background.jpg");
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

    if (!success) 
        printf( "FAILED TO LOAD MEDIA: %s\n", SDL_GetError());

    return success;
}

//copy lesson 24: Calculate and correct fps
void Game::FPSCounter() {//tính toán số khung hình trung bình trong một giây (FPS) của trò chơi và hiển thị số FPS đó trên màn hình.
    int avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);//tính số khung hình đã được hiển thị trong một giây
    if (avgFPS > 2000000) {
        avgFPS = 0;// tránh tràn số.
    }
    //Set text to be rendered
    timeText.str("");
    timeText << "FPS: " << avgFPS;

    SDL_Color whiteColor = { 255,255,255,255 };
    SDL_Texture* textTex = commonFunc::createText(timeText.str().c_str(), whiteColor);
    Entity text(64*3, 0, textTex);
    commonFunc::renderTexture(text);
    ++countedFrames;//biến đếm tần số được tăng lên để chu kỳ tính FPS tiếp theo có thể bắt đầu./tang bo dem khung
}


void Game::renderScore() {
    if (score < playerList.at(0).getX() / TILE_WIDTH) score = playerList.at(0).getX() / TILE_WIDTH;
    scoreText.str("");
    scoreText << "Score: " << score << "m";//ds htai
    SDL_Color whiteColor = { 255,255,255,255 };
    SDL_Color yellowColor = { 252,226,5,255 };

////kiểm tra xem điểm số hiện tại có lớn hơn điểm số cao nhất đã lưu trữ bằng cách đọc từ tệp tin "res/highscore.txt"
    ifstream readFile;
    readFile.open("res/highscore.txt");
    if (readFile.fail()) update_hight_score();//tệp tin không tồn tại hoặc không mở được, hàm sẽ gọi một hàm khác là update_high_score để tạo tệp tin và ghi điểm số hiện tại vào tệp tin như là điểm số cao nhất.
    readFile >> highscore;
    highscoreText.str("");
    highscoreText << "High Score: " << highscore << "m";//ki luc


//tạo ra hai đối tượng SDL_Texture cho điểm số và điểm số cao nhất bằng cách sử dụng hàm createText từ không gian tên commonFunc
    SDL_Texture* scoreTex = commonFunc::createText(scoreText.str().c_str(), yellowColor);
    SDL_Texture* highscoreTex = commonFunc::createText(highscoreText.str().c_str(), whiteColor);

//hai đối tượng Entity được tạo ra sử dụng các đối tượng texture cho điểm số và điểm số cao nhất và vị trí của chúng được đặt trên màn hình. 
    Entity current_score(1100, 30, scoreTex);
    Entity high_score(1100, 0, highscoreTex);

//renderTexture để vẽ cả hai đối tượng Entity điểm số và điểm số cao nhất lên màn hình.
    commonFunc::renderTexture(current_score);
    commonFunc::renderTexture(high_score);
}
//xem lai
bool Game::createMap() {

}

bool Game::createLevel() {

}

bool Game::createPlayer() {
    Player Potato(64 * 3, LEVEL_HEIGHT - TILE_HEIGHT * 5, PotatoTex);
    playerList.push_back(Potato);
    if (playerList.size() < 0) return false;
    return true;
}

bool Game::createMonster() {

//Các con dua chuot được tạo dựa trên những vị trí con dua chuot được lưu trữ trong các đối tượng LevelPart trong vector LevelPartList
    for (int i = 0; i < LevelPartList.size(); i++) {
        if (LevelPartList.at(i).getMonsterPos().size() > 0) {
//có bất kỳ vị trí con dua chuot nào, hàm lại tiếp tục một vòng lặp khác chạy qua một nửa số lượng vị trí con dua chuot
            for (int j = 0; j < LevelPartList.at(i).getMonsterPos().size() - 1; j += 2) {

//tạo monster mới với tọa độ x và y được tính toán dựa trên vị trí con xương sống hiện tại và vị trí của đối tượng LevelPart hiện tại
                Monster* monster = new Monster(LevelPartList.at(i).getMonsterPos().at(j) * TILE_WIDTH + LevelPartList.at(i).getX(), LevelPartList.at(i).getMonsterPos().at(j + 1) * TILE_WIDTH + LevelPartList.at(i).getY(), MonsterTex);
                MonsterList.push_back(monster);
            }
        }
    }
    return true;
}

bool Game::createMenu() {
    Menu gameMenu(ButtonTex, BackgroundTex, BackgroundTex);// (hình ảnh cho các nút, hình nền phía trước và sau của menu).
    menuList.push_back(gameMenu);
    if (menuList.size() < 0) return false;
    return true;
}

void Game::start_BredatorySFX() {//phát âm thanh khi xảy ra vụ nổ
    Mix_PlayChannel(-1, BredatorySFX, -1);//-1 la so lam lap vo han
    Mix_VolumeChunk(BredatorySFX, 0);//đặt là 0 (không có âm thanh), điều này có nghĩa là âm thanh được tắt ngay sau khi được phát.
}

void Game::playMusic() {
    Mix_VolumeChunk(BredatorySFX, 80);//đặt âm lượng là 80.
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

void Game::render_update_player() {
    playerList.at(0).update(LevelPartList, MonsterList, playerSFX, camera);
    playerList.at(0).handleCamera(camera, camVel);
    playerList.at(0).render(camera);
}

void Game::render_update_monster() {//giong xoa bullet
    for (int i = 0; i < MonsterList.size(); i++) {
        if (MonsterList.at(i) != NULL) {
            if (!MonsterList.at(i)->isDead()) {//đang sống
            // cập nhật vị trí và hướng đi của nó, sau đó vẽ nó trên màn hình.
                MonsterList.at(i)->render(camera);
                MonsterList.at(i)->update(playerList.at(0), LevelPartList, monsterSFX, camera);
            }
            else {
                delete MonsterList.at(i);
                MonsterList.at(i) = NULL;
                MonsterList.erase(MonsterList.begin() + i);
            }
        }
    }
}

void Game::render_update_bullet() {
    // lấy danh sách đạn từ người chơi và lặp qua từng đạn trong danh sách

    vector<Bullet*> bulletList = playerList.at(0).getBulletList();
    for (int i = 0; i < playerList.at(0).getBulletList().size(); i++) {
        if (bulletList.at(i) != NULL) {
            //đạn đang di chuyển, nó sẽ được vẽ trên màn hình và được cập nhật trong đối tượng LevelPartList 
            //để phát hiện va chạm với các đối tượng khác.
            if (bulletList.at(i)->isMoving()) {
                bulletList.at(i)->render(camera, BulletTex);
                 bulletList.at(i)->update(LevelPartList);
            }
            else {
                //đạn đã kết thúc chuyến đi của nó, nó sẽ bị xóa khỏi danh sách và bộ đếm đạn của người chơi được cập nhật
                // Nếu đối tượng đạn là NULL, nó sẽ bị bỏ qua.
                delete bulletList.at(i);
                bulletList.at(i) = NULL;
                bulletList.erase(bulletList.begin() + i);
                playerList.at(0).setBulletList(bulletList);
            }
        }
    }
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
                Monster* monster = new Monster(LevelPartList.at(0).getMonsterPos().at(j) * TILE_WIDTH + LevelPartList.at(0).getX(), LevelPartList.at(0).getMonsterPos().at(j + 1) * TILE_WIDTH + LevelPartList.at(0).getY(), MonsterTex);
                MonsterList.push_back(monster);
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




void Game::render_update_Bredatory() {
    //render explosion
    commonFunc::renderAnimation(BredatoryTex, camera.x - 64, 0, Bredatory[exCounter / 4], camera);
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

    commonFunc::clearRenderer();
    render_update_LevelPart();
    render_update_bullet();
    render_update_player();
    render_update_monster();
    render_update_Bredatory();

    FPSCounter();

    renderScore();
    if (playerList.at(0).isDead()) {
        menuList.at(0).renderRetryMenu();
        if (score > highscore) highscore = score;
        update_hight_score();
    }
    fpsTimer.unpause();
    if (menuList.at(0).need_reseting()) resetGame();
    
    
    commonFunc::renderPresent();
}

void Game::resetGame() {
    playerList.at(0).resetPlayer();
    camera.x = 0;
    camera.y = 0;
    camVel = 1.5;
    //xóa monster
    if(!MonsterList.empty())
        for (int i = MonsterList.size() - 1; i >= 0; i--) {
            delete MonsterList.at(i);
            MonsterList.at(i) = NULL;
            MonsterList.erase(MonsterList.begin() + i);
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

void Game::handleGameInput(SDL_Event& event) {
    if (event.type == SDL_QUIT) gameRunning = false;
    menuList.at(0).handleInputActive(event, gameRunning, playerList.at(0));
    if (!menuList.at(0).isMenu() && !menuList.at(0).isPaused()) playerList.at(0).handleInputActive(event, playerSFX);
}