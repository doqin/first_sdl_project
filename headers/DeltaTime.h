//
// Created by đỗ quyên on 6/12/24.
//

#ifndef DELTATIME_H
#define DELTATIME_H
#include "LTimer.h"

class DeltaTime {
public:
    //Initializes variables
    DeltaTime();

    //Initializes variables with existing offset
    explicit DeltaTime(float milliseconds);

    //Update delta time
    void update();

    //Get delta time
    float getDeltaTime();

    //Renamed deallocation of memory
    void free();

private:
    LTimer mTimer;
    float mDeltaTime;
    float mLastTime;
    float mCurrentTime;
};

/*------------------*
 DeltaTime functions
 -------------------*/

inline DeltaTime::DeltaTime() {
    mDeltaTime = 0;
    mTimer.start();

    mLastTime = 0;
    mCurrentTime = 0;
}

inline DeltaTime::DeltaTime (const float milliseconds) {
    mDeltaTime = milliseconds;
    mTimer.start();

    mLastTime = milliseconds;
    mCurrentTime = 0;
}

inline void DeltaTime::update() {
    mCurrentTime = mTimer.getTicks();
    mDeltaTime = (mCurrentTime - mLastTime) / 1000.f;
    mLastTime = mCurrentTime;
}

inline float DeltaTime::getDeltaTime() {
    return mDeltaTime;
}

/*-------------------------*
 Global delta time variable
 --------------------------*/

inline DeltaTime gDeltaTime;

#endif //DELTATIME_H
