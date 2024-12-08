//
// Created by đỗ quyên on 6/12/24.
//

#include "../headers/AllHeaders.h"
#include "../headers/global.h"

/*----------------*
Constant variables
------------------*/

//Screen dimension constants
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

//Button constants
const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 200;
const int TOTAL_BUTTONS = 4;

//Analog joystick dead zone
const int JOYSTICK_DEAD_ZONE = 1000;

//The dimensions of the level
const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 960;

/*--------------------*
Non-constant variables
----------------------*/

//Normalized direction
float xDir = 0;
float yDir = 0;

//The camera area
LCamera gCamera = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

//Display data
int gTotalDisplays = 0;
SDL_Rect* gDisplayBounds = nullptr;


/*------------------*
Important Components
--------------------*/

//Globally used font
TTF_Font* gFont = nullptr;

//Game controller handler with force feedback
SDL_GameController* gGameController = nullptr;

//Joystick handler with haptic
SDL_Joystick* gJoystick = nullptr;
SDL_Haptic* gJoyHaptic = nullptr;

/*-----*
Objects
-------*/

//The dot that will be moving around on the screen
LDot dot(LDot::DOT_WIDTH / 2, LDot::DOT_HEIGHT / 2);

//The window we're rendering to
LWindow gWindow;

//Text texture
LTexture gFPSTextTexture;

//RGB Particle textures
LTexture gRedTexture;
LTexture gGreenTexture;
LTexture gBlueTexture;

//Shimmer texture
LTexture gShimmerTexture;


