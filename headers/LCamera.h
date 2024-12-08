//
// Created by đỗ quyên on 8/12/24.
//

#ifndef LCAMERA_H
#define LCAMERA_H
#include <SDL_rect.h>

extern const int LEVEL_WIDTH;
extern const int LEVEL_HEIGHT;

class LCamera {
    public:
        //Initialize variables
        LCamera(int x, int y, int width, int height);

        //Update position
        void updatePosition(int playerPosX, int playerPosY);

        //Update size
        void updateSize(int cameraWidth, int cameraHeight);

        //Get camera properties
        int getX();
        int getY();
        int getWidth();
        int getHeight();
        SDL_Rect getRect();

    private:
        SDL_Rect mCamera;
};

inline LCamera::LCamera(const int x, const int y, const int width, const int height) {
    mCamera.x = x;
    mCamera.y = y;
    mCamera.w = width;
    mCamera.h = height;
}

inline void LCamera::updatePosition(const int playerPosX, const int playerPosY) {
    mCamera.x = playerPosX - mCamera.w / 2;
    mCamera.y = playerPosY - mCamera.h / 2;

    //Keep the camera in bounds
    if (mCamera.x < 0) {
        mCamera.x = 0;
    }
    if (mCamera.y < 0) {
        mCamera.y = 0;
    }
    if (mCamera.x > LEVEL_WIDTH - mCamera.w) {
        mCamera.x = LEVEL_WIDTH - mCamera.w;
    }
    if (mCamera.y > LEVEL_HEIGHT - mCamera.h) {
        mCamera.y = LEVEL_HEIGHT - mCamera.h;
    }
}

inline void LCamera::updateSize(const int cameraWidth, const int cameraHeight) {
    mCamera.w = cameraWidth;
    mCamera.h = cameraHeight;
}

inline int LCamera::getX() {
    return mCamera.x;
}

inline int LCamera::getY() {
    return mCamera.y;
}

inline int LCamera::getWidth() {
    return mCamera.w;
}

inline int LCamera::getHeight() {
    return mCamera.h;
}

inline SDL_Rect LCamera::getRect() {
    return mCamera;
}

/*-----*
Objects
-------*/

//The camera area
extern LCamera gCamera;

#endif //LCAMERA_H
