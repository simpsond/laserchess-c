//
// Created by Dustin Simpson on 1/18/21.
//

#ifndef LASER_CHESS_C_LASER_BEAM_H
#define LASER_CHESS_C_LASER_BEAM_H

#include "board.h"
#include "utils.h"

typedef struct StructBeam {
    Vector2 direction;
    Vector2 tile;
    int nextCount;
    struct StructBeam **next;
} Beam;

Beam* createBeam(Vector2 start, Vector2 dir);
void destroyBeam(Beam* beam);
void fireBeam(Beam* b, Board* board);


#endif //LASER_CHESS_C_LASER_BEAM_H
