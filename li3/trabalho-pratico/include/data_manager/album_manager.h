#ifndef ALBUM_MANAGER_H
#define ALBUM_MANAGER_H

#include "types/album.h"
#include "utils/pool.h"
#include "utils/compare.h"
#include "data_manager/music_manager.h"
#include <glib.h>


#define ALBUM_MANAGER_BLOCK_SIZE 50000
typedef struct AlbumManager AlbumManager;

AlbumManager* __create_album_manager__(void);
void __free_album_manager__(AlbumManager *manager);
void __add_album__(AlbumManager* album_manager, album_t* album);
album_t* __get_album__(AlbumManager* manager, const char *id);
void __remove_album__(AlbumManager* manager, const char* id);

int album_manager_has_key(AlbumManager* manager, const char* key);
GHashTable* get_album_table(AlbumManager* album_manager);

pool_t* get_album_allocator(AlbumManager* manager);

#endif