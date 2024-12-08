//
// Created by đỗ quyên on 6/12/24.
//

#ifndef GETDISTANCE_H
#define GETDISTANCE_H

//Calculate distance between centres of two circles
inline double distanceSquared(const int x1, const int y1, const int x2, const int y2) {
    const int deltaX = x2 - x1;
    const int deltaY = y2 - y1;
    return deltaX*deltaX + deltaY*deltaY;
}

#endif //GETDISTANCE_H
