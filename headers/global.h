//
// Created by đỗ quyên on 6/12/24.
//

#ifndef GLOBAL_H
#define GLOBAL_H

#include <SDL.h>
#include <SDL_ttf.h>

/*----------------*
Constant variables
------------------*/

//Screen dimension constants
extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;
extern const int SCREEN_FPS;
extern const int SCREEN_TICKS_PER_FRAME;

//Button constants
extern const int BUTTON_WIDTH;
extern const int BUTTON_HEIGHT;
extern const int TOTAL_BUTTONS;

//Analog joystick dead zone
extern const int JOYSTICK_DEAD_ZONE;

//The dimensions of the level
extern const int LEVEL_WIDTH;
extern const int LEVEL_HEIGHT;

/*--------------------*
Non-constant variables
----------------------*/

//Normalized direction
extern float xDir;
extern float yDir;

//Display data
extern int gTotalDisplays;
extern SDL_Rect* gDisplayBounds;

/*------------------*
Important Components
--------------------*/

//Globally used font
extern TTF_Font* gFont;

//Game controller handler with force feedback
extern SDL_GameController* gGameController;

//Joystick handler with haptic
extern SDL_Joystick* gJoystick;
extern SDL_Haptic* gJoyHaptic;

#endif //GLOBAL_H
