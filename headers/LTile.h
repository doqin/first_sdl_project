//
// Created by đỗ quyên on 8/12/24.
//

#ifndef LTILE_H
#define LTILE_H

#include <SDL.h>
#include <fstream>
#include "Circle.h"
#include "global.h"
#include "LCamera.h"
#include "LTexture.h"

enum TILESPRITES;

class LTile {
public:
    //Initializes position and type
    LTile(int x, int y, int tileType);

    //Shows the tile
    void render(SDL_Renderer* mRenderer, LCamera& camera);

    //Get the tile type
    int getType();

    //Get the collision box
    SDL_Rect* getBox();
private:
    //The attributes of the tile
    SDL_Rect mBox;

    //The tile type
    int mType;
};

//Forward declaration
bool touchesWall(const Circle* circle, LTile* tiles[]);
bool checkCollision(const SDL_Rect* a, const SDL_Rect* b);

inline LTile::LTile(const int x, const int y, const int tileType) {
    //Get the offsets
    mBox.x = x;
    mBox.y = y;

    //Set the collision box
    mBox.w = TILE_WIDTH;
    mBox.h = TILE_HEIGHT;

    //Get the tile type
    mType = tileType;
}

inline void LTile::render(SDL_Renderer* mRenderer, LCamera& camera) {
    //If the tile is on screen
    if (checkCollision(camera.getRect(), &mBox)) {
        //Show the tile
        gTileTexture.render(mRenderer, mBox.x - camera.getX(), mBox.y - camera.getY(), &gTileClips[mType]);
    }
}

inline int LTile::getType() {
    return mType;
}

inline SDL_Rect* LTile::getBox() {
    return &mBox;
}

inline bool setTiles(LTile* tiles[], const std::string &path) {
    //Success flag
    bool tilesLoaded = true;

    //Open the map
    std::ifstream map(path.c_str());

    //If the map couldn't be loaded
    if (map.fail()) {
        printf("Unable to load map file!\n");
        tilesLoaded = false;
    }
    else {
        //Tile offsets
        int x = 0, y = 0;

        //Initialize the tiles
        for (int i = 0; i < TOTAL_TILES; ++i) {
            //Determines what kind of tile will be made
            int tileType = -1;

            //Read tile from map file
            map >> tileType;

            //If there was a problem in read the map
            if (map.fail()) {
                //Stop loading map
                printf("Unable to load map file!\n");
                tilesLoaded = false;
                break;
            }

            //If the number is a valid tile number
            if ((tileType >= 0) && (tileType < TOTAL_TILE_SPRITES)) {
                tiles[i] = new LTile(x, y, tileType);
            }
            //If we don't recognize the tile type
            else {
                //Stop loading map
                printf("Error loading map: Invalid tile at %d!\n", i);
                tilesLoaded = false;
                break;
            }

            //Move to next tile spot
            x += TILE_WIDTH;

            //If we've gone too far
            if (x >= LEVEL_WIDTH) {
                //Move back
                x = 0;

                //Move to the next row
                y += TILE_HEIGHT;
            }
        }

        //Clip the sprite sheet
        if (tilesLoaded) {
            gTileClips[ TILE_RED ].x = 0;
            gTileClips[ TILE_RED ].y = 0;
            gTileClips[ TILE_RED ].w = TILE_WIDTH;
            gTileClips[ TILE_RED ].h = TILE_HEIGHT;
            gTileClips[ TILE_GREEN ].x = 0;
            gTileClips[ TILE_GREEN ].y = 80;
            gTileClips[ TILE_GREEN ].w = TILE_WIDTH;
            gTileClips[ TILE_GREEN ].h = TILE_HEIGHT;
            gTileClips[ TILE_BLUE ].x = 0;
            gTileClips[ TILE_BLUE ].y = 160;
            gTileClips[ TILE_BLUE ].w = TILE_WIDTH;
            gTileClips[ TILE_BLUE ].h = TILE_HEIGHT;
            gTileClips[ TILE_TOPLEFT ].x = 80;
            gTileClips[ TILE_TOPLEFT ].y = 0;
            gTileClips[ TILE_TOPLEFT ].w = TILE_WIDTH;
            gTileClips[ TILE_TOPLEFT ].h = TILE_HEIGHT;
            gTileClips[ TILE_LEFT ].x = 80;
            gTileClips[ TILE_LEFT ].y = 80;
            gTileClips[ TILE_LEFT ].w = TILE_WIDTH;
            gTileClips[ TILE_LEFT ].h = TILE_HEIGHT;
            gTileClips[ TILE_BOTTOMLEFT ].x = 80;
            gTileClips[ TILE_BOTTOMLEFT ].y = 160;
            gTileClips[ TILE_BOTTOMLEFT ].w = TILE_WIDTH;
            gTileClips[ TILE_BOTTOMLEFT ].h = TILE_HEIGHT;
            gTileClips[ TILE_TOP ].x = 160;
            gTileClips[ TILE_TOP ].y = 0;
            gTileClips[ TILE_TOP ].w = TILE_WIDTH;
            gTileClips[ TILE_TOP ].h = TILE_HEIGHT;
            gTileClips[ TILE_CENTER ].x = 160;
            gTileClips[ TILE_CENTER ].y = 80;
            gTileClips[ TILE_CENTER ].w = TILE_WIDTH;
            gTileClips[ TILE_CENTER ].h = TILE_HEIGHT;
            gTileClips[ TILE_BOTTOM ].x = 160;
            gTileClips[ TILE_BOTTOM ].y = 160;
            gTileClips[ TILE_BOTTOM ].w = TILE_WIDTH;
            gTileClips[ TILE_BOTTOM ].h = TILE_HEIGHT;
            gTileClips[ TILE_TOPRIGHT ].x = 240;
            gTileClips[ TILE_TOPRIGHT ].y = 0;
            gTileClips[ TILE_TOPRIGHT ].w = TILE_WIDTH;
            gTileClips[ TILE_TOPRIGHT ].h = TILE_HEIGHT;
            gTileClips[ TILE_RIGHT ].x = 240;
            gTileClips[ TILE_RIGHT ].y = 80;
            gTileClips[ TILE_RIGHT ].w = TILE_WIDTH;
            gTileClips[ TILE_RIGHT ].h = TILE_HEIGHT;
            gTileClips[ TILE_BOTTOMRIGHT ].x = 240;
            gTileClips[ TILE_BOTTOMRIGHT ].y = 160;
            gTileClips[ TILE_BOTTOMRIGHT ].w = TILE_WIDTH;
            gTileClips[ TILE_BOTTOMRIGHT ].h = TILE_HEIGHT;
        }
    }

    //Close the file
    map.close();

    //If the map was loaded fine
    return tilesLoaded;
}
/*-----*
Objects
-------*/

extern LTile* tiles[];

#endif //LTILE_H
