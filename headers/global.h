//
// Created by đỗ quyên on 6/12/24.
//

#ifndef GLOBAL_H
#define GLOBAL_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <sstream>

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

//Tile constants
extern const int TILE_WIDTH;
extern const int TILE_HEIGHT;
extern const int TOTAL_TILES;
extern const int TOTAL_TILE_SPRITES;

//The different tile sprites
enum TILESPRITES {
    TILE_RED,
    TILE_GREEN,
    TILE_BLUE,
    TILE_CENTER,
    TILE_TOP,
    TILE_TOPRIGHT,
    TILE_RIGHT,
    TILE_BOTTOMRIGHT,
    TILE_BOTTOM,
    TILE_BOTTOMLEFT,
    TILE_LEFT,
    TILE_TOPLEFT,
};
/*--------------------*
Non-constant variables
----------------------*/

//Normalized direction
extern float xDir;
extern float yDir;

//Display data
extern int gTotalDisplays;
extern SDL_Rect* gDisplayBounds;

//Tile clips
extern SDL_Rect gTileClips[];

//Set text color as white
extern const SDL_Color textColor;

//Frames elapsed
extern int countedFrames;

//Main loop flag
extern bool quit;

//Event handler
extern SDL_Event e;

//In memory text stream
extern std::stringstream fpsText;


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
