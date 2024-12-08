//
// Created by đỗ quyên on 7/12/24.
//

#ifndef LPARTICLE_H
#define LPARTICLE_H
#include <iostream>

#include "LTexture.h"

//Particle engine
class Particle {
public:
    //Initialize position and animation
    Particle(int x, int y, int duration);

    //Shows the particle
    void render(SDL_Renderer *mRenderer, int camX, int camY);

    //Checks if particle is dead
    bool isDead();

private:
    //Offsets
    int mPosX, mPosY;

    //Current frame of animation
    int mFrame;

    //Animation duration
    int mDuration;

    //Type of particle
    LTexture* mTexture;
};

inline Particle::Particle(const int x, const int y, const int duration) {
    //Set offsets
    mPosX = x - 8 + (rand() % 13);
    mPosY = y - 8 + (rand() % 13);

    //Initialize animation
    mFrame = rand() % 10;

    //Set animation duration
    mDuration = duration;

    //Set type
    switch (rand() % 3) {
        case 0: mTexture = &gRedTexture; break;
        case 1: mTexture = &gGreenTexture; break;
        case 2: mTexture = &gBlueTexture; break;
    }
}

inline void Particle::render(SDL_Renderer* mRenderer, const int camX, const int camY) {
    //Show image
    mTexture->renderOnce(mRenderer, static_cast<Uint8>(192.0 * (1.0 - static_cast<float>(mFrame) / mDuration)), mPosX - camX, mPosY - camY);

    //Show shimmer
    if (mFrame % 10 == 0) {
        gShimmerTexture.render(mRenderer, mPosX - camX, mPosY - camY);
    }

    //Animate
    mFrame++;
}

inline bool Particle::isDead() {
    return mFrame > mDuration;
}

#endif //LPARTICLE_H
