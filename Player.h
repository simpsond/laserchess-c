//
// Created by Dustin Simpson on 1/23/21.
//

#ifndef LASER_CHESS_C_PLAYER_H
#define LASER_CHESS_C_PLAYER_H

#include <SDL.h>
#include "Player.h"

typedef enum {
    PLAYER1,
    PLAYER2
} player_t;

typedef struct StructPlayer {
    player_t id;
    SDL_Color color;
    char* name;
} Player;

Player* getPlayerById(player_t id, Player* players, size_t sizePlayers);

#endif //LASER_CHESS_C_PLAYER_H
