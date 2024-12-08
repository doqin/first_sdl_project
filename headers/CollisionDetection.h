//
// Created by đỗ quyên on 6/12/24.
//

#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H
#include <SDL_rect.h>

#include "Circle.h"
#include "GetDistance.h"

//Circle-Circle collision detector
inline bool checkCollision(const Circle* a, const Circle* b) {
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
inline bool checkCollision(const Circle* a, const SDL_Rect* b) {
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


#endif //COLLISIONDETECTION_H
