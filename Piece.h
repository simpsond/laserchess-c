//
// Created by Dustin Simpson on 1/23/21.
//

#ifndef LASER_CHESS_C_PIECE_H
#define LASER_CHESS_C_PIECE_H

#include "Player.h"
#include "utils.h"
#include "GameState.h"

typedef enum {
    TRIANGLE,
    STRAIGHT,
    DIAGONAL,
    BLOCK,
    SPLITTER,
    HYPERCUBE,
    LASER,
    KING
} pieces_t;

typedef enum {
    ABSORB,
    REFLECT,
    DESTROY
} intersectAction_t;

typedef struct StructBeamIntersect {
    intersectAction_t beamWill;
    int reflectionAngleCount;
    float reflectionAngles;
} BeamIntersect;

typedef struct StructPiece {
    Player* player;
    Vector2 location;
    int rotation;
    pieces_t type;
    bool isActive;
    bool markedDestroy; // Flag to indicate piece is in the process of being destroyed.
    BeamIntersect beamIntersectProc[4];
} Piece;

void rotatePiece(GameState* gs);
void commitPieceRotation(GameState* gs);
void cancelPieceRotation(GameState* gs);

void attachDefaultBeamIntersects(Piece* piece);
void processBeamPieceCollision(GameState* gs, Beam* tip, Piece* hitPiece);
BeamIntersect* getBeamIntersect(Beam* tip, Piece* hitPiece);

#endif //LASER_CHESS_C_PIECE_H
