//
// Created by Dustin Simpson on 1/18/21.
//
#include "laser-beam.h"
#include "laser-chess.h"
#include <stdlib.h>

Beam* createBeam(Vector2 start, Vector2 dir) {
    Beam* beam = (Beam*)malloc(sizeof(Beam));
    beam->tile = start;
    beam->exitDirection = dir;
    beam->nextCount = 0;
    beam->prev = NULL;

    return beam;
}

void destroyBeam(Beam* beam) {
//    Beam* cur = beam;
//    Beam** next;
//    int nextCount = 0;
//
//    do {
//        next = cur->next;
//        nextCount = cur->nextCount;
//        free(cur);
//        if(nextCount > 0) {
//            cur = next[0];
//            free(next);
//        }
//    } while(nextCount > 0);

}

void expandBeam(GameState* gs, Beam* tip) {
    Beam* next;
    Piece* collisionPiece;

    printf("In expand beam, tip is %d, %d Entry (%d, %d), Exit (%d, %d)\n", (int)tip->tile.x, (int)tip->tile.y, (int)tip->entryDirection.x, (int)tip->entryDirection.y, (int)tip->exitDirection.x, (int)tip->exitDirection.y);

    if(getPieceOnTile(gs, tip->tile, &collisionPiece) && tip->prev) {
        if (!processBeamPieceCollision(gs, tip, collisionPiece)) {
            return;
        }
    }
//            for(int i = 0; i < tip->nextCount; i++) {
//                expandBeam(gs, tip->next[i]);
//            }
//        }
//    } else {
//        for(int i = 0; i < tip->nextCount; i++) {
//            expandBeam(gs, tip->next[i]);
//        }
//    }


    if(isValidTile(addVector2(tip->tile, tip->exitDirection))) {
        next = (Beam*)malloc(sizeof(Beam));
        next->tile = roundVector2(addVector2(tip->tile, tip->exitDirection));
        next->entryDirection = tip->exitDirection;
        next->exitDirection = tip->exitDirection;
        next->nextCount = 0;
        next->prev = tip;
        tip->nextCount++;
        if(tip->nextCount == 1) {
            tip->next = (Beam **) malloc(sizeof(Beam *));
        } else {
            tip->next = (Beam **) realloc(tip->next, sizeof(Beam *) * tip->nextCount);
        }
        tip->next[tip->nextCount - 1] = next;

        for(int i = 0; i < tip->nextCount; i++) {
            expandBeam(gs, tip->next[i]);
        }

    }
}

void fireBeam(GameState* gs, Board* board) {
    Beam* cur, *next;
    Piece* collisionPiece;

    cur = gs->beam;
    expandBeam(gs, cur);
    printBeam(cur);
}

void splitBeam(Beam* tip, Vector2 direction) {
    Beam* prev = tip->prev;
    Beam* next;
    next = (Beam*)malloc(sizeof(Beam));
    next->tile = tip->tile;
//    next->tile = addVector2(tip->tile,direction);
    next->entryDirection = prev->exitDirection;
    next->exitDirection = direction;
//    next->prev = tip;
//    next->isForkHead = true;
    next->prev = NULL; // Somewhat of a hack to get the system to not reprocess the splitter collision
    next->nextCount = 0;

//    tip->nextCount++;
//    if(tip->nextCount == 1) {
//        tip->next = (Beam **) malloc(sizeof(Beam *));
//    } else {
//        tip->next = (Beam **) realloc(tip->next, sizeof(Beam *) * tip->nextCount);
//    }
//    tip->next[tip->nextCount - 1] = next;
    prev->nextCount++;
    if(prev->nextCount == 1) {
        prev->next = (Beam **) malloc(sizeof(Beam *));
    } else {
        prev->next = (Beam **) realloc(prev->next, sizeof(Beam *) * prev->nextCount);
    }
    prev->next[prev->nextCount - 1] = next;
}

void printBeam(Beam* beam) {
    printf("Tile %d,%d\n", (int)beam->tile.x, (int)beam->tile.y);
    for(int i = 0; i < beam->nextCount; i++) {
       printBeam(beam->next[i]);
    }
}
