//
// Created by Dustin Simpson on 1/9/21.
//
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

Vector2 rotate(Vector2 point, Vector2 origin, int degrees) {
    if(degrees == 0) {
        return point;
    }

    float s = sin(degToRad(degrees));
    float c = cos(degToRad(degrees));

    point.x -= origin.x;
    point.y -= origin.y;

    float xnew = point.x * c - point.y * s;
    float ynew = point.x * s + point.y * c;

    point.x = xnew + origin.x;
    point.y = ynew + origin.y;

    return point;
}

void rotatePoints(Vector2* points, Vector2 origin, int size, int degrees) {
    if(degrees == 0) {
        return;
    }

    for(int i = 0; i < size; i++) {
        points[i] = rotate(points[i],origin, degrees);
    }
}

void getBoxRefPoints(Vector2 p1, Vector2 p2, float margin, Vector2* refPoints, int size) {
    if(size < 9) {
        printf("No room for getBoxRefPoints\n");
        exit(1);
    }
    float xCenter = (p2.x - p1.x) / 2 + p1.x;
    float yCenter = (p2.y - p1.y) / 2 + p1.y;

    refPoints[0] = (Vector2){p1.x + margin, p1.y + margin};
    refPoints[1] = (Vector2){xCenter, p1.y + margin};
    refPoints[2] = (Vector2){p2.x - margin, p1.y + margin};
    refPoints[3] = (Vector2){p2.x - margin, yCenter};
    refPoints[4] = (Vector2){p2.x - margin, p2.y - margin};
    refPoints[5] = (Vector2){xCenter, p2.y - margin};
    refPoints[6] = (Vector2){p1.x + margin, p2.y - margin};
    refPoints[7] = (Vector2){p1.x + margin, yCenter};
    refPoints[8] = (Vector2){xCenter, yCenter};
}

Vector2 addVector2(Vector2 a, Vector2 b) {
    return (Vector2){a.x + b.x, a.y + b.y};
}

Vector2 rotateDegCW(Vector2 vec, float angle) {
    Vector2 returnVector;
    float radAngle = angle * M_PI / 180;
    float cs = cos(radAngle);
    float sn = sin(radAngle);

    returnVector.x = vec.x * cs + vec.y*sn;
    returnVector.y = vec.x * -sn + vec.y * cs;

    return returnVector;
}

SDL_Color adjustColorBrightness(struct SDL_Color color, float scale) {
    struct SDL_Color diff;


    if(scale > 0) {
        color.r = color.r + (scale * (255 - color.r));
        color.g = color.g + (scale * (255 - color.g));
        color.b = color.b + (scale * (255 - color.b));
    }

    if(scale < 0) {
        color.r = color.r + (scale * color.r);
        color.g = color.g + (scale * color.g);
        color.b = color.b + (scale * color.b);
    }

    return color;
}
