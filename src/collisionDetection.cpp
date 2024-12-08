//
// Created by đỗ quyên on 8/12/24.
//

#include "../headers/collisionDetection.h"
#include "../headers/LTile.h"
#include "../headers/getDistance.h"

//Circle-Circle collision detector
bool checkCollision(const SDL_Rect* a, const SDL_Rect* b) {
    //Calculate the sides of rect A
    const int leftA = a->x;
    const int rightA = a->x + a->w;
    const int topA = a->y;
    const int bottomA = a->y + a->h;

    //Calculate the sides of rect B
    const int leftB = b->x;
    const int rightB = b->x + b->w;
    const int topB = b->y;
    const int bottomB = b->y + b->h;

    //If any of the sides from A are outside of B
    if( bottomA <= topB )
    {
        return false;
    }

    if( topA >= bottomB )
    {
        return false;
    }

    if( rightA <= leftB )
    {
        return false;
    }

    if( leftA >= rightB )
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}

//Circle-Circle collision detector
bool checkCollision(const Circle* a, const Circle* b) {
    //Calculate total radius squared
    int totalRadiusSquared = a->r + b->r;
    totalRadiusSquared = totalRadiusSquared * totalRadiusSquared;

    //If the distance between the centers of the circles is less than the sum of their radii
    if (distanceSquared(a->x, a->y, b->x, b->y) < totalRadiusSquared) {
        //The circles have collided
        return true;
    }

    //If not
    return false;
}

//Circle-Box collision detector
bool checkCollision(const Circle* a, const SDL_Rect* b) {
    //Closest point on collision box
    int cX, cY;

    //Find closest x offset
    if (a->x < b->x) {
        cX = b->x;
    }
    else if (a->x > b->x + b->w) {
        cX = b->x + b->w;
    }
    else {
        cX = a->x;
    }

    //Find closest y offset
    if (a->y < b->y) {
        cY = b->y;
    }
    else if (a->y > b->y + b->h) {
        cY = b->y + b->h;
    }
    else {
        cY = a->y;
    }

    //If the closest point is inside the circle
    if (distanceSquared(a->x, a->y, cX, cY) < a->r*a->r) {
        //This box and the circle have collided
        return true;
    }

    //If the shapes have not collided
    return false;
}

bool touchesWall(const Circle* circle, LTile* tiles[]) {
    //Go through the tiles
    for (int i = 0; i < TOTAL_TILES; ++i) {
        //If the tile is a wall type tile
        if (tiles[i]->getType() >= TILE_CENTER && tiles[i]->getType() <= TILE_TOPLEFT) {
            //If the collision circle touches the wall tile
            if (checkCollision(circle, tiles[i]->getBox())) {
                return true;
            }
        }
    }

    //If no wall tiles were touched
    return false;
}