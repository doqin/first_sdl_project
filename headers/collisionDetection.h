//
// Created by đỗ quyên on 6/12/24.
//

#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H
#include "Circle.h"
#include "global.h"

class LTile;

//Circle-Circle collision detector
bool checkCollision(const SDL_Rect* a, const SDL_Rect* b);

//Circle-Circle collision detector
bool checkCollision(const Circle* a, const Circle* b);

//Circle-Box collision detector
bool checkCollision(const Circle* a, const SDL_Rect* b);

//Circle-Tile collision detector
bool touchesWall(const Circle* circle, LTile* tiles[]);

#endif //COLLISIONDETECTION_H
