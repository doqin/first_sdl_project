//
// Created by đỗ quyên on 6/12/24.
//

#ifndef LTIMER_H
#define LTIMER_H
#include <SDL.h>

//The application time based timer
class LTimer {
public:
    //Initializes variables
    LTimer();

    //The various clock actions
    void start();

    void stop();

    void pause();

    void unpause();

    //Gets the timer's time
    Uint32 getTicks() const;

    //Checks the status of the timer
    bool isStarted() const;

    bool isPaused() const;

private:
    //The clock time when the timer started
    Uint32 mStartTicks;

    //The ticks stored when the timer was paused
    Uint32 mPausedTicks;

    //The timer status
    bool mPaused;
    bool mStarted;
};

/*--------------*
LTimer functions
----------------*/

//Initializes variables
inline LTimer::LTimer() {
    mStartTicks = 0;
    mPausedTicks = 0;

    mPaused = false;
    mStarted = false;
}

//The various clock actions
inline void LTimer::start() {
    //Start the timer
    mStarted = true;

    //Unpause the timer
    mPaused = false;

    //Get the current clock time
    mStartTicks = SDL_GetTicks();
    mPausedTicks = 0;
}

inline void LTimer::stop() {
    //Stop the timer
    mStarted = false;

    //Unpause the timer
    mPaused = false;

    //Clear tick variables
    mStartTicks = 0;
    mPausedTicks = 0;
}

inline void LTimer::pause() {
    //If the timer is running and isn't already paused
    if (mStarted && !mPaused) {
        //Pause the timer
        mPaused = true;

        //Calculate the paused ticks
        mPausedTicks = SDL_GetTicks() - mStartTicks;
        mStartTicks = 0;
    }
}

inline void LTimer::unpause() {
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

//Gets the timer's time
inline Uint32 LTimer::getTicks() const {
    //The actual timer time
    Uint32 time = 0;

    //If the timer is running
    if (mStarted) {
        //If the timer is paused
        if (mPaused) {
            //Return the number of ticks when the timer was paused
            time = mPausedTicks;
        }
        else {
            //Return the current time minus the start time
            time = SDL_GetTicks() - mStartTicks;
        }
    }
    return time;
}

//Checks the status of the timer
inline bool LTimer::isStarted() const {
    //Timer is running and paused or unpaused
    return mStarted;
}

inline bool LTimer::isPaused() const {
    //Timer is running and paused
    return mPaused && mStarted;
}

#endif //LTIMER_H
