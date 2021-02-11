//
// Created by Dustin Simpson on 1/23/21.
//

#ifndef LASER_CHESS_C_GAMESTATE_H
#define LASER_CHESS_C_GAMESTATE_H

#include "Player.h"
#include "laser-beam.h"

struct StructPiece;

typedef struct StructGameState {
    Player* players;
    struct StructPiece* pieces;
    int pieceCount;

    int activePlayer;
    int movesLeft;

    bool isTileHovered;
    Vector2 hoveredTile;

    bool isPieceSelected;
    struct StructPiece* selectedPiece;
    bool isPieceRotating;
    int startingRotation;

    bool isLaserOn;
    bool hasLaserFiredThisTurn;
//    Vector2 laserPath[9][9];
    Beam *beam;
} GameState;

void saveGameStateFile(GameState* gs, char* file);

#endif //LASER_CHESS_C_GAMESTATE_H
