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
    Vector2 rotation_v =getVectorRotation(hitPiece);
    rotation_v.x *= -1;
    rotation_v.y *= -1;
    float collisionAngle = round(calcAngleVector2(tip->entryDirection, rotation_v));
    BeamIntersect * beamIntersect = &hitPiece->beamIntersectProc[(int)collisionAngle / 90];

//    if(tip->isForkHead) {
//        printf("Collision and isForkHead is true (%d, %d)\n", (int)tip->tile.x, (int)tip->tile.y);
//    } else {
        printf("Collision and isForkHead is false (%d, %d)\n", (int)tip->tile.x, (int)tip->tile.y);
//    }

    switch (beamIntersect->beamWill) {
        case REFLECT:
//            if(tip->isForkHead) {
//                break;
//            }
            tip->exitDirection = roundVector2(rotateDegCW(tip->entryDirection, beamIntersect->reflectionAngles[0]));
            for(int i = 1; i < beamIntersect->reflectionAngleCount; i++) {
                splitBeam(tip, rotateDegCW(tip->entryDirection, beamIntersect->reflectionAngles[i]));
            }
            return true;
            break;
        case DESTROY:
            hitPiece->markedDestroy = true;
            return false;
            break;
        case PASS:
            break;
        default:
            break;
    }

    return true;
}


void attachDefaultBeamIntersects(Piece* piece) {
    switch (piece->type) {
        case SPLITTER:
            piece->beamIntersectProc[0] = (BeamIntersect){.beamWill=REFLECT, .reflectionAngleCount=2,.reflectionAngles={270.0f, 90.0f}};
            piece->beamIntersectProc[1] = (BeamIntersect){.beamWill=REFLECT, .reflectionAngleCount=1,.reflectionAngles={270.0f}};
            piece->beamIntersectProc[2] = (BeamIntersect){.beamWill=DESTROY, .reflectionAngleCount=0};
            piece->beamIntersectProc[3] = (BeamIntersect){.beamWill=REFLECT, .reflectionAngleCount=1,.reflectionAngles={90.0f}};
            break;
        case STRAIGHT:
            piece->beamIntersectProc[0] = (BeamIntersect){.beamWill=DESTROY, .reflectionAngleCount=0};
            piece->beamIntersectProc[1] = (BeamIntersect){.beamWill=REFLECT, .reflectionAngleCount=1,.reflectionAngles={180.0f}};
            piece->beamIntersectProc[2] = (BeamIntersect){.beamWill=DESTROY, .reflectionAngleCount=0};
            piece->beamIntersectProc[3] = (BeamIntersect){.beamWill=REFLECT, .reflectionAngleCount=1,.reflectionAngles={180.0f}};
            break;
        case LASER:
            piece->beamIntersectProc[0] = (BeamIntersect){.beamWill=DESTROY, .reflectionAngleCount=0};
            piece->beamIntersectProc[1] = (BeamIntersect){.beamWill=DESTROY, .reflectionAngleCount=0};
            piece->beamIntersectProc[2] = (BeamIntersect){.beamWill=DESTROY, .reflectionAngleCount=0};
            piece->beamIntersectProc[3] = (BeamIntersect){.beamWill=DESTROY, .reflectionAngleCount=0};
            break;
        case BLOCK:
            piece->beamIntersectProc[0] = (BeamIntersect){.beamWill=REFLECT, .reflectionAngleCount=1,.reflectionAngles={180.0f}};
            piece->beamIntersectProc[1] = (BeamIntersect){.beamWill=DESTROY, .reflectionAngleCount=0};
            piece->beamIntersectProc[2] = (BeamIntersect){.beamWill=DESTROY, .reflectionAngleCount=0};
            piece->beamIntersectProc[3] = (BeamIntersect){.beamWill=DESTROY, .reflectionAngleCount=0};
            break;
        case HYPERCUBE:
            piece->beamIntersectProc[0] = (BeamIntersect){.beamWill=PASS, .reflectionAngleCount=0};
            piece->beamIntersectProc[1] = (BeamIntersect){.beamWill=PASS, .reflectionAngleCount=0};
            piece->beamIntersectProc[2] = (BeamIntersect){.beamWill=PASS, .reflectionAngleCount=0};
            piece->beamIntersectProc[3] = (BeamIntersect){.beamWill=PASS, .reflectionAngleCount=0};
            break;
        case TRIANGLE:
            piece->beamIntersectProc[0] = (BeamIntersect){.beamWill=REFLECT, .reflectionAngleCount=1, .reflectionAngles={270.0f}};
            piece->beamIntersectProc[1] = (BeamIntersect){.beamWill=DESTROY, .reflectionAngleCount=0};
            piece->beamIntersectProc[2] = (BeamIntersect){.beamWill=DESTROY, .reflectionAngleCount=0};
            piece->beamIntersectProc[3] = (BeamIntersect){.beamWill=REFLECT, .reflectionAngleCount=1, .reflectionAngles={90.0f}};
            break;
        case DIAGONAL:
            piece->beamIntersectProc[0] = (BeamIntersect){.beamWill=REFLECT, .reflectionAngleCount=1, .reflectionAngles={270.0f}};
            piece->beamIntersectProc[1] = (BeamIntersect){.beamWill=REFLECT, .reflectionAngleCount=1, .reflectionAngles={90.0f}};
            piece->beamIntersectProc[2] = (BeamIntersect){.beamWill=DESTROY, .reflectionAngleCount=0};
            piece->beamIntersectProc[3] = (BeamIntersect){.beamWill=DESTROY, .reflectionAngleCount=0};
            break;
        case KING:
            piece->beamIntersectProc[0] = (BeamIntersect){.beamWill=DESTROY, .reflectionAngleCount=0};
            piece->beamIntersectProc[1] = (BeamIntersect){.beamWill=DESTROY, .reflectionAngleCount=0};
            piece->beamIntersectProc[2] = (BeamIntersect){.beamWill=DESTROY, .reflectionAngleCount=0};
            piece->beamIntersectProc[3] = (BeamIntersect){.beamWill=DESTROY, .reflectionAngleCount=0};
            break;
        default:
            break;
    }
    if(piece->type == SPLITTER) {
//        float *angle = malloc(sizeof(float));
//        *angle = 90.0f;
    }
}

Vector2 getVectorRotation(Piece* piece) {
    return rotateDegCW((Vector2){0.0f, -1.0f}, piece->rotation);
}
