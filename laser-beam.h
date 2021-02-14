//
// Created by Dustin Simpson on 1/18/21.
//

#ifndef LASER_CHESS_C_LASER_BEAM_H
#define LASER_CHESS_C_LASER_BEAM_H

//#include "GameState.h"
#include "board.h"
#include "utils.h"

struct StructGameState;

typedef struct StructBeam {
    int id;
    Vector2 entryDirection;
    Vector2 exitDirection;
    Vector2 tile;
    struct StructBeam *prev;
    int nextCount;
    struct StructBeam **next;
} Beam;

Beam* createBeam(Vector2 start, Vector2 dir);
void destroyBeam(Beam* beam);

void fireBeam(struct StructGameState* gs, Board* board);
void splitBeam(Beam* tip, Vector2 direction);

void printBeam(Beam* beam);

#endif //LASER_CHESS_C_LASER_BEAM_H
