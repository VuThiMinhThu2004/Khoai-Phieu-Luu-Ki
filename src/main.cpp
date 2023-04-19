#include <sstream>
#include <fstream>

#include "RenderWindow.h"
#include "Entity.h"
#include "Player.h"
#include "Timer.h"
using namespace std;

SDL_Event event;

SDL_Texture* BackgroundTex = NULL;
SDL_Texture* PotatoTex = NULL;
SDL_Texture* TileTex = NULL;
SDL_Texture* BulletTex = NULL;

//FPScounter
LTimer fpsTimer;
stringstream timeText;
int countedFrames = 0;


//Các biến SDL_Rect để lưu trữ các thông số của các đối tượng trên màn hình.

SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
float camVel = 1.5;
SDL_Rect gTileClips[TOTAL_TILE_SPRITES];
Tile* tileSet[TOTAL_TILES];
bool init();

Player Potato(0, 0, PotatoTex);
Entity Background(0, 0, BackgroundTex);

void FPSCounter();
bool setTiles(Tile* tiles[]);
void gameloop();


// int main(int argc, char* argv[]) {
//     init();
    
//     if (!setTiles(tileSet)) {
//         printf("Failed to load tile set!\n");
//     }
            
//     gameloop();
//     //Giải phóng bộ nhớ
//     //Giải phóng bộ nhớ
//     commonFunc::cleanUp();
//     return 0;
// }

bool init(){//lazyfoo
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

    if (success == true) commonFunc::renderWindow("KHOAI PHIEU LUU", SCREEN_WIDTH, SCREEN_HEIGHT);
    
    BackgroundTex = commonFunc::loadTexture("res/images/Background.jpg");
    PotatoTex = commonFunc::loadTexture("res/images/Potato.png");
    TileTex = commonFunc::loadTexture("res/images/TileMap.png");

    commonFunc::loadFont("res/lazy.ttf");

    return success;
}

//copy lesson 24: Calculate and correct fps
void FPSCounter() {//tính toán số khung hình trung bình trong một giây (FPS) của trò chơi và hiển thị số FPS đó trên màn hình.
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
    SDL_Color whiteColor = { 255,255,255,255 };
    SDL_Texture* textTex = commonFunc::createText(timeText.str().c_str(), whiteColor);
    Entity text(64*3, 0, textTex);
    commonFunc::renderTexture(text);
    ++countedFrames;//biến đếm tần số được tăng lên để chu kỳ tính FPS tiếp theo có thể bắt đầu./tang bo dem khung
}

bool setTiles(Tile* tiles[]) {
    //Success flag
    bool tilesLoaded = true;
    //The tile offsets
    int x = 0, y = 0;
    //Open the map
    ifstream map("res/map/map.map");
    //If the map couldn't be loaded
    if (map.fail()) {
        printf("Unable to load map file!\n");
        tilesLoaded = false;
    }
    else
    {
        //Initialize the tiles
        for (int i = 0; i < TOTAL_TILES; ++i) {
            //Determines what kind of tile will be made
            int tileType = -1;
            //Read tile from map file
            map >> tileType;
            //If the was a problem in reading the map
            if (map.fail()) {
                //Stop loading map
                printf("Error loading map: Unexpected end of file!\n");
                tilesLoaded = false;
                break;
            }
            //If the number is a valid tile number
            if ((tileType >= 0) && (tileType < TOTAL_TILE_SPRITES)) {
                tiles[i] = new Tile(x, y, TileTex, tileType);
            }
            //If we don't recognize the tile type
            else
            {
                //Stop loading map
                printf("Error loading map: Invalid tile type at %d!\n", i);
                tilesLoaded = false;
                break;
            }
            //Move to next tile spot
            x += TILE_WIDTH;
            //If we've gone too far
            if (x >= LEVEL_WIDTH) {
                //Move back
                x = 0;
                //Move to the next row
                y += TILE_HEIGHT;
            }
        }
        if (tilesLoaded)
        {
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
        }
    }
    //Close the file
    map.close();
    //If the map was loaded fine
    return tilesLoaded;
}
int main(int argc, char* argv[]) {
    
    commonFunc::loadFont("res/lazy.ttf");
    if ( !init() ) {
        printf("Failed to initialize!\n");
        return 0;
    }

    if (!setTiles(tileSet)) {
        printf("Failed to load tile set!\n");
    }

    Player Potato(0, 0, PotatoTex);
    Entity Background(0, 0, BackgroundTex);
 
    fpsTimer.start();
    SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    
    //Game loop
    bool gameRunning = true;
     
    SDL_Event event;
    while (gameRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) gameRunning = false;
            Potato.handleInputActive(event);
        }
        Potato.update();
        Potato.handleCamera(camera, camVel);
        commonFunc::clearRenderer();
        for (int i = 0; i < TOTAL_TILES; i++) {
            commonFunc::renderTile(*tileSet[i], gTileClips[tileSet[i]->getType()], camera);
        }
            
       
        commonFunc::renderTexture(Background, &camera);
        commonFunc::renderPlayer(Potato, camera);
        FPSCounter();
        commonFunc::renderPresent();
    }
    //Giải phóng bộ nhớ
    commonFunc::cleanUp();
    return 0;
}