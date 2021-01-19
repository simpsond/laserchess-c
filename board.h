//
// Created by Dustin Simpson on 1/18/21.
//

#ifndef LASER_CHESS_C_BOARD_H
#define LASER_CHESS_C_BOARD_H
#include <SDL.h>

typedef struct structBoard {
    int x;
    int y;
    int width;
    int height;

    int tileXCount;
    int tileYCount;

    int blockWidth;
    int blockHeight;
    int blockPadding;

    SDL_Renderer* renderer;
} Board;

#endif //LASER_CHESS_C_BOARD_H
