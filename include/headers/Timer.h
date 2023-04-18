#pragma once
#include <iostream>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <sstream>//cung cấp các phương thức để đọc và ghi các giá trị vào hoặc ra một chuỗi ký tự (string) dưới dạng các đối tượng

#include "RenderWindow.h"
#include "Entity.h"
using namespace std;
//lesson 22
class LTimer
{
public:
	//Initializes variables
	LTimer();

	//The various clock actions
	void start();
	void stop();
	void pause();
	void unpause();

	//Gets the timer's time: để tính toán thời gian giữa các frame hoặc đo lường thời gian xử lý của một hoạt động nhất định trong trò chơi hoặc ứng dụng.
	Uint32 getTicks();//trả về số mili giây đã trôi qua từ lần gọi hàm start() của đối tượng LTimer.

	//Checks the status of the timer
	bool isStarted();
	bool isPaused();

private:
	//The clock time when the timer started
	Uint32 mStartTicks;

	//The ticks stored when the timer was paused
	Uint32 mPausedTicks;

	//trạng thái của đồng hồ lúc tạm dừng và bắt đầu
	bool mStarted;
	bool mPaused;
};