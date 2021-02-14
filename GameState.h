//
// Created by Dustin Simpson on 1/23/21.
//

#ifndef LASER_CHESS_C_GAMESTATE_H
#define LASER_CHESS_C_GAMESTATE_H

#include "Player.h"
#include "laser-beam.h"
#include "third-party/mpack/mpack.h"
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

void loadGameStateFileMP(GameState* gs, char* file);
void loadGameStateFile(GameState* gs, char* file);

void saveGameStateFileMP(GameState* gs, char* file);
void loadGameStateFileMP(GameState* gs, char* file);

// Serialize Helpers
void msgPackPlayer(mpack_writer_t* writer, Player* player);
void msgPackPiece(mpack_writer_t* writer, struct StructPiece* piece);
void msgPackVector2(mpack_writer_t* writer, Vector2* vector);
void msgPackVector2Int(mpack_writer_t* writer, Vector2* vector);
void msgPackBeamIntersect(mpack_writer_t* writer, struct StructBeamIntersect* beamIntersect);
void msgPackBeam(mpack_writer_t* writer, struct StructBeam* beam);

// Deserialize Helpers
void msgUnpackPlayers(mpack_node_t* tmpNode, Player* players);
void msgUnpackVector2Int(mpack_node_t* tmpNode, Vector2* vector);

#endif //LASER_CHESS_C_GAMESTATE_H
