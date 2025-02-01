#ifndef MUSIC_MANAGER_H
#define MUSIC_MANAGER_H

#include "types/music.h"

#define MUSIC_MANAGER_BLOCK_SIZE 18000

typedef struct MusicManager MusicManager;

MusicManager* create_music_manager(void);
void free_music_manager(MusicManager *manager);
void add_music(MusicManager *manager, music_t*music);
music_t* get_music(MusicManager *manager, const char *id);
void remove_music(MusicManager *manager, const char *id);
void increment_music_stream(MusicManager* m ,char* music_id);

pool_t* get_music_allocator(MusicManager* manager);

int music_manager_has_key(MusicManager* manager, const char* key);

GHashTable* get_music_table(MusicManager* manager);
#endif
