#include "LevelPart.h"

//lesson31
LevelPart::LevelPart(int p_x, int p_y, const char* p_path, SDL_Texture* p_tileTex)
    :x(p_x), y(p_y)
{
    bool tilesLoaded = true;

    //The tile offsets
    int x = getX(), y = getY();

    ifstream map(p_path);

    if (map.fail()) {
        printf("Unable to load map file!\n");
        tilesLoaded = false;
    }
    else
    {
        //Tạo các tile
        for (int i = 0; i < TOTAL_TILES; ++i) {
            //Chọn loại cho tile
            int tileType = -1;

            map >> tileType;

            if (map.fail()) {
                printf("Error loading map: Unexpected end of file!\n");
                tilesLoaded = false;
                break;
            }

            if ((tileType >= 0) && (tileType < TOTAL_TILE_SPRITES)) {
                Tile* tile = new Tile(x, y, p_tileTex, tileType);
                tilesList.push_back(tile);
            }
            else {
                printf("Error loading map: Invalid tile type at %d!\n", i);
                tilesLoaded = false;
                break;
            }
            //Dịch x của tile tiếp theo
            x += TILE_WIDTH;

            //Cuối level
            if (x >= getX() + LEVEL_WIDTH) {                
                x = getX();
                y += TILE_HEIGHT;////Xuống dòng mới và làm lại
            }
        }
    }
    map.close();
}

void LevelPart::render(SDL_Rect p_TileClips[], SDL_Rect& p_camera) {
    for (int i = 0; i < tilesList.size(); i++) {
        commonFunc::renderTile(*tilesList.at(i), p_TileClips[tilesList.at(i)->getType()], p_camera);
    }
}

void LevelPart::setLevelX(LevelPart& p_level) {
    x = p_level.getX() + LEVEL_WIDTH;
    for (int i = 0; i < tilesList.size(); i++) {
        int row = i / 21;
        int collumn = (i - row * 21) * TILE_WIDTH + getX();
        tilesList.at(i)->setX(collumn);
    }
}

void LevelPart::setLevelX(int p_x) {
    x = p_x;
    for (int i = 0; i < tilesList.size(); i++) {
        int row = i / 21;
        int collumn = (i - row*21) * TILE_WIDTH + getX();
        tilesList.at(i)->setX(collumn);
    }
}

void LevelPart::setTilesType(const char* p_path) {
    bool tilesLoaded = true;
    int x = getX(), y = getY();
    ifstream map(p_path);

    if (map.fail()) {
        printf("Unable to load map file!\n");
        tilesLoaded = false;
    }
    else
    {
        //Tạo các tile
        for (int i = 0; i < TOTAL_TILES; ++i) {
            //Chọn loại cho tile
            int tileType = -1;

            map >> tileType;

            if (map.fail()) {
                printf("Error loading map: Unexpected end of file!\n");
                tilesLoaded = false;
                break;
            }

            if ((tileType >= 0) && (tileType < TOTAL_TILE_SPRITES)) {
                tilesList.at(i)->setType(tileType);
            }
            else {
                printf("Error loading map: Invalid tile type at %d!\n", i);
                tilesLoaded = false;
                break;
            }
            //Dịch x của tile tiếp theo
            x += TILE_WIDTH;

            //Đến giới hạn level
            if (x >= getX() + LEVEL_WIDTH) {
                x = getX();
                y += TILE_HEIGHT;
            }
        }
    }
    map.close();
}

