//
// Created by đỗ quyên on 6/12/24.
//
#include <SDL.h>
#include "../headers/AllHeaders.h"
#include "../headers/global.h"

extern float xDir;
extern float yDir;
extern SDL_Rect camera;

// ReSharper disable once CppPassValueParameterByConstReference
void eventHandler(bool &quit, SDL_Event e) {

    //User requests quit
    if(e.type == SDL_QUIT) quit = true;
    //Handle key press
    else if(e.type == SDL_KEYDOWN) {
        switch(e.key.keysym.sym) {
            case SDLK_ESCAPE:
                quit = true;
                break;
            default:
                break;
        }
    }
    else {
        if (e.type == SDL_JOYBUTTONDOWN) {
            //Use game controller
            if (gGameController != nullptr) {
                //Play rumble at 75% strength for 500 milliseconds
                if (SDL_GameControllerRumble(gGameController, 0xFFFF * 3 / 4, 0xFFFF * 3 / 4, 500) != 0) {
                    printf("Warning: Unable to play game controller rumble! %s\n", SDL_GetError());
                }
            }
            //Use haptics
            else if (gJoyHaptic != nullptr) {
                //Play rumble at 75% strength for 500 milliseconds
                if (SDL_HapticRumblePlay(gJoyHaptic, 0.75, 500) != 0) {
                    printf("Warning: Unable to play haptic rumble! %s\n", SDL_GetError());
                }
            }
        }
    }

    //Handle window events
    gWindow.handleEvent(e);

    //Update camera width and height
    gCamera.updateSize(gWindow.getWidth(), gWindow.getHeight());

    //Handle input for the dot
    dot.handleEvent(e);
}
