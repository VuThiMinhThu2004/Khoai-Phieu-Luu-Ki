#include "RenderWindow.h"

using namespace std;

void commonFunc::renderWindow(const char* p_title, int p_width, int p_height) {//lazyfoo
	//Khởi tạo cửa sổ
	window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, p_width, p_height, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
	}
	//Tạo renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL) {
		printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
	}
}

//Loads individual image as texture: lazyfoo
SDL_Texture* commonFunc::loadTexture(const char* p_filePath) {
	SDL_Texture* texture = NULL;
	texture = IMG_LoadTexture(renderer, p_filePath);
	if (texture == NULL) 
		printf( "Failed to load texture image: %s\n", IMG_GetError() );
		//cerr << "Texture has failed to load: " << IMG_GetError() << endl;
	return texture;
}


//Copy texture vào renderer và render
void commonFunc::renderTexture(Entity& entity, SDL_Rect *rec, SDL_Rect *camera, double angle, SDL_Point* center, SDL_RendererFlip flip) {
	//mặc định là in toàn bộ texture 
	SDL_Rect dst = { entity.getX(), entity.getY(), entity.getFrame().w, entity.getFrame().h };

	if (rec != NULL) {
		dst.w = rec->w;
		dst.h = rec->h;
	}

	if (camera != NULL) {
		dst.x = dst.x - camera->x;
		dst.y = dst.y - camera->y;
	}

	SDL_RenderCopyEx(renderer, entity.getTex(), rec, &dst, angle, center, flip);
}


void commonFunc::renderTexture(SDL_Texture* p_tex, int p_x, int p_y, int p_w, int p_h, SDL_Rect* rec, SDL_Rect* camera, double angle, SDL_Point* center, SDL_RendererFlip flip) {
	SDL_Rect dst = { p_x, p_y, p_w, p_h };

	if (rec != NULL) {
		dst.w = rec->w;
		dst.h = rec->h;
	}

	if (camera != NULL) {
		dst.x = dst.x - camera->x;
		dst.y = dst.y - camera->y;
	}

	SDL_RenderCopyEx(renderer, p_tex, rec, &dst, angle, center, flip);
}

//vẽ một Tile : lesson 39
void commonFunc::renderTile(Entity& entity, SDL_Rect& rec, SDL_Rect& camera) {
	SDL_Rect dst = { entity.getX() - camera.x, entity.getY() - camera.y, rec.w, rec.h };
	SDL_RenderCopy(renderer, entity.getTex(), &rec, &dst);
}


void commonFunc::renderAnimation(SDL_Texture* p_tex, int p_x, int p_y, SDL_Rect &p_clip, SDL_Rect &p_camera, double p_angle, SDL_Point* p_center, SDL_RendererFlip p_flip) {
   	SDL_Rect src = { p_clip.x, p_clip.y, p_clip.w, p_clip.h };
	SDL_Rect dst = { p_x - p_camera.x, p_y - p_camera.y, p_clip.w, p_clip.h};
	
	SDL_RenderCopyEx(renderer, p_tex, &p_clip, &dst, p_angle, p_center, p_flip);
}


void commonFunc::renderPlayer(Entity& entity, SDL_Rect &camera) {
	SDL_Rect src = { 0, 0, entity.getFrame().w, entity.getFrame().h };
	SDL_Rect dst = {(entity.getX() - camera.x), (entity.getY() - camera.y), (entity.getFrame().w), (entity.getFrame().h) };
	SDL_RenderCopy(renderer, entity.getTex(), &src, &dst);
}

//Load font: lesson 16
bool commonFunc::loadFont(const char* filePath) {
	TTF_CloseFont(font);
	font = TTF_OpenFont(filePath, 28);
	if (font == NULL) {
		printf ("Failed to load lazy font! SDL_ttf Error: %s\n",TTF_GetError());
		return false;
	}
	return true;
}

//coppy lesson16
SDL_Texture* commonFunc::createText(string textureText, SDL_Color textColor) {
    //Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText.c_str(), textColor);//tạo ra một bề mặt (surface) văn bản dựa trên font đã được load trước đó
	SDL_Texture* mtexture = NULL;
	if (textSurface == NULL) {
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else {
		//Create texture from surface pixels
		mtexture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (mtexture == NULL) {
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		//Get rid of old surface
		SDL_FreeSurface(textSurface);
		return mtexture;
	}
	return NULL;
}


void commonFunc::Color() {
	surface = SDL_GetWindowSurface(window);

	//SDL_MapRGB() được sử dụng để chuyển đổi giá trị RGB sang giá trị pixel tương ứng với định dạng màu của surface.
	SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 255, 0, 0));
	SDL_UpdateWindowSurface(window);
}



void commonFunc::clearRenderer() {
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);//white
	SDL_RenderClear(renderer);
}

void commonFunc::renderPresent() {
	SDL_RenderPresent(renderer);//Update screen
}

void commonFunc::cleanUp() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);


	window = NULL;
	renderer = NULL;


	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

//lazyfoo 27
bool commonFunc::checkCollision(SDL_Rect a, SDL_Rect b) {
	//tọa độ các cạnh của hình chữ nhật a và b.
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	leftA = a.x;
	rightA = a.x + a.w;
	topA = a.y;
	bottomA = a.y + a.h;

	leftB = b.x;
	rightB = b.x + b.w;
	topB = b.y;
	bottomB = b.y + b.h;

	//chiều dọc.
	if (bottomA <= topB) {
		return false;
	}

	if (topA >= bottomB) {
		return false;
	}

	//chiều ngang.
	if (rightA <= leftB) {
		return false;
	}

	if (leftA >= rightB) {
		return false;
	}

	return true;
}

//ls 28: kiểm tra xem hình chữ nhật box có chạm với tile không.
bool commonFunc::touchesWood(SDL_Rect& box, vector<LevelPart>& LevelPartList) {
	for (int i = 0; i < LevelPartList.size(); i++) {
		if (box.x > LevelPartList.at(i).getX() && box.x + box.w + 13 < LevelPartList.at(i).getX() + LEVEL_WIDTH && box.y >= 0 && box.y < LEVEL_HEIGHT - TILE_HEIGHT) {
			int collumn_left = (box.x - LevelPartList.at(i).getX()) / TILE_WIDTH;
			int collumn_rigth = collumn_left + 1;
			int row_up = box.y / TILE_HEIGHT;
			int row_down = row_up + 1;

			int stt1 = row_up * 21 + collumn_rigth;
			int stt2 = row_down * 21 + collumn_rigth;
			int stt3 = row_up * 21 + collumn_left;
			int stt4 = row_down * 21 + collumn_left;

			if ((LevelPartList.at(i).getTilesList().at(stt1)->getType() >= 0) && (LevelPartList.at(i).getTilesList().at(stt1)->getType() <= 84))
				if (checkCollision(box, LevelPartList.at(i).getTilesList().at(stt1)->getCollision())) return true;

			if ((LevelPartList.at(i).getTilesList().at(stt2)->getType() >= 0) && (LevelPartList.at(i).getTilesList().at(stt2)->getType() <= 84))
				if (checkCollision(box, LevelPartList.at(i).getTilesList().at(stt2)->getCollision())) return true;


			if ((LevelPartList.at(i).getTilesList().at(stt3)->getType() >= 0) && (LevelPartList.at(i).getTilesList().at(stt3)->getType() <= 84))
				if (checkCollision(box, LevelPartList.at(i).getTilesList().at(stt3)->getCollision())) return true;

			if ((LevelPartList.at(i).getTilesList().at(stt4)->getType() >= 0) && (LevelPartList.at(i).getTilesList().at(stt4)->getType() <= 84))
				if (checkCollision(box, LevelPartList.at(i).getTilesList().at(stt4)->getCollision())) return true;
		}
	}
	return false;
}



bool commonFunc::touchesWood(SDL_Rect& box, vector<LevelPart>& LevelPartList, bool& grounded, int& groundSTT, int& levelSTT) {

	bool check = false;
	for (int i = 0; i < LevelPartList.size(); i++) {
		if (box.x + box.w + 12 >= LevelPartList.at(i).getX() && box.x <= LevelPartList.at(i).getX() + LEVEL_WIDTH && box.y >= 0 && box.y < LEVEL_HEIGHT - TILE_HEIGHT) {
			int column_left = (box.x - LevelPartList.at(i).getX()) / TILE_WIDTH;
			int column_right = column_left + 1;
			int row_up = box.y / TILE_HEIGHT;
			int row_down = row_up + 1;


			//tính toán các giá trị đại diện cho các ô vuông trong một ma trận hình chữ nhật.
			int stt1 = row_up * 21 + column_right;
			int stt2 = row_down * 21 + column_right;
			int stt3 = row_up * 21 + column_left;
			int stt4 = row_down * 21 + column_left;
			

			if (box.x <= LevelPartList.at(i).getX() && box.x + box.w + 12 >= LevelPartList.at(i).getX() || box.x <= LevelPartList.at(i).getX() + LEVEL_WIDTH && box.x + box.w + 12 >= LevelPartList.at(i).getX() + LEVEL_WIDTH) {
				grounded = false;
			}
			else {
				//ktra box có va chạm với Tile tương ứng không
				if ((LevelPartList.at(i).getTilesList().at(stt1)->getType() >= 0) && (LevelPartList.at(i).getTilesList().at(stt1)->getType() <= 84))
					if (checkCollision(box, LevelPartList.at(i).getTilesList().at(stt1)->getCollision())) check = true;

				if ((LevelPartList.at(i).getTilesList().at(stt2)->getType() >= 0) && (LevelPartList.at(i).getTilesList().at(stt2)->getType() <= 84))
					if (checkCollision(box, LevelPartList.at(i).getTilesList().at(stt2)->getCollision())) check = true;


				if ((LevelPartList.at(i).getTilesList().at(stt3)->getType() >= 0) && (LevelPartList.at(i).getTilesList().at(stt3)->getType() <= 84))
					if (checkCollision(box, LevelPartList.at(i).getTilesList().at(stt3)->getCollision())) check = true;

				if ((LevelPartList.at(i).getTilesList().at(stt4)->getType() >= 0) && (LevelPartList.at(i).getTilesList().at(stt4)->getType() <= 84))
					if (checkCollision(box, LevelPartList.at(i).getTilesList().at(stt4)->getCollision())) check = true;


				if ((LevelPartList.at(i).getTilesList().at(stt2)->getType() > 84) && (LevelPartList.at(i).getTilesList().at(stt4)->getType() > 84)) grounded = false;
				if ((LevelPartList.at(i).getTilesList().at(stt4)->getType() > 84) && (LevelPartList.at(i).getTilesList().at(stt2)->getType() <= 84) && box.x + box.w <= LevelPartList.at(i).getTilesList().at(stt2)->getX()) grounded = false;
			}
				
			groundSTT = stt4;
			levelSTT = i;
		}
	}
	return check;
}