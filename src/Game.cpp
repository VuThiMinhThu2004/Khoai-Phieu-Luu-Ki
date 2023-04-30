#include "Game.h"
using namespace std;


bool Game::init() {//lazyfoo
    //Initialization flag
    bool success = true;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }

    if (!(IMG_Init(IMG_INIT_PNG))) {//khởi tạo thư viện hỗ trợ định dạng hình ảnh PNG và JPG
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
        success = false;
    }

    if (!(IMG_Init(IMG_INIT_JPG))) {
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
        success = false;
    }

    if (TTF_Init() == -1) {// tạo thư viện hỗ trợ định dạng font chữ TrueType (TTF).
        printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    //khởi tạo thư viện hỗ trợ âm thanh SDL_mixe
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    if (success == true) commonFunc::renderWindow("KHOAI PHIEU LUU KI", SCREEN_WIDTH, SCREEN_HEIGHT);

    return success;
}

bool Game::loadMedia() {//lazyfoo 
    bool success = true;//Loading success flag


    if (!commonFunc::loadFont("res/Pixel-UniCode.ttf")) success = false;

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

//Để tải nhạc, chúng tôi gọi mix_loadmus và để tải hiệu ứng âm thanh, chúng tôi gọi là mix_loadwav.

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

    playerSFX[0] = Mix_LoadWAV("res/sfx/playeHit.wav"); // hit
    if (playerSFX[0] == NULL) {
        printf( "Failed to load player sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    playerSFX[1] = Mix_LoadWAV("res/sfx/playerJump.wav"); //jumping
    if (playerSFX[1] == NULL) {
        printf( "Failed to load player sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    playerSFX[2] = Mix_LoadWAV("res/sfx/playerLanding.wav"); //landing
    if (playerSFX[2] == NULL) {
        printf( "Failed to load player sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    playerSFX[3] = Mix_LoadWAV("res/sfx/playerLaser.wav"); //shooting
    if (playerSFX[3] == NULL) {
        printf( "Failed to load player sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    monsterSFX[0] = Mix_LoadWAV("res/sfx/shooting.wav"); //died
    if (monsterSFX[0] == NULL) {
        printf( "Failed to load skeleton sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    monsterSFX[1] = Mix_LoadWAV("res/sfx/damageHit.wav"); //hit
    if (monsterSFX[1] == NULL) {
        printf( "Failed to load skeleton sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    if (!success) cout << "FAILED TO LOAD MEDIA: " << SDL_GetError() << endl;
    
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


/*
Hàm sau đó tạo một đối tượng Entity để hiển thị số FPS trên màn hình. 
Nó sử dụng hàm createText() trong commonFunc để tạo một SDL_Texture chứa nội dung của chuỗi timeText,
 đại diện cho số FPS. Sau đó, nó tạo một đối tượng Entity với vị trí và SDL_Texture được tạo ở trên, 
 và sau đó gọi hàm renderTexture() trong commonFunc để hiển thị đối tượng Entity trên màn hình.
*/
    SDL_Color yellowColor = { 252,226,5,255 };
    SDL_Texture* textTex = commonFunc::createText(timeText.str().c_str(), yellowColor);
    Entity text(64*18, 0, textTex);
    commonFunc::renderTexture(text);
    ++countedFrames;//biến đếm tần số được tăng lên để chu kỳ tính FPS tiếp theo có thể bắt đầu./tang bo dem khung
}

void Game::HpCounter() {//tính toán số mang của trò chơi và hiển thị trên màn hình.
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
    scoreText << "Score: " << score << "m";//ds htai
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
    SDL_Texture* highscoreTex = commonFunc::createText(highscoreText.str().c_str(), yellowColor);

//hai đối tượng Entity được tạo ra sử dụng các đối tượng texture cho điểm số và điểm số cao nhất và vị trí của chúng được đặt trên màn hình. 
    Entity current_score(64*3, 30, scoreTex);
    Entity high_score(64*3, 0, highscoreTex);

//renderTexture để vẽ cả hai đối tượng Entity điểm số và điểm số cao nhất lên màn hình.
    commonFunc::renderTexture(current_score);
    commonFunc::renderTexture(high_score);
}

bool Game::createMap() {
    path_pos Map1({ }, "res/map/map1.map");
    mapList.push_back(Map1);
    path_pos Map2({ 15,5 }, "res/map/map2.map");//có các vị trí 15 và 5 được đánh dấu trên bản đồ.
    mapList.push_back(Map2);
    path_pos Map3({ 1,9,16,6,18,7 }, "res/map/map3.map");
    mapList.push_back(Map3);
    path_pos Map4({ 9,9 }, "res/map/map4.map");
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

//một số ngẫu nhiên được tạo để chọn một bản đồ từ vector mapList.
    for (int i = 0; i < TOTAL_LEVEL_PART; i++) {
        int random = rand() % (TOTAL_MAP - 1);
        if (i == 0) random = TOTAL_MAP - 1;//bản đồ cuối cùng trong vector (tức bản đồ spawn) sẽ được chọn thay vì một bản đồ ngẫu nhiên.

// x được đặt là i * LEVEL_WIDTH, tọa độ y được đặt là 0, đường dẫn đến tệp bản đồ được đặt là đường dẫn của bản đồ được chọn, và texture cho các ô đất được đặt là tileTex
        LevelPart level(i * LEVEL_WIDTH, 0, mapList.at(random).path, tileTex);

// Vị trí xương của level cũng được thiết lập theo vị trí xương của bản đồ được chọn.
       level.setMonsterPos(mapList.at(random).monsterPos);
//Đối tượng LevelPart được thêm vào vector LevelPartList.
        LevelPartList.push_back(level);

    }
    if (LevelPartList.size() < TOTAL_LEVEL_PART) {
        cout << "FAILED TO CREATE LEVEL!" << endl;
        return false;
    }
    return true;
}

bool Game::createMonster() {

//Các con xương sống được tạo dựa trên những vị trí con xương sống được lưu trữ trong các đối tượng LevelPart trong vector LevelPartList
    for (int i = 0; i < LevelPartList.size(); i++) {
        if (LevelPartList.at(i).getMonsterPos().size() > 0) {
//có bất kỳ vị trí con xương sống nào, hàm lại tiếp tục một vòng lặp khác chạy qua một nửa số lượng vị trí con xương sống.
            for (int j = 0; j < LevelPartList.at(i).getMonsterPos().size() - 1; j += 2) {

//tạo Skeleton mới với tọa độ x và y được tính toán dựa trên vị trí con xương sống hiện tại và vị trí của đối tượng LevelPart hiện tại
//exture cho con xương sống được thiết lập thành skeletonTex. Đối tượng Skeleton được thêm vào vector skeletonList.
                Monster* skeleton = new Monster(LevelPartList.at(i).getMonsterPos().at(j) * TILE_WIDTH + LevelPartList.at(i).getX(), LevelPartList.at(i).getMonsterPos().at(j + 1) * TILE_WIDTH + LevelPartList.at(i).getY(), monsterTex);
                monsterList.push_back(skeleton);
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
    Menu gameMenu(buttonTex, backgroundTex, backgroundTex);// (hình ảnh cho các nút, hình nền phía trước và sau của menu).
    menuList.push_back(gameMenu);
    if (menuList.size() < 0) return false;
    return true;
}

void Game::start_BredatorySFX() {//phát âm thanh khi xảy ra vụ nổ
    Mix_PlayChannel(-1, bredatorySFX, -1);//phát âm thanh explosionSFX trong kênh âm thanh tùy ý, Tham số thứ 3 (-1) cho biết số lần lặp lại của âm thanh, trong trường hợp này là vô hạn.
    Mix_VolumeChunk(bredatorySFX, 0);//đặt âm lượng của âm thanh explosionSFX là 0 (không có âm thanh), điều này có nghĩa là âm thanh được tắt ngay sau khi được phát.
}

void Game::playMusic() {
    Mix_VolumeChunk(bredatorySFX, 80);//đặt âm lượng của explosionSFX (hiệu ứng âm thanh nổ) là 80.
    if (Mix_PlayingMusic() == 0) {//nếu không phát nhạc nền
        Mix_FadeInMusic(backgroundMusic, -1, 1000);//phát nhạc nền
        Mix_VolumeMusic(50);//âm lượng
    }
    else if (Mix_PausedMusic() == 1) Mix_ResumeMusic();//bài nhạc tạm dừng ->tiếp tục phát nhạc bằng hàm Mix_ResumeMusic().
    else if (playerList.at(0).isDead()) Mix_HaltMusic();//chết->phương thức sẽ dừng phát nhạc bằng hàm Mix_HaltMusic().
}

void Game::pauseMusicTime() {
    Mix_VolumeChunk(bredatorySFX, 0);//đặt âm lượng nổ = 0 để tạm dừng các âm thanh khác
    if (Mix_PlayingMusic() == 1) {// nếu đang phát nhạc
        Mix_PauseMusic();//dừng nhạc
    }
    fpsTimer.pause();//dừng đồng hồ đếm fps
}

//cập nhật và vẽ các phần của màn chơi.
void Game::render_update_LevelPart() {
    //update LevelPart
    if (LevelPartList.at(0).getX() - camera.x <= -LEVEL_WIDTH) {//LevelPart vượt quá giới hạn của màn hình (trục x)
        int random = rand() % (TOTAL_MAP - 1);//tạo phần mới = cách random
        LevelPartList.at(0).setTilesType(mapList.at(random).path);//thêm phần mới vào cuối ds
        LevelPartList.at(0).setLevelX(LevelPartList.at(LevelPartList.size() - 1));
        LevelPartList.at(0).setMonsterPos(mapList.at(random).monsterPos);

        if (LevelPartList.at(0).getMonsterPos().size() > 0) {//phần mới chứa skeleton
            for (int j = 0; j < LevelPartList.at(0).getMonsterPos().size() - 1; j += 2) {
        //chương trình sẽ tạo ra một đối tượng Skeleton mới và thêm vào danh sách Skeleton.
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
        LevelPartList.at(i).render(gTileClips, camera);//vẽ = cách sử dụng hình ảnh tile và cập nhật camera để di chuyển màn hình
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
                bulletList.at(i)->render(camera, bulletTex);
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

void Game::render_update_player() {
    playerList.at(0).update(LevelPartList, monsterList, playerSFX, camera);//ập nhật trạng thái của người chơi, bao gồm di chuyển, va chạm với các vật thể khác, bắn đạn, etc.
    playerList.at(0).handleCamera(camera, camVel);//điều khiển camera và di chuyển nó theo vị trí của người chơi.
    playerList.at(0).render(camera);//để vẽ ng chơi lên màn hình
    HpCounter();

}

void Game::render_update_monster() {
    for (int i = 0; i < monsterList.size(); i++) {
        if (monsterList.at(i) != NULL) {//đối tượng Skeleton đang được xử lý
            if (!monsterList.at(i)->isDead()) {//đang sống
            // cập nhật vị trí và hướng đi của nó, sau đó vẽ nó trên màn hình.
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
    exCounter++;//để chọn sprite phù hợp cho hiệu ứng nổ dựa trên số frame đã trôi qu
    if (exCounter / 4 >= 8) exCounter = 12;//co 4 frame
    // số frame đã trôi qua vượt quá 60 (tương đương với việc hiệu ứng đã chạy xong), biến exCounter sẽ được reset lại thành 24 để bắt đầu lại hiệu ứng từ đầu.
}

void Game::update_hight_score() {
    ofstream outFile;
    outFile.open("res/highscore.txt");//mở tệp để ghi
    outFile.clear();
    outFile << highscore;
    outFile.close();
}

// void render_update_heart(){
//     for (auto& heart : heartList) {
// 		heart->render(renderer);
// 	}
// }

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
    
    if (playerList.at(0).isDead()) {//chết
    //hiển thị một menu thử lại và kiểm tra xem điểm số của người chơi có phải là điểm cao mới không-> nó cập nhật điểm cao trong một tệp.
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
    commonFunc::renderPresent();
}

void Game::resetGame() {
    playerList.at(0).resetPlayer();
    camera.x = 0;
    camera.y = 0;
    camVel = 1.5;

    //xóa skeleton
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

