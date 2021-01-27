//
// Created by Dustin Simpson on 1/23/21.
//

#include <stdbool.h>
#include "Piece.h"
#include "laser-chess.h"

void rotatePiece(GameState* gs) {
    if(gs->isPieceSelected) {
        if(!gs->isPieceRotating) {
            gs->isPieceRotating = true;
            gs->startingRotation = gs->selectedPiece->rotation;
        }
        gs->selectedPiece->rotation += 90;
    }
}

void commitPieceRotation(GameState* gs) {
    gs->isPieceRotating = false;
    gs->startingRotation = 0;
    endMove(gs);
}

void cancelPieceRotation(GameState* gs) {
    if(gs->isPieceRotating) {
        gs->isPieceRotating = false;
        gs->selectedPiece->rotation = gs->startingRotation;
        gs->startingRotation = 0;
    }
}

void processBeamPieceCollision(GameState* gs, Beam* tip, Piece* hitPiece) {
//    Vector2 beamDirection = tip->direction;
//    tip->direction = (Vector2){.x = beamDirection}
    tip->direction = rotateDegCW(tip->direction, hitPiece->beamIntersectProc[0].reflectionAngles);
}


void attachDefaultBeamIntersects(Piece* piece) {
    if(piece->type == SPLITTER) {
//        float *angle = malloc(sizeof(float));
//        *angle = 90.0f;
        piece->beamIntersectProc[0] = (BeamIntersect){.beamWill=REFLECT, .reflectionAngleCount=1,.reflectionAngles=90.0f};
    }
}

BeamIntersect* getBeamIntersect(Beam* tip, Piece* hitPiece) {
    if(tip->direction.x == 0 && tip->direction == 1) {

    }
}
