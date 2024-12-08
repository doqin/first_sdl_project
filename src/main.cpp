#include <SDL.h>
#include <cstdio>
#include <string>
#include <sstream>
#include "../headers/AllHeaders.h"
#include "../headers/global.h"

/*--------------------*
Non-constant variables
----------------------*/

extern float xDir;
extern float yDir;

/*------------*
Main functions
--------------*/

//Starts up the SDL and creates window -> init.cpp
bool init();

//Loads media -> loadMedia.cpp
bool loadMedia();

//Frees media and shuts down SDL -> close.cpp
void close();

//Handles event queues -> eventHandler.cpp
void eventHandler(bool &quit, SDL_Event e);

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

            //The set wall
            SDL_Rect wall;
            wall.x = 300;
            wall.y = 40;
            wall.w = 40;
            wall.h = 400;
            SDL_Rect relativeWall;
            relativeWall.x = wall.x;
            relativeWall.y = wall.y;
            relativeWall.w = wall.w;
            relativeWall.h = wall.h;

            //Set text color as black
            constexpr SDL_Color textColor = {0, 0, 0, 0xFF};

            //The frames per second timer
            LTimer fpsTimer;

            //The frames per second cap timer
            LTimer capTimer;

            //In memory text stream
            std::stringstream fpsText;

            //Start counting frames per second
            int countedFrames = 0;
            fpsTimer.start();

            //gWindow's renderer
            SDL_Renderer* gRenderer = gWindow.getRenderer();

            //While application is running
            while(!quit) {
                //Start cap timer
                capTimer.start();

                //Handle events on queue
                while(SDL_PollEvent(&e) != 0) {
                    eventHandler(quit, e);
                }

                // Calculate angle
                // ReSharper disable once CppDFAUnusedValue
                // ReSharper disable once CppEntityAssignedButNoRead
                // ReSharper disable once CppDFAUnreadVariable
                double joystickAngle = atan2(static_cast<double>(yDir), static_cast<double>(xDir)) * (180.0 / M_PI);

                //Correct angle
                if (xDir == 0 && yDir == 0) {
                    joystickAngle = 0;
                }

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
                dot.move(&wall);

                //Update camera position
                gCamera.updatePosition(dot.getPosX(), dot.getPosY());

                //Clear screen
                gWindow.clear();

                //Render text
                if (!gFPSTextTexture.loadFromRenderedText(gRenderer, gFont, fpsText.str(), textColor)) {
                    printf("Unable to render text!");
                }

                //Get camera position
                const int cameraPosX = gCamera.getX();
                const int cameraPosY = gCamera.getY();

                //Render wall
                SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
                relativeWall.x = wall.x - cameraPosX;
                relativeWall.y = wall.y - cameraPosY;
                SDL_RenderDrawRect(gRenderer, &relativeWall);

                //Render textures
                dot.render(gRenderer, cameraPosX, cameraPosY);
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
            // ReSharper disable once CppDFAUnusedValue
            gRenderer = nullptr;
        }
    }

    //Free resources and close SDL
    close();

    return 0;
}
