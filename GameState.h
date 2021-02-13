//
// Created by Dustin Simpson on 1/23/21.
//

#ifndef LASER_CHESS_C_GAMESTATE_H
#define LASER_CHESS_C_GAMESTATE_H

#include "Player.h"
#include "laser-beam.h"
#include "cbor.h"

struct StructPiece;
struct StructBeamIntersect;
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

// Serialize Helpers
void serializeVector2(struct cbor_item_t** vec2cbor, Vector2 vector);
void serializePlayers(struct cbor_item_t** playercbor, Player* players);
void serializePlayer(struct cbor_item_t** playercbor, Player player);
void serializeBeamIntersect(struct cbor_item_t** beamcbor, struct StructBeamIntersect* beamIntersect);
void serializePiece(struct cbor_item_t** piececbor, struct StructPiece* piece);
void serializeBeam(struct cbor_item_t** beamcbor, struct StructBeam* beam);

#endif //LASER_CHESS_C_GAMESTATE_H
