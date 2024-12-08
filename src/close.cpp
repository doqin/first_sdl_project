//
// Created by đỗ quyên on 6/12/24.
//

#include <SDL_image.h>
#include <SDL_mixer.h>
#include "../headers/global.h"
#include "../headers/AllHeaders.h"

void freeController() {
    if (gGameController != nullptr) {
        SDL_GameControllerClose(gGameController);
    }
    if (gJoyHaptic != nullptr) {
        SDL_HapticClose(gJoyHaptic);
    }
    if (gJoystick != nullptr) {
        SDL_JoystickClose(gJoystick);
    }
    gGameController = nullptr;
    gJoystick = nullptr;
    gJoyHaptic = nullptr;
}

//Frees media and shuts down SDL
void close() {
    //Free loaded image
    gFPSTextTexture.free();
    gRedTexture.free();
    gGreenTexture.free();
    gBlueTexture.free();
    gShimmerTexture.free();

    //Free dots
    dot.free();

    //Close game controller
    freeController();

    //Free global font
    TTF_CloseFont(gFont);
    gFont = nullptr;

    //Destroy window
    gWindow.free();

    //Quit SDL subsystems
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
