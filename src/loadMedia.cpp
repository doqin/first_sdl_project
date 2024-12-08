//
// Created by đỗ quyên on 6/12/24.
//
#include "../headers/global.h"
#include "../headers/AllHeaders.h"

//Loads media
bool loadMedia() {
    //Loading success flag
    bool success = true;

    SDL_Renderer *gRenderer = gWindow.getRenderer();

    //Open the font
    gFont = TTF_OpenFont("../assets/fonts/lazy.ttf", 28);
    if (gFont == nullptr) {
        printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
        success = false;
    }

    //Load dot texture
    if (!dot.loadTexture(gRenderer, "../assets/images/dot.bmp")) {
        printf("Failed to load dot.png!\n");
        success = false;
    }

    if (!gTileTexture.loadFromFile(gRenderer, "../assets/images/tiles.png")) {
        printf("Failed to load tiles.png!\n");
        success = false;
    }

    //Load tile map
    if (!setTiles(tiles, "../data/lazy.map")) {
        printf("Failed to set tiles!\n");
        success = false;
    }

    //Load shimmer texture
    if (!gShimmerTexture.loadFromFile(gRenderer, "../assets/images/shimmer.bmp")) {
        printf("Failed to load shimmer.bmp!\n");
        success = false;
    }

    //Free gRenderer
    // ReSharper disable once CppDFAUnusedValue
    gRenderer = nullptr;
    return success;
}
