#include "RenderWindow.h"
#include "Entity.h"

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
//tải texture từ đường dẫn p_filePath.
SDL_Texture* commonFunc::loadTexture(const char* p_filePath) {
	SDL_Texture* texture = NULL;
	texture = IMG_LoadTexture(renderer, p_filePath);
	if (texture == NULL) 
		printf( "Failed to load texture image: %s\n", IMG_GetError() );
		//cerr << "Texture has failed to load: " << IMG_GetError() << endl;
	return texture;
}


//Copy texture vào renderer và render
/*
vẽ texture lên renderer theo vị trí của entity, hình dạng được chỉ định bởi rec, camera,
 góc xoay angle, trục quay center, và flip (trạng thái: không quay, quay theo chiều dọc hoặc chiều ngang).
*/
void commonFunc::renderTexture(Entity& entity, SDL_Rect *rec, SDL_Rect *camera, double angle, SDL_Point* center, SDL_RendererFlip flip) {
	//mặc định là in toàn bộ texture ra(hình dạng của texture)
	SDL_Rect dst = { entity.getX(), entity.getY(), entity.getFrame().w, entity.getFrame().h };
	
	//nếu đầu vào rec != NULL thì in ra theo rec
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


/*
Vẽ texture lên renderer theo tọa độ x,y, kích thước p_w,p_h, hình dạng được 
chỉ định bởi rec, camera, góc xoay angle, trục quay center, và flip.
*/
void commonFunc::renderTexture(SDL_Texture* p_tex, float p_x, float p_y, float p_w, float p_h, SDL_Rect* rec, SDL_Rect* camera, double angle, SDL_Point* center, SDL_RendererFlip flip) {
	//mặc định là in toàn bộ texture ra
	SDL_Rect dst = { p_x, p_y, p_w, p_h };

	//nếu đầu vào rec != NULL thì in ra theo rec
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

//vẽ một Tile (một phần trong một màn chơi) lên renderer theo vị trí của entity và hình dạng được chỉ định bởi rec, camera.
void commonFunc::renderTile(Entity& entity, SDL_Rect& rec, SDL_Rect& camera) {
	SDL_Rect dst = { entity.getX() - camera.x, entity.getY() - camera.y, rec.w, rec.h };
	SDL_RenderCopy(renderer, entity.getTex(), &rec, &dst);
}


//vẽ animation lên renderer theo tọa độ x,y, hình dạng animation được chỉ định bởi p_clip, camera, góc xoay angle, trục quay center, và flip.
void commonFunc::renderAnimation(SDL_Texture* p_tex, float p_x, float p_y, SDL_Rect &p_clip, SDL_Rect &p_camera, double p_angle, SDL_Point* p_center, SDL_RendererFlip p_flip) {
   	SDL_Rect src = { p_clip.x, p_clip.y, p_clip.w, p_clip.h };
	SDL_Rect dst = { p_x - p_camera.x, p_y - p_camera.y, p_clip.w, p_clip.h};
	
	//hoạt động giống như SDL_RENDERCOPY ban đầu, nhưng với các đối số bổ sung để xoay và lật.
	SDL_RenderCopyEx(renderer, p_tex, &p_clip, &dst, p_angle, p_center, p_flip);
}


//vẽ Player lên renderer theo vị trí của entity và camera.
void commonFunc::renderPlayer(Entity& entity, SDL_Rect &camera) {
	SDL_Rect src = { 0, 0, entity.getFrame().w, entity.getFrame().h };
	SDL_Rect dst = { float (entity.getX() - camera.x), float(entity.getY() - camera.y),float(entity.getFrame().w), float(entity.getFrame().h) };
	SDL_RenderCopy(renderer, entity.getTex(), &src, &dst);
}

//Load font: lesson 16
bool commonFunc::loadFont(const char* filePath) {
	TTF_CloseFont(font);// đóng font chữ hiện tại (nếu có) 
	font = TTF_OpenFont(filePath, 28);//mở font chữ tại đường dẫn filePath và với kích thước chữ là 28
	if (font == NULL) {
		printf ("Failed to load lazy font! SDL_ttf Error: %s\n",TTF_GetError());
		return false;
	}
	return true;
}

//coppy lesson16
//Create text:tạo ra một texture chứa nội dung văn bản được truyền vào dưới dạng chuỗi (p_text) và màu sắc của văn bản (p_textColor).
SDL_Texture* commonFunc::createText(string textureText, SDL_Color textColor) {
    //Render text surface
	//Điều này tạo ra một bề mặt màu rắn từ phông chữ, văn bản và màu sắc được đưa ra
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText.c_str(), textColor);//tạo ra một bề mặt (surface) văn bản dựa trên font đã được load trước đó
	SDL_Texture* mtexture = NULL;
	if (textSurface == NULL) {//không tạo thành công
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else {
		//Create texture from surface pixels
		//Tạo texture text từ surface:texture được tạo ra từ bề mặt văn bản bằng cách sử dụng hàm SDL_CreateTextureFromSurface().
		mtexture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (mtexture == NULL) {
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		//Get rid of old surface
		SDL_FreeSurface(textSurface);//Sau khi texture được tạo, bề mặt văn bản được giải phóng bằng hàm SDL_FreeSurface().
		return mtexture;
	}
	return NULL;// có lỗi trong quá trình tạo texture
}

//Tô màu cho cửa sổ của ứng dụng bằng màu đỏ. 
void commonFunc::Color() {
	surface = SDL_GetWindowSurface(window);//Lấy thông tin về surface (bề mặt) của cửa sổ
	
	//tô màu cho toàn bộ surface. Tham số thứ hai của hàm này là con trỏ tới vùng diện tích cần tô màu, ở đây đang để là NULL để tô màu cho toàn bộ surface.
	//SDL_MapRGB() được sử dụng để chuyển đổi giá trị RGB sang giá trị pixel tương ứng với định dạng màu của surface.
	//Trong đoạn code này, giá trị R = 255, G = 0 và B = 0 được sử dụng để tạo màu đỏ.
	SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 255, 0, 0));
	SDL_UpdateWindowSurface(window);//cập nhật cửa sổ hiển thị
}



//xóa toàn bộ nội dung của renderer
void commonFunc::clearRenderer() {
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);//thiết lập màu sắc cho renderer để vẽ (trong trường hợp này là màu trắng).
	SDL_RenderClear(renderer);//xóa toàn bộ nội dung của renderer với màu sắc đã được thiết lập trước đó
	//Việc này giúp cho việc vẽ các đối tượng mới lên renderer sau đó được thực hiện trên một bề mặt trống trải.
}

//cập nhật cửa sổ hiển thị, cho phép người dùng xem được nội dung đã được vẽ lên cửa sổ.
void commonFunc::renderPresent() {
	SDL_RenderPresent(renderer);////Update screen
}


//giải phóng tài nguyên và thoát khỏi SDL, Mix, TTF và IMG.
void commonFunc::cleanUp() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	//Đặt lại địa chỉ của cửa sổ và bộ vẽ thành NULL.
	window = NULL;
	renderer = NULL;

	//giải phóng tài nguyên của các thư viện tương ứng.
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();//thoát khỏi SDL và giải phóng tất cả các tài nguyên của nó.
}