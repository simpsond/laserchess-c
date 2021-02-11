//
// Created by Dustin Simpson on 1/23/21.
//
#define LIBRG_IMPL
#include "third-party/librg/librg.h"
#include "GameState.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define GAMESTATEENTITYID 1
#define OWNERID 1

//int32_t mywrite_create(librg_world *w, librg_event *e) {
//    char *buffer = librg_event_buffer_get(w, e);
//    const char *str = "Hello world!";
//
//    /* make sure we have enough space */
//    if ((int32_t)strlen(str) > librg_event_size_get(w, e))
//        return LIBRG_WRITE_REJECT;
//
//    /* feel free to replace memcpy with any serialization library */
//    /* like: protobuf, flatbuf, msgpack, cap'n'proto, etc */
//    memcpy(buffer, str, strlen(str));
//
//    int64_t owner_id = librg_event_owner_get(w, e);
//    int64_t entity_id = librg_event_entity_get(w, e);
//    printf("entity: %d was created for owner: %d\n", (int)entity_id, (int)owner_id);
//
//    return strlen(str);
//}
//
//int32_t mywrite_update(librg_world *w, librg_event *e) {
//    int32_t my_secret_number = 42;
//    char *buffer = librg_event_buffer_get(w, e);
//    memcpy(buffer, &my_secret_number, sizeof(int32_t));
//    return sizeof(int32_t);
//}
//
//int32_t mywrite_remove(librg_world *w, librg_event *e) {
//    int64_t owner_id = librg_event_owner_get(w, e);
//    int64_t entity_id = librg_event_entity_get(w, e);
//    printf("entity: %d was removed for owner: %d\n", (int)entity_id, (int)owner_id);
//    return 0;
//}

void saveGameStateFile(GameState* gs, char* file) {
    struct stat st = {0};

    if (stat("saves", &st) == -1) {
        mkdir("saves", 0700);
    }

//    librg_world *world = librg_world_create();
//    if(!world) {
//        printf("world create failed\n");
//    }
//
//    librg_event_set(world, LIBRG_WRITE_CREATE, mywrite_create);
//    librg_event_set(world, LIBRG_WRITE_UPDATE, mywrite_update);
//    librg_event_set(world, LIBRG_WRITE_REMOVE, mywrite_remove);
//
//    librg_entity_track(world, GAMESTATEENTITYID);
//    librg_entity_chunk_set(world, GAMESTATEENTITYID, 1);
//    librg_entity_owner_set(world, GAMESTATEENTITYID, OWNERID);
//
//    char buffer[256] = {0};
//    size_t total_size = 256;
//
//    if(librg_world_write(world, OWNERID, buffer, &total_size, NULL) != LIBRG_OK) {
//        printf("error in world write\n");
//    } else {
//        char fileWPath[256] = "saves/";
//        strcat(fileWPath, file);
//        zpl_file f = {0};
//        zpl_file_open_mode(&f, ZPL_FILE_MODE_WRITE, fileWPath);
//        zpl_file_write(&f, buffer, total_size);
//        zpl_file_close(&f);
//    }
//
//    librg_world_destroy(world);
}
