//
// Created by đỗ quyên on 6/12/24.
//

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include "../headers/global.h"
#include "../headers/AllHeaders.h"

//Get controller(s)
void getController() {
    //Check for joystick
    if (SDL_NumJoysticks() < 1) {
        printf("Warning: No joysticks connected!\n");
    }
    else {
        //Check if joystick is game controller interface compatible
        if (!SDL_IsGameController(0)) {
            printf("Warning: Joystick is not game controller interface compatible! SDL_Error: %s\n", SDL_GetError());
        }
        else {
            //Open game controller and check if it supports rumble
            gGameController = SDL_GameControllerOpen(0);
            if (!SDL_GameControllerHasRumble(gGameController)) {
                printf("Warning: Game controller does not have rumble! SDL Error %s\n", SDL_GetError());
            }
        }
        if (gGameController == nullptr) {
            //Open first joystick
            gJoystick = SDL_JoystickOpen(0);
            if (gJoystick == nullptr) {
                printf("Warning: Unable to open joystick! SDL Error: %s\n", SDL_GetError());
            }
            else {
                //Check if joystick supports haptic
                if (!SDL_JoystickIsHaptic(gJoystick)) {
                    printf("Warning: Unable to open joystick! SDL_Error: %s\n", SDL_GetError());
                }
                else {
                    //Get joystick haptic device
                    gJoyHaptic = SDL_HapticOpenFromJoystick(gJoystick);
                    if (gJoyHaptic == nullptr) {
                        printf("Warning: Unable to get joystick haptics! SDL Error: %s\n", SDL_GetError());
                    }
                    else {
                        //Initialize rumble
                        if (SDL_HapticRumbleInit(gJoyHaptic) < 0) {
                            printf("Warning: Unable to initialize haptic rumble! SDL Error: %s\n", SDL_GetError());
                        }
                    }
                }
            }
        }
    }
}

//Starts up the SDL and creates window
bool init() {
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_GAMECONTROLLER) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }
    else {
        //Set texture filtering to linear
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
            printf("Warning: Linear texture filtering not enabled!");
        }
        //Get controller(s)
        getController();

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
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            success = false;
        }
        else {
            //Initialize PNG loading
            if (constexpr int imgFlags = IMG_INIT_PNG; !(IMG_Init(imgFlags) & imgFlags)) {
                printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                success = false;
            }

            //Initialize SDL_mixer
            if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
                printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
                success = false;
            }

            //Initialize SDL_ttf
            if (TTF_Init() == -1) {
                printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
                success = false;
            }
        }
    }

    return success;
}

