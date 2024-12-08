#include <SDL.h>
#include <SDL_image.h>
#include <cstdio>
#include <string>
#include <sstream>
#include "../headers/LWindow.h"
#include "../headers/LTexture.h"

/**Constant variables*/

//Total globally allocated windows
constexpr int TOTAL_WINDOWS = 3;

//The dimensions of the level
constexpr int LEVEL_WIDTH = 1280;
constexpr int LEVEL_HEIGHT = 960;

//Screen dimension constants
constexpr int WINDOW_WIDTH = 640;
constexpr int WINDOW_HEIGHT = 480;
constexpr int SCREEN_FPS = 60;
constexpr int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

/**Non-constant variables*/

//Display data
int gTotalDisplays = 0;
SDL_Rect* gDisplayBounds = nullptr;

/**Objects*/

//Our custom window
LWindow gWindow;

/**Function prototypes below*/

//Load prototypes
bool init();
bool loadMedia();
void close();

/**Main functions*/

//Starts up the SDL and creates window
bool init() {
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }
    else {
        //Set texture filtering to linear
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
            printf("Warning: Linear texture filtering not enabled!");
        }

        //Get number of displays
        gTotalDisplays = SDL_GetNumVideoDisplays();
        if (gTotalDisplays < 1) {
            printf("Error: No video displays found!");
            success = false;
        }
        else if (gTotalDisplays < 2) {
            printf("Warning: Only one display connect!");
        }

        //Get bounds of each display
        gDisplayBounds = new SDL_Rect[gTotalDisplays];
        for (int i = 0; i < gTotalDisplays; ++i) {
            SDL_GetDisplayBounds(i, &gDisplayBounds[i]);
        }

        //Create window
        if (!gWindow.init(WINDOW_WIDTH, WINDOW_HEIGHT)) {
            printf("Window 0 could not be created!");
            success = false;
        }
    }

    return success;
}

//Loads media
bool loadMedia() {
    //Loading success flag
    bool success = true;

    return success;
}

//Frees media and shuts down SDL
void close() {
    //Destroy windows
    gWindow.free();

    //Quit SDL subsystems
    SDL_Quit();
}

//Main loop
int main(int argc, char* args[]) {
    if (!init()) {
        printf("Failed to initialize!\n");
    }
    else {
        if (!loadMedia()) {
            printf("Failed to load media!\n");
        }
        else {
            //Main loop flag
            bool quit = false;

            //Event handler
            SDL_Event e;

            //While application is running
            while(!quit) {
                //Handle events on queue
                while(SDL_PollEvent(&e) != 0) {

                    //User requests quit
                    if(e.type == SDL_QUIT)
                        quit = true;

                    //Handle window events
                    gWindow.handleEvent(e);
                }
                //Update window
                gWindow.render();
            }
        }
    }
    //Free resources and close SDL
    close();

    return 0;
}

/**Secondary functions*/
