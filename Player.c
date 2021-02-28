//
// Created by Dustin Simpson on 2/14/21.
//
#include "Player.h"

Player* getPlayerById(player_t id, Player* players, size_t sizePlayers) {
    for(int i = 0; i < sizePlayers; i++) {
        if(players[i].id == id) {
            return &players[i];
        }
    }

    return NULL;
}
