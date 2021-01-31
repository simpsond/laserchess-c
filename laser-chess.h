//
// Created by Dustin Simpson on 1/8/21.
//

#ifndef LASER_CHESS_C_LASER_CHESS_H
#define LASER_CHESS_C_LASER_CHESS_H

#include <stdbool.h>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include "utils.h"
#include "GameState.h"
#include "Piece.h"
#include "Player.h"
#include "laser-beam.h"
#include "board.h"

#define MAXPIECES 36

typedef struct StructMove {
    Vector2 startPos;
    Vector2 endPos;
    bool willCapture;
    Piece* capturedPiece;
} Move;



// Utility Functions
bool getTileUnder(Board* b, Vector2 p, Vector2* tile);
void getBoxFromTile(Board* b, Vector2 tile, Vector2* p1, Vector2* p2);
bool getPieceOnTile(GameState* gs, Vector2 tile, Piece** piece);
bool isValidTile(Vector2 tile);

void reconcileDestroyedPieces(GameState* gs);

// Gameplay Functions
void endTurn(GameState* gs);
void endMove(GameState* gs);
void fireLaser(GameState* gs, Board* board);
void turnLaserOff(GameState* gs);
size_t getValidMoves(GameState* gs, Piece* piece, Move* moves);

#endif //LASER_CHESS_C_LASER_CHESS_H
