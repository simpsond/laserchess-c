//
// Created by Dustin Simpson on 1/23/21.
//
#include "GameState.h"
#include "Piece.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


void loadGameStateFileMP(GameState* gs, char* file) {
    size_t fileSize;
    char fileWPath[256] = "saves/";
    FILE* fp = fopen(strcat(fileWPath, file),"r+");
    fseek(fp, 0L, SEEK_END);
    fileSize = ftell(fp);
    rewind(fp);

    char* buffer = malloc(fileSize);
    fread(buffer, 1,fileSize,fp);
    fclose(fp);

    mpack_tree_t tree;
    mpack_tree_init_data(&tree, buffer, fileSize);
    mpack_tree_parse(&tree);
    mpack_node_t root = mpack_tree_root(&tree);

    mpack_node_t tmpNode = mpack_node_map_str (root, "players", 7);
    msgUnpackPlayers(&tmpNode, gs->players);

    tmpNode = mpack_node_map_str (root, "pieceCount", 10);
    gs->pieceCount = mpack_node_u8(tmpNode);

    tmpNode = mpack_node_map_str (root, "activePlayer", 12);
    gs->activePlayer = mpack_node_u8(tmpNode);

    tmpNode = mpack_node_map_str (root, "movesLeft", 9);
    gs->movesLeft = mpack_node_u8(tmpNode);

    tmpNode = mpack_node_map_cstr(root, "isTileHovered");
    gs->isTileHovered = mpack_node_bool(tmpNode);

    tmpNode = mpack_node_map_cstr(root, "hoveredTile");
    msgUnpackVector2Int(&tmpNode, &gs->hoveredTile);

    tmpNode = mpack_node_map_cstr(root, "isPieceSelected");
    gs->isPieceSelected = mpack_node_bool(tmpNode);
    

    mpack_tree_destroy(&tree);
}

void saveGameStateFileMP(GameState* gs, char* file) {
    char* data;
    size_t size;
    mpack_writer_t writer;
    mpack_writer_init_growable(&writer, &data, &size);

    mpack_start_map(&writer, 14);
    mpack_write_cstr(&writer, "players");
    mpack_start_array(&writer,2);
    for(int i = 0; i < 2; i++) {

        msgPackPlayer(&writer, &gs->players[i]);
    }
    mpack_finish_array(&writer);

    mpack_write_cstr(&writer, "pieces");
    mpack_start_array(&writer,gs->pieceCount);
    for(int i = 0; i < gs->pieceCount; i++) {
        msgPackPiece(&writer, &gs->pieces[i]);
    }
    mpack_finish_array(&writer);

    mpack_write_cstr(&writer, "pieceCount");
    mpack_write_u8(&writer, gs->pieceCount);

    mpack_write_cstr(&writer, "activePlayer");
    mpack_write_u8(&writer, gs->activePlayer);

    mpack_write_cstr(&writer, "movesLeft");
    mpack_write_u8(&writer, gs->movesLeft);

    mpack_write_cstr(&writer, "isTileHovered");
    mpack_write_bool(&writer, gs->isTileHovered);

    mpack_write_cstr(&writer, "hoveredTile");
    msgPackVector2Int(&writer, &gs->hoveredTile);

    mpack_write_cstr(&writer, "isPieceSelected");
    mpack_write_bool(&writer, gs->isPieceSelected);

    mpack_write_cstr(&writer, "selectedPiece");
    if(gs->selectedPiece) {
        mpack_write_u8(&writer, gs->selectedPiece->id);
    } else {
        mpack_write_nil(&writer);
    }

    mpack_write_cstr(&writer, "isPieceRotating");
    mpack_write_bool(&writer, gs->isPieceRotating);

    mpack_write_cstr(&writer, "startingRotation");
    mpack_write_u8(&writer, gs->startingRotation);

    mpack_write_cstr(&writer, "isLaserOn");
    mpack_write_bool(&writer, gs->isLaserOn);

    mpack_write_cstr(&writer, "hasLaserFiredThisTurn");
    mpack_write_bool(&writer, gs->hasLaserFiredThisTurn);

    mpack_write_cstr(&writer, "beam");
    if(gs->beam) {
        msgPackBeam(&writer, gs->beam);
    } else {
        mpack_write_nil(&writer);
    }

    mpack_finish_map(&writer);

// finish writing
    if (mpack_writer_destroy(&writer) != mpack_ok) {
        fprintf(stderr, "An error occurred encoding the data!\n");
        return;
    }

    char fileWPath[256] = "saves/";
    FILE* fp = fopen(strcat(fileWPath, file),"w+");
    fwrite(data, 1, size, fp);
    free(data);

    fflush(fp);
    fclose(fp);
}

void msgPackBeam(mpack_writer_t* writer, struct StructBeam* beam) {
    mpack_start_map(writer, 6);

    mpack_write_cstr(writer, "entryDirection");
    msgPackVector2Int(writer, &beam->entryDirection);

    mpack_write_cstr(writer, "exitDirection");
    msgPackVector2Int(writer, &beam->exitDirection);

    mpack_write_cstr(writer, "tile");
    msgPackVector2Int(writer, &beam->tile);

    mpack_write_cstr(writer, "prev");
    mpack_write_u8(writer, beam->id);

    mpack_write_cstr(writer, "nextCount");
    mpack_write_u8(writer, beam->nextCount);

    mpack_write_cstr(writer, "next");
    mpack_start_array(writer, beam->nextCount);
    for(int i = 0; i < beam->nextCount; i++) {
        msgPackBeam(writer, beam->next[i]);
    }
    mpack_finish_array(writer);

    mpack_finish_map(writer);
}

void msgPackPiece(mpack_writer_t* writer, struct StructPiece* piece) {
    mpack_start_map(writer, 8);

    mpack_write_cstr(writer, "player");
    mpack_write_u8(writer, piece->player->id);

    mpack_write_cstr(writer, "id");
    mpack_write_u8(writer, piece->id);

    mpack_write_cstr(writer, "location");
    msgPackVector2(writer, &piece->location);

    mpack_write_cstr(writer, "rotation");
    mpack_write_u8(writer, piece->rotation);

    mpack_write_cstr(writer, "type");
    mpack_write_u8(writer, piece->type);

    mpack_write_cstr(writer, "isActive");
    mpack_write_bool(writer, piece->isActive);

    mpack_write_cstr(writer, "markedDestroy");
    mpack_write_bool(writer, piece->markedDestroy);

    mpack_write_cstr(writer, "beamIntersectProc");
    mpack_start_array(writer,4);
    for(int i = 0; i < 4; i++) {
       msgPackBeamIntersect(writer, &piece->beamIntersectProc[i]);
    }
    mpack_finish_array(writer);
    mpack_finish_map(writer);
}

void msgPackBeamIntersect(mpack_writer_t* writer, struct StructBeamIntersect* beamIntersect) {
    mpack_start_map(writer, 3);

    mpack_write_cstr(writer, "beamWill");
    mpack_write_u8(writer, beamIntersect->beamWill);

    mpack_write_cstr(writer, "reflectionAngleCount");
    mpack_write_u8(writer, beamIntersect->reflectionAngleCount);

    mpack_write_cstr(writer, "reflectionAngles");
    mpack_start_array(writer,beamIntersect->reflectionAngleCount);
    for(int i = 0; i < beamIntersect->reflectionAngleCount; i++) {
        mpack_write_float(writer, beamIntersect->reflectionAngles[i]);
    }
    mpack_finish_array(writer);

    mpack_finish_map(writer);
}

void msgPackVector2(mpack_writer_t* writer, Vector2* vector) {
    mpack_start_array(writer,2);
    mpack_write_float(writer, vector->x);
    mpack_write_float(writer, vector->y);
    mpack_finish_array(writer);
}

void msgPackVector2Int(mpack_writer_t* writer, Vector2* vector) {
    mpack_start_array(writer,2);
    mpack_write_int(writer, round(vector->x));
    mpack_write_int(writer, round(vector->y));
    mpack_finish_array(writer);
}

void msgPackPlayer(mpack_writer_t* writer, Player* player) {
    mpack_start_map(writer, 3);

    mpack_write_cstr(writer, "id");
    mpack_write_u8(writer, player->id);
    mpack_write_cstr(writer, "color");
    mpack_start_array(writer, 4);
    mpack_write_u8(writer, player->color.r);
    mpack_write_u8(writer, player->color.g);
    mpack_write_u8(writer, player->color.b);
    mpack_write_u8(writer, player->color.a);
    mpack_finish_array(writer);
    mpack_write_cstr(writer, "name");
    mpack_write_cstr(writer, player->name);
    mpack_finish_map(writer);
}

void msgUnpackPlayers(mpack_node_t* playersNode, Player* players) {
    mpack_node_t playerNode = mpack_node_array_at(*playersNode, 0);

    mpack_node_t tmpNode = mpack_node_map_str (playerNode, "id", 2);
    players[0].id = mpack_node_u8(tmpNode);

    mpack_node_t colorNode;
    tmpNode = mpack_node_map_str (playerNode, "color", 5);
    colorNode = mpack_node_array_at(tmpNode, 0);
    players[0].color.r = mpack_node_u8(colorNode);
    colorNode = mpack_node_array_at(tmpNode, 1);
    players[0].color.g = mpack_node_u8(colorNode);
    colorNode = mpack_node_array_at(tmpNode, 2);
    players[0].color.b = mpack_node_u8(colorNode);
    colorNode = mpack_node_array_at(tmpNode, 3);
    players[0].color.a = mpack_node_u8(colorNode);

//    tmpNode = mpack_node_map_str(playerNode, "name", 4);
//    mpack_node_copy_data(tmpNode,players[0].name,3);

}

void msgUnpackVector2Int(mpack_node_t* tmpNode, Vector2* vector) {
    mpack_node_t vCompNode = mpack_node_array_at(*tmpNode, 0);
    vector->x = mpack_node_int(vCompNode);
    vCompNode = mpack_node_array_at(*tmpNode, 1);
    vector->y = mpack_node_int(vCompNode);
}
