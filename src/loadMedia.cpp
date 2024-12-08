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
        printf("Failed to load dot.png!");
        success = false;
    }

    //Load red texture
    if (!gRedTexture.loadFromFile(gRenderer, "../assets/images/red.bmp")) {
        printf("Failed to load red.bmp!");
        success = false;
    }

    //Load green texture
    if (!gGreenTexture.loadFromFile(gRenderer, "../assets/images/green.bmp")) {
        printf("Failed to load green.bmp!");
        success = false;
    }

    //Load blue texture
    if (!gBlueTexture.loadFromFile(gRenderer, "../assets/images/blue.bmp")) {
        printf("Failed to load blue.bmp!");
        success = false;
    }

    //Load shimmer texture
    if (!gShimmerTexture.loadFromFile(gRenderer, "../assets/images/shimmer.bmp")) {
        printf("Failed to load shimmer.bmp!");
        success = false;
    }

    //Set texture transparency
    gRedTexture.setAlpha(192);
    gGreenTexture.setAlpha(192);
    gBlueTexture.setAlpha(192);
    gShimmerTexture.setAlpha(192);

    //Free gRenderer
    gRenderer = nullptr;
    return success;
}
