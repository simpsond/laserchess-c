//
// Created by Dustin Simpson on 1/8/21.
//
//

#include <math.h>
#include <stdbool.h>
#include <SDL.h>

#define degToRad(angleInDegrees) ((angleInDegrees) * M_PI / 180.0)
#define radToDeg(angleInRadians) ((angleInRadians) * 180.0 / M_PI)

#ifndef LASER_CHESS_C_UTILS_H
#define LASER_CHESS_C_UTILS_H

typedef struct structVector2 {
    float x;
    float y;
} Vector2;

typedef struct structVector3 {
    float x;
    float y;
    float z;
} Vector3;



Vector2 rotate(Vector2 point, Vector2 origin, int degrees);
void rotatePoints(Vector2* points, Vector2 origin, int size, int degrees);
void getBoxRefPoints(Vector2 p1, Vector2 p2, float margin, Vector2* refPoints, int size);

Vector2 addVector2(Vector2 a, Vector2 b);
Vector2 rotateDegCW(Vector2 vec, float angle); // Rotation given in clockwise degrees
float calcAngleVector2(Vector2 reference_vec, Vector2 vec);

Vector2 roundVector2(Vector2 v);

SDL_Color adjustColorBrightness(struct SDL_Color color, float scale);

#endif //LASER_CHESS_C_UTILS_H
