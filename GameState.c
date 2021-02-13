//
// Created by Dustin Simpson on 1/23/21.
//
#include "GameState.h"
#include "Piece.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void saveGameStateFile(GameState* gs, char* file) {
    struct stat st = {0};

    if (stat("saves", &st) == -1) {
        mkdir("saves", 0700);
    }

    /* Preallocate the map structure */
    cbor_item_t * root = cbor_new_definite_map(14);
    cbor_item_t* tmp;

    serializePlayers(&tmp, gs->players);
    cbor_map_add(root, (struct cbor_pair) {
            cbor_move(cbor_build_string("players")),
            cbor_move(tmp)
    });

    tmp = cbor_new_definite_array(gs->pieceCount);
    cbor_item_t* piecetmp;
    for(int i = 0; i < gs->pieceCount; i++) {
        serializePiece(&piecetmp, &gs->pieces[i]);
        cbor_array_set(tmp, i, cbor_move(piecetmp));
    }
    cbor_decref(&piecetmp);
    cbor_map_add(root, (struct cbor_pair) {
            cbor_move(cbor_build_string("pieces")),
            cbor_move(tmp)
    });

    cbor_map_add(root, (struct cbor_pair) {
            .key = cbor_move(cbor_build_string("pieceCount")),
            .value = cbor_move(cbor_build_uint16(gs->pieceCount))
    });

    cbor_map_add(root, (struct cbor_pair) {
            .key = cbor_move(cbor_build_string("activePlayer")),
            .value = cbor_move(cbor_build_uint16(gs->activePlayer))
    });

    cbor_map_add(root, (struct cbor_pair) {
            .key = cbor_move(cbor_build_string("movesLeft")),
            .value = cbor_move(cbor_build_uint16(gs->movesLeft))
    });
    cbor_map_add(root, (struct cbor_pair) {
            .key = cbor_move(cbor_build_string("isTileHovered")),
            .value = cbor_move(cbor_build_uint16(gs->isTileHovered))
    });
    serializeVector2(&tmp, gs->hoveredTile);
    cbor_map_add(root, (struct cbor_pair) {
            .key = cbor_move(cbor_build_string("hoveredTile")),
            .value = cbor_move(tmp)
    });

    cbor_map_add(root, (struct cbor_pair) {
            .key = cbor_move(cbor_build_string("isPieceSelected")),
            .value = cbor_move(cbor_build_bool(gs->isPieceSelected))
    });

    if(gs->selectedPiece) {
        cbor_map_add(root, (struct cbor_pair) {
                .key = cbor_move(cbor_build_string("selectedPiece")),
                .value = cbor_move(cbor_build_uint8(gs->selectedPiece->id))
        });
    }

    cbor_map_add(root, (struct cbor_pair) {
            .key = cbor_move(cbor_build_string("isPieceRotating")),
            .value = cbor_move(cbor_build_bool(gs->isPieceRotating))
    });

    cbor_map_add(root, (struct cbor_pair) {
            .key = cbor_move(cbor_build_string("startingRotation")),
            .value = cbor_move(cbor_build_uint8(gs->startingRotation))
    });
    cbor_map_add(root, (struct cbor_pair) {
            .key = cbor_move(cbor_build_string("isLaserOn")),
            .value = cbor_move(cbor_build_bool(gs->isLaserOn))
    });

    cbor_map_add(root, (struct cbor_pair) {
            .key = cbor_move(cbor_build_string("hasLaserFiredThisTurn")),
            .value = cbor_move(cbor_build_bool(gs->hasLaserFiredThisTurn))
    });

    if(gs->beam) {
        serializeBeam(&tmp, gs->beam);
        cbor_map_add(root, (struct cbor_pair) {
                .key = cbor_move(cbor_build_string("beam")),
                .value = cbor_move(tmp)
        });
//        cbor_decref(&tmp);
    }

    /* Output: `length` bytes of data in the `buffer` */
    unsigned char * buffer;
    size_t buffer_size, length = cbor_serialize_alloc(root, &buffer, &buffer_size);

    char fileWPath[256] = "saves/";
    FILE* fp = fopen(strcat(fileWPath, file),"w+");
    fwrite(buffer, 1, length, fp);
    free(buffer);

    fflush(fp);
    fclose(fp);
    cbor_decref(&root);
}

void serializeVector2(cbor_item_t** vec2cbor, Vector2 vector) {
    *vec2cbor = cbor_new_definite_map(2);

    cbor_map_add(*vec2cbor, (struct cbor_pair) {
        cbor_move(cbor_build_string("x")),
            cbor_move(cbor_build_float8(vector.x))
    });
    cbor_map_add(*vec2cbor, (struct cbor_pair) {
            cbor_move(cbor_build_string("y")),
            cbor_move(cbor_build_float8(vector.y))
    });
}

void serializePlayer(struct cbor_item_t** playercbor, Player player) {
    *playercbor = cbor_new_definite_map(6);

    cbor_map_add(*playercbor, (struct cbor_pair) {
            cbor_move(cbor_build_string("id")),
            cbor_move(cbor_build_uint8(player.id))
    });
    cbor_map_add(*playercbor, (struct cbor_pair) {
            cbor_move(cbor_build_string("name")),
            cbor_move(cbor_build_string(player.name))
    });
    cbor_map_add(*playercbor, (struct cbor_pair) {
            cbor_move(cbor_build_string("color_a")),
            cbor_move(cbor_build_uint8(player.color.a))
    });
    cbor_map_add(*playercbor, (struct cbor_pair) {
            cbor_move(cbor_build_string("color_r")),
            cbor_move(cbor_build_uint8(player.color.r))
    });
    cbor_map_add(*playercbor, (struct cbor_pair) {
            cbor_move(cbor_build_string("color_g")),
            cbor_move(cbor_build_uint8(player.color.g))
    });
    cbor_map_add(*playercbor, (struct cbor_pair) {
            cbor_move(cbor_build_string("color_b")),
            cbor_move(cbor_build_uint8(player.color.b))
    });
}

void serializePlayers(struct cbor_item_t** playercbor, Player* players) {
    cbor_item_t *tmp;
   *playercbor = cbor_new_definite_array(2);
   for(int i = 0; i < 2; i++) {
       serializePlayer(&tmp, players[i]);
       cbor_array_set(*playercbor,i, cbor_move(tmp));
   }
}

void serializeBeamIntersect(struct cbor_item_t** beamcbor, struct StructBeamIntersect* beamIntersect) {
    cbor_item_t *tmp = cbor_new_definite_array(2);
   *beamcbor = cbor_new_definite_map(3);

    cbor_map_add(*beamcbor, (struct cbor_pair) {
            cbor_move(cbor_build_string("beamWill")),
            cbor_move(cbor_build_uint8(beamIntersect->beamWill))
    });
    cbor_map_add(*beamcbor, (struct cbor_pair) {
            cbor_move(cbor_build_string("reflectionAngleCount")),
            cbor_move(cbor_build_uint8(beamIntersect->reflectionAngleCount))
    });

    for(int i = 0; i < beamIntersect->reflectionAngleCount; i++) {
        cbor_array_set(tmp, i, cbor_move(cbor_build_float8(beamIntersect->reflectionAngles[i])));
    }
    cbor_map_add(*beamcbor, (struct cbor_pair) {
            cbor_move(cbor_build_string("reflectionAngles")),
            cbor_move(tmp)
    });
}

void serializePiece(struct cbor_item_t** piececbor, struct StructPiece* piece) {
    cbor_item_t* tmp;
    *piececbor = cbor_new_definite_map(8);

    cbor_map_add(*piececbor, (struct cbor_pair) {
            cbor_move(cbor_build_string("player")),
            cbor_move(cbor_build_uint8(piece->player->id))
    });
    cbor_map_add(*piececbor, (struct cbor_pair) {
            cbor_move(cbor_build_string("id")),
            cbor_move(cbor_build_uint8(piece->id))
    });
    serializeVector2(&tmp, piece->location);
    cbor_map_add(*piececbor, (struct cbor_pair) {
            cbor_move(cbor_build_string("location")),
            cbor_move(tmp)
    });
    cbor_map_add(*piececbor, (struct cbor_pair) {
            cbor_move(cbor_build_string("rotation")),
            cbor_move(cbor_build_uint8(piece->rotation))
    });
    cbor_map_add(*piececbor, (struct cbor_pair) {
            cbor_move(cbor_build_string("type")),
            cbor_move(cbor_build_uint8(piece->type))
    });
    cbor_map_add(*piececbor, (struct cbor_pair) {
            cbor_move(cbor_build_string("isActive")),
            cbor_move(cbor_build_bool(piece->isActive))
    });
    cbor_map_add(*piececbor, (struct cbor_pair) {
            cbor_move(cbor_build_string("markedDestroy")),
            cbor_move(cbor_build_bool(piece->markedDestroy))
    });
    tmp = cbor_new_definite_array(4);
    cbor_item_t* beamintersect;
    for(int i = 0; i < 4; i++) {
        serializeBeamIntersect(&beamintersect, &piece->beamIntersectProc[i]);
        cbor_array_set(tmp, i, cbor_move(beamintersect));
    }
    cbor_map_add(*piececbor, (struct cbor_pair) {
            cbor_move(cbor_build_string("beamIntersectProc")),
            cbor_move(tmp)
    });
}

void serializeBeam(struct cbor_item_t** beamcbor, struct StructBeam* beam) {
    *beamcbor = cbor_new_definite_map(6);
    cbor_item_t* tmp;

    serializeVector2(&tmp, beam->entryDirection);
    cbor_map_add(*beamcbor, (struct cbor_pair) {
            cbor_move(cbor_build_string("entryDirection")),
            cbor_move(tmp)
    });

    serializeVector2(&tmp, beam->exitDirection);
    cbor_map_add(*beamcbor, (struct cbor_pair) {
            cbor_move(cbor_build_string("exitDirection")),
            cbor_move(tmp)
    });

    serializeVector2(&tmp, beam->tile);
    cbor_map_add(*beamcbor, (struct cbor_pair) {
            cbor_move(cbor_build_string("tile")),
            cbor_move(tmp)
    });

    cbor_item_t* nexts = cbor_new_definite_array(beam->nextCount);
    for(int i = 0; i < beam->nextCount; i++) {
        serializeBeam(&tmp, beam->next[i]);
        cbor_array_set(nexts, i, tmp);
    }

    cbor_map_add(*beamcbor, (struct cbor_pair) {
            cbor_move(cbor_build_string("next")),
            cbor_move(nexts)
    });

    cbor_map_add(*beamcbor, (struct cbor_pair) {
            cbor_move(cbor_build_string("nextCount")),
            cbor_move(cbor_build_uint8(beam->nextCount))
    });

}
