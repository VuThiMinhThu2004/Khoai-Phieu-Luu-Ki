#include "LevelPart.h"

LevelPart::LevelPart(int p_x, int p_y, const char* p_path, SDL_Texture* p_tileTex)
    :x(p_x), y(p_y)
{
    //Success flag
    bool tilesLoaded = true;

    //The tile offsets
    int x = getX(), y = getY();

    //Mở map
    ifstream map(p_path);

    //Nếu ko đọc đc dữ liệu trong map
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

            //Đọc từ map
            map >> tileType;

            //Debug
            if (map.fail()) {
                printf("Error loading map: Unexpected end of file!\n");
                tilesLoaded = false;
                break;
            }

            //Nếu như đọc đc tileType và thỏa mãn
            if ((tileType >= 0) && (tileType < TOTAL_TILE_SPRITES)) {
                Tile* tile = new Tile(x, y, p_tileTex, tileType);
                tilesList.push_back(tile);
            }
            //Nếu như tileType ko thoả mãn
            else {
                printf("Error loading map: Invalid tile type at %d!\n", i);
                tilesLoaded = false;
                break;
            }
            //Dịch x của tile tiếp theo: o tiep theo dc dat ben canh o truoc
            x += TILE_WIDTH;

            //Nếu như đến cuoi cap
            if (x >= getX() + LEVEL_WIDTH) {
                //hàng hiện tại của các ô đã hoàn thành và cần bắt đầu hàng tiếp theo.
                //Xuống dòng mới và làm lại
                x = getX();
                y += TILE_HEIGHT;
            }
        }
    }
    map.close();
}

void LevelPart::render(SDL_Rect p_TileClips[], SDL_Rect& p_camera) {
     //duyệt qua danh sách tilesList và vẽ từng Tile trên màn 
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

        /*
        dam bao rằng tất cả các Tile trong đối tượng LevelPart mới được cập nhật vị trí của chúng
        để nằm trong đúng vị trí tương đối so với đối tượng LevelPart được truyền vào làm tham số.
        */
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

//tạo các Tile object từ dữ liệu được lưu trữ trong một file và đặt chúng vào vị trí thích hợp trong màn chơi.
void LevelPart::setTilesType(const char* p_path) {
    bool tilesLoaded = true;
    int x = getX(), y = getY();
    ifstream map(p_path);

    //Nếu ko đọc đc dữ liệu trong map
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

            //Đọc từ map
            map >> tileType;

            //Debug
            if (map.fail()) {
                printf("Error loading map: Unexpected end of file!\n");
                tilesLoaded = false;
                break;
            }

            //Nếu như đọc đc tileType và thỏa mãn
            if ((tileType >= 0) && (tileType < TOTAL_TILE_SPRITES)) {
                tilesList.at(i)->setType(tileType);
            }
            //Nếu như tileType ko thoả mãn
            else {
                printf("Error loading map: Invalid tile type at %d!\n", i);
                tilesLoaded = false;
                break;
            }
            //Dịch x của tile tiếp theo
            x += TILE_WIDTH;

            //Nếu như đến giới hạn level
            if (x >= getX() + LEVEL_WIDTH) {
                //Xuống dòng mới và làm lại
                x = getX();
                y += TILE_HEIGHT;
            }
        }
    }
    map.close();
}

