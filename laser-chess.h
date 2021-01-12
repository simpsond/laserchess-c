//
// Created by Dustin Simpson on 1/8/21.
//

#ifndef LASER_CHESS_C_LASER_CHESS_H
#define LASER_CHESS_C_LASER_CHESS_H

#include <stdbool.h>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include "utils.h"

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

typedef struct structBoard {
    int x;
    int y;
    int width;
    int height;

    int blockWidth;
    int blockHeight;
    int blockPadding;

    SDL_Renderer* renderer;
} Board;

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
    bool isActive;
    pieces_t type;
} Piece;

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
} GameState;

// Drawing Functions
void drawBoard(Board* b, GameState* gs);
void drawPiece(Board* b, Piece* piece);
void drawTriangle(Vector2 p1, Vector2 p2, SDL_Color color, int rotation, struct SDL_Renderer* renderer);
void drawDiagonal(Vector2 p1, Vector2 p2, SDL_Color color, int rotation, struct SDL_Renderer* renderer);
void drawBlock(Vector2 p1, Vector2 p2, SDL_Color color, int rotation, struct SDL_Renderer* renderer);
void drawStraight(Vector2 p1, Vector2 p2, SDL_Color color, int rotation, struct SDL_Renderer* renderer);
void drawSplitter(Vector2 p1, Vector2 p2, SDL_Color color, int rotation, struct SDL_Renderer* renderer);
void drawHypercube(Vector2 p1, Vector2 p2, struct SDL_Color color, int rotation, struct SDL_Renderer* renderer);
void drawLaser(Vector2 p1, Vector2 p2, SDL_Color color, int rotation, struct SDL_Renderer* renderer);
void drawKing(Vector2 p1, Vector2 p2, SDL_Color color, int rotation, struct SDL_Renderer* renderer);

// Utility Functions
bool getTileUnder(Board* b, Vector2 p, Vector2* tile);
void getBoxFromTile(Board* b, Vector2 tile, Vector2* p1, Vector2* p2);
bool getPieceOnTile(GameState* gs, Vector2 tile, Piece* piece);

// Gameplay Functions
void endTurn(GameState* gs);

#endif //LASER_CHESS_C_LASER_CHESS_H
