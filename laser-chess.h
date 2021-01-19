//
// Created by Dustin Simpson on 1/8/21.
//

#ifndef LASER_CHESS_C_LASER_CHESS_H
#define LASER_CHESS_C_LASER_CHESS_H

#include <stdbool.h>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include "utils.h"
#include "laser-beam.h"
#include "board.h"

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
    PLAYER1,
    PLAYER2
} player_t;

typedef struct StructPlayer {
    SDL_Color color;
    char* name;
} Player;

typedef struct StructPiece {
    Player* player;
    Vector2 location;
    int rotation;
    pieces_t type;
    bool isActive;
} Piece;

typedef struct StructMove {
    Vector2 startPos;
    Vector2 endPos;
    bool willCapture;
    Piece* capturedPiece;
} Move;

typedef struct StructGameState {
    Player* players;
    Piece* pieces;
    int pieceCount;

    int activePlayer;
    int movesLeft;

    bool isTileHovered;
    Vector2 hoveredTile;

    bool isPieceSelected;
    Piece* selectedPiece;
    bool isPieceRotating;
    int startingRotation;

    bool isLaserOn;
    bool hasLaserFiredThisTurn;
//    Vector2 laserPath[9][9];
    Beam *beam;
} GameState;

// Utility Functions
bool getTileUnder(Board* b, Vector2 p, Vector2* tile);
void getBoxFromTile(Board* b, Vector2 tile, Vector2* p1, Vector2* p2);
bool getPieceOnTile(GameState* gs, Vector2 tile, Piece** piece);
bool isValidTile(Vector2 tile);

// Gameplay Functions
void endTurn(GameState* gs);
void endMove(GameState* gs);
void rotatePiece(GameState* gs);
void commitPieceRotation(GameState* gs);
void cancelPieceRotation(GameState* gs);
void fireLaser(GameState* gs, Board* board);
void turnLaserOff(GameState* gs);
size_t getValidMoves(GameState* gs, Piece* piece, Move* moves);

#endif //LASER_CHESS_C_LASER_CHESS_H
