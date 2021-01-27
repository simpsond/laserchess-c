//
// Created by Dustin Simpson on 1/9/21.
//
#include "laser-chess.h"
#include "laser-beam.h"

Vector2 getTileCenter(Board* b, int tileX, int tileY) {
    return (Vector2){b->blockWidth * tileX + b->x + (b->blockWidth / 2), b->blockHeight * tileY + b->y + (b->blockHeight / 2)};
}

void getBoxFromTile(Board* b, Vector2 tile, Vector2* p1, Vector2* p2) {
    p1->x = b->blockWidth * tile.x + b->x;
    p1->y = b->blockHeight * tile.y + b->y;

    p2->x = p1->x + b->blockWidth;
    p2->y = p1->y + b->blockHeight;
}

bool getPieceOnTile(GameState* gs, Vector2 tile, Piece** piece) {
    for(int i = 0; i < gs->pieceCount; i++) {
        Piece* testPiece = &gs->pieces[i];
        if(testPiece->isActive && testPiece->location.x == tile.x && testPiece->location.y == tile.y) {
            *piece = &gs->pieces[i];
            return true;
        }
    }
    return false;
}

bool isValidTile(Vector2 tile) {
    if(tile.x >= 0 && tile.x < 9 && tile.y >= 0 && tile.y < 9) {
        return true;
    } else {
        return false;
    }
}

void endTurn(GameState* gs) {
    gs->activePlayer++;
    if(gs->activePlayer > 1) {
        gs->activePlayer = 0;
    }
    gs->movesLeft = 2;
    gs->hasLaserFiredThisTurn = false;
}

void endMove(GameState* gs) {
    gs->movesLeft--;
    gs->isPieceSelected = false;
    if(gs->movesLeft == 0) {
        endTurn(gs);
    }
}



void fireLaser(GameState* gs, Board* board) {
    Vector2 laserDirVector;
    switch (gs->selectedPiece->rotation) {
        case 0:
            laserDirVector = (Vector2){0.0f, -1.0f};
            break;
        case 90:
            laserDirVector = (Vector2){1.0f, 0.0f};
            break;
        case 180:
            laserDirVector = (Vector2){0.0f, 1.0f};
            break;
        case 270:
            laserDirVector = (Vector2){-1.0f, 0.0f};
            break;
   }

    gs->beam = createBeam(gs->selectedPiece->location, laserDirVector);
    fireBeam(gs, board);
    gs->isLaserOn = true;

    gs->hasLaserFiredThisTurn = true;
    endMove(gs);
}

void turnLaserOff(GameState* gs) {

    gs->isLaserOn = false;
    destroyBeam(gs->beam);
}

size_t getValidMoves(GameState* gs, Piece* piece, Move* moves) {
    Piece* tmpTargetPiece;
    bool canCapture = false;
    Vector2 targetTile;
    bool pieceOnTargetTile;
    int validMoveCount = 0;

    Vector2 moveTests[4] = {
            {1.0f, 0.0f},
            {-1.0f, 0.0f},
            {0.0f, 1.0f},
            {0.0f, -1.0f}
    };

    if(piece->type == BLOCK || piece->type == KING || piece->type == HYPERCUBE) {
        canCapture = true;
    }

    for(int i = 0; i < 4; i++) {
        targetTile = (Vector2){piece->location.x + moveTests[i].x, piece->location.y + moveTests[i].y};
        if(isValidTile(targetTile)) {
            pieceOnTargetTile = getPieceOnTile(gs, targetTile,
                                               &tmpTargetPiece);
            if(!pieceOnTargetTile || canCapture) {
                *moves++ = (Move) {
                        (Vector2) {piece->location.x, piece->location.y},
                        targetTile,
                        pieceOnTargetTile,
                        tmpTargetPiece,
                };
                validMoveCount++;
            }
        }
    }

    return validMoveCount;
}

bool getTileUnder(Board* b, Vector2 p, Vector2* tile) {
    if(p.x < b->x || p.x > b->x + b->width || p.y < b->y || p.y > b->y + b->height) {
        return false;
    } else {
        tile->x = trunc(((p.x - b->x) / b->blockWidth));
        tile->y = trunc(((p.y - b->y) / b->blockHeight));
        return true;
    }
}
