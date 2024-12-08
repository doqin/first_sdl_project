//
// Created by đỗ quyên on 6/12/24.
//

#ifndef LDOT_H
#define LDOT_H
#include <SDL.h>
#include "Circle.h"
#include "LParticle.h"
#include "CollisionDetection.h"
#include "DeltaTime.h"
#include "global.h"
#include "LTexture.h"

extern const int LEVEL_WIDTH;
extern const int LEVEL_HEIGHT;

inline constexpr int TOTAL_PARTICLES = 20;

//The dot that will move around on the screen
class LDot {
public:
    //The dimensions of the dot
    static constexpr int DOT_WIDTH = 20;
    static constexpr int DOT_HEIGHT = 20;

    //Maximum axis velocity of the dot
    static constexpr int DOT_VEL = 200;

    //Initializes the variables
    LDot(int x, int y);

    //Deallocate dot
    ~LDot();

    //Takes key presses and adjusts the dot's velocity
    void handleEvent(const SDL_Event &e);

    //Moves the dot
    void move(const SDL_Rect *square, const Circle *circle);

    //Load texture for dot
    bool loadTexture(SDL_Renderer *mRenderer, const std::string &path);

    //Deallocate texture
    void free();

    //Shows the dot of the screen
    void render(SDL_Renderer* mRenderer, int camX, int camY);

    //Gets collision circle
    Circle& getCollider();

    //Position accessors
    int getPosX() const;
    int getPosY() const;
private:
    //The particles
    Particle* particles[TOTAL_PARTICLES];

    //Shows the particles
    void renderParticles(SDL_Renderer *mRenderer, int camX, int camY);

    //The X and Y offsets of the dot
    float mPosX, mPosY;

    //The velocity of the dot
    float mVelX, mVelY;

    //The dot's texture
    LTexture mDotTexture;

    //Dot's collision circle
    Circle mCollider;

    //Moves the collision circle relative to the dot's offset
    void shiftColliders();
};

/*-----------*
Dot functions
-------------*/

//Initializes the variables
inline LDot::LDot(const int x, const int y) {
    //Initialize the offsets
    mPosX = x;
    mPosY = y;

    //Create the necessary SDL_Rects
    mCollider.r = DOT_WIDTH / 2;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;

    //Initialize particles
    for (int i = 0; i < TOTAL_PARTICLES; i++) {
        particles[i] = new Particle(mPosX, mPosY, 20);
    }

    //Initialize colliders relative to position
    shiftColliders();
}

inline LDot::~LDot() {
    for (int i = 0; i < TOTAL_PARTICLES; i++) {
        delete particles[i];
    }
}

//Takes key presses and adjusts the dot's velocity
inline void LDot::handleEvent(const SDL_Event &e) {
    if (e.type == SDL_JOYAXISMOTION) {
        //Motion on controller 0
        if (e.jaxis.which == 0) {

            //X-axis motion
            if (e.jaxis.axis == 0) {

                //Left of dead zone
                if (e.jaxis.value < -JOYSTICK_DEAD_ZONE) {
                    xDir = e.jaxis.value / 32767.f;
                }
                //Right of dead zone
                else if (e.jaxis.value > JOYSTICK_DEAD_ZONE) {
                    xDir = e.jaxis.value / 32767.f;
                }
                else xDir = 0;
            }
            //Y-axis motion
            if (e.jaxis.axis == 1) {

                //Left of dead zone
                if (e.jaxis.value < -JOYSTICK_DEAD_ZONE) {
                    yDir = e.jaxis.value / 32767.f;
                }

                //Right of dead zone
                else if (e.jaxis.value > JOYSTICK_DEAD_ZONE) {
                    yDir = e.jaxis.value / 32767.f;
                }
                else yDir = 0;
            }
        }
        mVelX = DOT_VEL * xDir;
        mVelY = DOT_VEL * yDir;
    }
    //If a key was pressed
    else if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
        //Adjust the velocity
        switch(e.key.keysym.sym) {
            case SDLK_UP:
                mVelY -= DOT_VEL;
            break;
            case SDLK_DOWN:
                mVelY += DOT_VEL;
            break;
            case SDLK_LEFT:
                mVelX -= DOT_VEL;
            break;
            case SDLK_RIGHT:
                mVelX += DOT_VEL;
            break;
            default:
                break;
        }
    }
    else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
        //Adjust the velocity
        switch(e.key.keysym.sym) {
            case SDLK_UP:
                mVelY += DOT_VEL;
            break;
            case SDLK_DOWN:
                mVelY -= DOT_VEL;
            break;
            case SDLK_LEFT:
                mVelX += DOT_VEL;
            break;
            case SDLK_RIGHT:
                mVelX -= DOT_VEL;
            break;
            default:
                break;
        }
    }
}

//Moves the dot
inline void LDot::move(const SDL_Rect* square = nullptr, const Circle* circle = nullptr) {

    //Delta time
    const float delta = gDeltaTime.getDeltaTime();

    //Move the dot left or right
    mPosX += mVelX * delta;
    shiftColliders();

    //If the dot went too far to the left or right
    if ((mPosX - mCollider.r < 0) || (mPosX + mCollider.r > LEVEL_WIDTH) || (square ? checkCollision(&mCollider, square) : circle ? checkCollision(&mCollider, circle) : false)) {
        //Move back
        mPosX -= mVelX * delta;
        shiftColliders();
    }

    //Move the dot up or down
    mPosY += mVelY * delta;
    shiftColliders();

    //If the dot went too far up or down
    if ((mPosY - mCollider.r < 0.f) || (mPosY + mCollider.r > LEVEL_HEIGHT) || (square ? checkCollision(&mCollider, square) : circle ? checkCollision(&mCollider, circle) : false)) {
        //Move back
        mPosY -= mVelY * delta;
        shiftColliders();
    }
}

//Load texture for dot
inline bool LDot::loadTexture(SDL_Renderer* mRenderer, const std::string &path) {
    return mDotTexture.loadFromFile(mRenderer, path);
}

//Deallocate texture
inline void LDot::free() {
    mDotTexture.free();
}

//Shows the dot of the screen
inline void LDot::render(SDL_Renderer* mRenderer, const int camX, const int camY) {
    //Show particles
    renderParticles(mRenderer, camX, camY);

    //Show the dot
    mDotTexture.render(mRenderer, mPosX - mCollider.r - camX, mPosY - mCollider.r - camY);
}

//Gets collision circle
inline Circle& LDot::getCollider() {
    return mCollider;
}

//Moves the collision circle relative to the dot's offset
inline void LDot::shiftColliders() {
    mCollider.x = mPosX;
    mCollider.y = mPosY;
}

inline int LDot::getPosX() const{
    return mPosX;
}

inline int LDot::getPosY() const {
    return mPosY;
}

inline void LDot::renderParticles(SDL_Renderer* mRenderer, const int camX, const int camY) {
    //Go through particles
    for (int i = 0; i < TOTAL_PARTICLES; ++i) {
        //Delete and replace dead particles
        if (particles[i]->isDead()) {
            delete particles[i];
            particles[i] = new Particle(mPosX, mPosY, 20);
        }
    }

    //Show particles
    for (int i = 0; i < TOTAL_PARTICLES; ++i) {
        particles[i]->render(mRenderer, camX, camY);
    }
}

/*-----*
Objects
-------*/

//The dot that will be moving around on the screen
extern LDot dot;

#endif //LDOT_H
