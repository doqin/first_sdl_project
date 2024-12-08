//
// Created by đỗ quyên on 8/12/24.
//
#include <SDL.h>
#include "../headers/global.h"
#include "../headers/AllHeaders.h"
#include <string>
#include <sstream>

void render() {
    SDL_Renderer* gRenderer = gWindow.getRenderer();
    //Calculate and correct fps
    float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
    if (avgFPS > 2000000) {
        avgFPS = 0;
    }

    //Set text to be rendered
    fpsText.str("");
    fpsText << static_cast<int>(avgFPS);

    //Update delta time
    gDeltaTime.update();

    //Move the dot
    dot.move(tiles);
    dot.setCamera(gCamera);

    //Clear screen
    gWindow.clear();

    //Render text
    if (!gFPSTextTexture.loadFromRenderedText(gRenderer, gFont, fpsText.str(), textColor)) {
        printf("Unable to render text!\n");
    }

    //Render tiles
    for (int i = 0; i < TOTAL_TILES; ++i) {
        tiles[i]->render(gRenderer, gCamera);
    }

    //Render textures
    dot.render(gRenderer, gCamera);
    gFPSTextTexture.render(gRenderer, 0, 0);

    //Update screen
    gWindow.render();
    ++countedFrames;

    //If frame finished early
    if (const int frameTicks = capTimer.getTicks(); frameTicks < SCREEN_TICKS_PER_FRAME) {
        //Wait remaining time
        SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
    }
}