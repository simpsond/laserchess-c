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
        if(gs->selectedPiece->rotation == 360) {
            gs->selectedPiece->rotation = 0;
        }
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

bool processBeamPieceCollision(GameState* gs, Beam* tip, Piece* hitPiece) {
//    Vector2 beamDirection = tip->direction;
//    tip->direction = (Vector2){.x = beamDirection}
    printf("Processing Beam Collision\n");
    Vector2 rotation_v =getVectorRotation(hitPiece);
    rotation_v.x *= -1;
    rotation_v.y *= -1;
    printf("Piece Rotation is %d degrees, %d,%d\n", hitPiece->rotation, (int)rotation_v.x, (int)rotation_v.y);
    float collisionAngle = calcAngleVector2(tip->direction, rotation_v);
    printf("Offset of IntersectProc is %d\n", (int)collisionAngle / 90);
    BeamIntersect * beamIntersect = &hitPiece->beamIntersectProc[(int)collisionAngle / 90];

    switch (beamIntersect->beamWill) {
        case REFLECT:
            tip->direction = rotateDegCW(tip->direction, beamIntersect->reflectionAngles);
            return true;
            break;
        case DESTROY:
            hitPiece->markedDestroy = true;
            return false;
            break;
        default:
            break;
    }
}


void attachDefaultBeamIntersects(Piece* piece) {
    if(piece->type == SPLITTER) {
//        float *angle = malloc(sizeof(float));
//        *angle = 90.0f;
        piece->beamIntersectProc[0] = (BeamIntersect){.beamWill=REFLECT, .reflectionAngleCount=1,.reflectionAngles=270.0f};
        piece->beamIntersectProc[1] = (BeamIntersect){.beamWill=REFLECT, .reflectionAngleCount=1,.reflectionAngles=270.0f};
        piece->beamIntersectProc[2] = (BeamIntersect){.beamWill=DESTROY, .reflectionAngleCount=0};
        piece->beamIntersectProc[3] = (BeamIntersect){.beamWill=REFLECT, .reflectionAngleCount=1,.reflectionAngles=90.0f};
    }
}

Vector2 getVectorRotation(Piece* piece) {
    return rotateDegCW((Vector2){0.0f, -1.0f}, piece->rotation);
}
