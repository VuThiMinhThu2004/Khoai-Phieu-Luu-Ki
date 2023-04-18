#include "RenderWindow.h"
#include "Entity.h"
#include "Timer.h"
using namespace std;
//ls 23
LTimer::LTimer() {
    //Initialize the variables
    mStartTicks = 0;
    mPausedTicks = 0;

    mPaused = false;
    mStarted = false;
}

//Our constructor initializes the internal data members.

void LTimer::start() {
    //Start the timer
    if (!mStarted) {
        mStarted = true;

        //Unpause the timer
        mPaused = false;

        //Get the current clock time
        mStartTicks = SDL_GetTicks();
        mPausedTicks = 0;
    }
}

//The stop function basically reinitializes all the variables.
void LTimer::stop() { //dừng và thiết lập lại các biến thành viên của Timer
    //Stop the timer
    mStarted = false;

    //Unpause the timer
    mPaused = false;

    //Clear tick variables
    mStartTicks = 0;
    mPausedTicks = 0;
}

void LTimer::pause() {//tạm dùng và lưu lại thời gian đã tích lũy
    //If the timer is running and isn't already paused
    if (mStarted && !mPaused) {
        //Pause the timer
        mPaused = true;

        //Calculate the paused ticks
        mPausedTicks = SDL_GetTicks() - mStartTicks;
        mStartTicks = 0;
    }
}

void LTimer::unpause() {
    //If the timer is running and paused
    if (mStarted && mPaused) {
        //Unpause the timer
        mPaused = false;

        //Reset the starting ticks
        mStartTicks = SDL_GetTicks() - mPausedTicks;

        //Reset the paused ticks
        mPausedTicks = 0;
    }
}

Uint32 LTimer::getTicks() {// trả về thời gian đã tích lũy
    //The actual timer time
    Uint32 time = 0;

    //If the timer is running
    if (mStarted) {
        //If the timer is paused
        if (mPaused) {
            //Return the number of ticks when the timer was paused
            time = mPausedTicks;//thời gian được lưu trữ khi dừng lại
        }
        else {
            //Return the current time minus the start time
            time = SDL_GetTicks() - mStartTicks;//trả lại thời gian so với khi nó bắt đầu
        }
    }

    return time;
}

bool LTimer::isStarted() {
    //Timer is running and paused or unpaused
    return mStarted;
}

bool LTimer::isPaused() {
    //Timer is running and paused
    return mPaused && mStarted;
}