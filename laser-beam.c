//
// Created by Dustin Simpson on 1/18/21.
//
#include "laser-beam.h"
#include "laser-chess.h"
#include <stdlib.h>

Beam* createBeam(Vector2 start, Vector2 dir) {
    Beam* beam = (Beam*)malloc(sizeof(Beam));
    beam->tile = start;
    beam->direction = dir;
    beam->nextCount = 0;

    return beam;
}

void destroyBeam(Beam* beam) {
    Beam* cur = beam;
    Beam** next;
    int nextCount = 0;

    do {
        next = cur->next;
        nextCount = cur->nextCount;
        free(cur);
        if(nextCount > 0) {
            cur = next[0];
            free(next);
        }
    } while(nextCount > 0);

}

void fireBeam(Beam* beam, Board* board) {
    Beam* cur, *next;

    cur = beam;
    while(isValidTile(addVector2(cur->tile, cur->direction))) {
        next = (Beam*)malloc(sizeof(Beam));
        next->tile = addVector2(cur->tile, cur->direction);
        next->direction = cur->direction;
        next->nextCount = 0;

        cur->nextCount++;
        cur->next = (Beam**)malloc(sizeof (Beam*));
        cur->next[0] = next;

        cur = next;
    }
}
