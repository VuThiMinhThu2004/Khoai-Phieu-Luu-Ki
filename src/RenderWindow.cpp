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
void commonFunc::renderTexture(SDL_Texture* p_tex, int p_x, int p_y, int p_w, int p_h, SDL_Rect* rec, SDL_Rect* camera, double angle, SDL_Point* center, SDL_RendererFlip flip) {
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
void commonFunc::renderAnimation(SDL_Texture* p_tex, int p_x, int p_y, SDL_Rect &p_clip, SDL_Rect &p_camera, double p_angle, SDL_Point* p_center, SDL_RendererFlip p_flip) {
   	SDL_Rect src = { p_clip.x, p_clip.y, p_clip.w, p_clip.h };
	SDL_Rect dst = { p_x - p_camera.x, p_y - p_camera.y, p_clip.w, p_clip.h};
	
	//hoạt động giống như SDL_RENDERCOPY ban đầu, nhưng với các đối số bổ sung để xoay và lật.
	SDL_RenderCopyEx(renderer, p_tex, &p_clip, &dst, p_angle, p_center, p_flip);
}


//vẽ Player lên renderer theo vị trí của entity và camera.
void commonFunc::renderPlayer(Entity& entity, SDL_Rect &camera) {
	SDL_Rect src = { 0, 0, entity.getFrame().w, entity.getFrame().h };
	SDL_Rect dst = { int (entity.getX() - camera.x), int(entity.getY() - camera.y),int(entity.getFrame().w), int(entity.getFrame().h) };
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

//lazyfoo
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

	//hai hình chữ nhật không giao nhau theo chiều dọc.
	if (bottomA <= topB) {
		return false;
	}

	if (topA >= bottomB) {
		return false;
	}

	//hai hình chữ nhật không giao nhau theo chiều ngang.
	if (rightA <= leftB) {
		return false;
	}

	if (leftA >= rightB) {
		return false;
	}

	return true;
}

//ls 28
//kiểm tra xem hình chữ nhật box có chạm vào bức tường (thể hiện bởi các Tile) không.
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


//kiểm tra hcn box có va chạm vào bức tường (thể hiện bởi các Tile) không.
/*
có hai phiên bản: phiên bản đầu tiên trả về kết quả kiểm tra va chạm dưới dạng bool,
trong khi phiên bản thứ hai trả về cả trạng thái đất liền (grounded) của hộp box và vị trí của Tile đất liền đầu tiên (groundSTT) và Tile của màn chơi hiện tại (levelSTT).
*/
bool commonFunc::touchesWood(SDL_Rect& box, vector<LevelPart>& LevelPartList, bool& grounded, int& groundSTT, int& levelSTT) {
	
	/*
	mỗi LevelPart, nó sẽ kiểm tra xem hộp box có chạm vào LevelPart không 
	bằng cách so sánh tọa độ x và y của box với kích thước của LevelPart
	Nếu box chạm vào LevelPart, hàm sẽ xác định các chỉ số cột và dòng của các Tile mà hộp box đang chạm vào.
	nó sẽ kiểm tra va chạm bằng cách gọi hàm checkCollision để kiểm tra xem hộp box có va chạm với Tile tương ứng không.
	*/
	bool check = false;
	for (int i = 0; i < LevelPartList.size(); i++) {
		if (box.x + box.w + 12 >= LevelPartList.at(i).getX() && box.x <= LevelPartList.at(i).getX() + LEVEL_WIDTH && box.y >= 0 && box.y < LEVEL_HEIGHT - TILE_HEIGHT) {
			int column_left = (box.x - LevelPartList.at(i).getX()) / TILE_WIDTH;
			int column_right = column_left + 1;
			int row_up = box.y / TILE_HEIGHT;
			int row_down = row_up + 1;


			//tính toán các giá trị đại diện cho các ô vuông trong một ma trận hình chữ nhật.
			int stt1 = row_up * 21 + column_right;// 21 (số cột của ma trận)
			int stt2 = row_down * 21 + column_right;
			int stt3 = row_up * 21 + column_left;
			int stt4 = row_down * 21 + column_left;
			

			/*
			kiểm tra xem hộp box có đang đứng trên Tile đất không. 
			Nếu hộp box không đứng trên Tile đất nào, trạng thái đất liền sẽ được thiết lập là false.
			Nếu hộp box đang đứng trên Tile đất, hàm sẽ gán grounded bằng true.
			hàm sẽ lưu vị trí của Tile đất đầu tiên mà hộp box đang đứng lên đó vào groundSTT, 
			và lưu chỉ số Tile của màn chơi hiện tại vào levelSTT.
			*/

			/*
			hàm touchesWall kiểm tra xem hộp box có chạm vào LevelPartList.at(i) hay không 
			bằng cách so sánh tọa độ x của box với tọa độ x của LevelPartList.at(i) và so sánh tọa độ y của box với giới hạn của chiều cao màn chơi.
			Nếu hộp box chạm vào LevelPartList.at(i), nó tính toán các chỉ số cột và dòng tương ứng của Tile mà hộp box đang chạm vào. 
			Sau đó, nó kiểm tra va chạm bằng cách gọi hàm checkcollisionN
			*/

			//kiểm tra xem hộp có nằm trong khoảng của đối tượng i hay không bằng cách so sánh giá trị x và chiều rộng của hộp với giá trị x và chiều rộng của đối tượng i.
			if (box.x <= LevelPartList.at(i).getX() && box.x + box.w + 12 >= LevelPartList.at(i).getX() || box.x <= LevelPartList.at(i).getX() + LEVEL_WIDTH && box.x + box.w + 12 >= LevelPartList.at(i).getX() + LEVEL_WIDTH) {
				grounded = false;//Nếu không, grounded được gán bằng false, có nghĩa là hộp không được nằm trên mặt đất.
			}
			else {//có=>tiếp tục kiểm tra va chạm của hộp với các ô vuông của bản đồ
				//Các ô vuông được xác định bằng cách tính toán chỉ số (stt1, stt2, stt3 và stt4) của nó trong danh sách các ô vuông trong đối tượng i.
				//Chỉ số này được tính bằng cách nhân số hàng của ô vuông với 21 (độ rộng của mỗi hàng là 21 ô vuông) và cộng với số cột của ô vuông.


//nếu ô vuông có kiểu (getType) nằm trong khoảng từ 0 đến 84 và hộp va chạm với ô vuông đó (bằng cách sử dụng hàm checkCollision), biến check sẽ được gán bằng true, có nghĩa là hộp nằm trên mặt đất.
				if ((LevelPartList.at(i).getTilesList().at(stt1)->getType() >= 0) && (LevelPartList.at(i).getTilesList().at(stt1)->getType() <= 84))
					if (checkCollision(box, LevelPartList.at(i).getTilesList().at(stt1)->getCollision())) check = true;

				if ((LevelPartList.at(i).getTilesList().at(stt2)->getType() >= 0) && (LevelPartList.at(i).getTilesList().at(stt2)->getType() <= 84))
					if (checkCollision(box, LevelPartList.at(i).getTilesList().at(stt2)->getCollision())) check = true;


				if ((LevelPartList.at(i).getTilesList().at(stt3)->getType() >= 0) && (LevelPartList.at(i).getTilesList().at(stt3)->getType() <= 84))
					if (checkCollision(box, LevelPartList.at(i).getTilesList().at(stt3)->getCollision())) check = true;

				if ((LevelPartList.at(i).getTilesList().at(stt4)->getType() >= 0) && (LevelPartList.at(i).getTilesList().at(stt4)->getType() <= 84))
					if (checkCollision(box, LevelPartList.at(i).getTilesList().at(stt4)->getCollision())) check = true;

//Nếu ô vuông tại stt2 và stt4 có kiểu lớn hơn 84 thì grounded được gán bằng false, có nghĩa là hộp không nằm trên mặt đất
//Nếu ô vuông tại stt4 có kiểu lớn hơn 84 và ô vuông tại stt2 có kiểu từ 0 đến 84 và hộp không nằm bên phải của ô vuông tại stt2, grounded được gán bằng false.
				if ((LevelPartList.at(i).getTilesList().at(stt2)->getType() > 84) && (LevelPartList.at(i).getTilesList().at(stt4)->getType() > 84)) grounded = false;
				if ((LevelPartList.at(i).getTilesList().at(stt4)->getType() > 84) && (LevelPartList.at(i).getTilesList().at(stt2)->getType() <= 84) && box.x + box.w <= LevelPartList.at(i).getTilesList().at(stt2)->getX()) grounded = false;
			}
				
			groundSTT = stt4;
			levelSTT = i;
		}
	}
//đoạn code trên kiểm tra va chạm của hộp với bản đồ và xác định xem hộp có nằm trên mặt đất hay không.
	return check;
}