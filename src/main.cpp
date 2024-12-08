#include <SDL.h>
#include <cstdio>
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

//Render function -> render.cpp
void render();

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
            fpsTimer.start();
            //While application is running
            while(!quit) {
                //Start cap timer
                capTimer.start();

                //Handle events on queue
                while(SDL_PollEvent(&e) != 0) {
                    eventHandler(quit, e);
                }

                //Render to screen
                render();
            }
        }
    }

    //Free resources and close SDL
    close();

    return 0;
}
