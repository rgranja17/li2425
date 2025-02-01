#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "data_manager/artist_manager.h"
#include "data_manager/music_manager.h"
#include "data_manager/user_manager.h"
#include "data_manager/album_manager.h"
//#include "data_manager/history_manager.h"

#include "types/user.h"
#include "types/album.h"
#include "types/history.h"
#include "types/artist.h"
#include "types/music.h"

#include "utils/output.h"

#include "parser/parserGen.h"

typedef struct Database_manager Database_manager;

Database_manager *newDatabase(void) ;
UserManager *getUserManager(Database_manager* db);
ArtistManager *getArtistManager(Database_manager* db);
MusicManager *getMusicManager(Database_manager* db);
AlbumManager *getAlbumManager(Database_manager* db);
//HistoryManager *getHistoryManager(Database_manager* db);

void database_manager_add_artist(Database_manager* db, artist_t* artist);
void database_manager_add_music(Database_manager* db, music_t* music);
void database_manager_add_user(Database_manager* db, user_t* user);
void database_manager_add_album(Database_manager* db, album_t* album);
//void database_manager_add_history(Database_manager* db, history_t* history);
void database_manager_add_history(Database_manager* db, history_t* history, char* user_id);

int database_manager_has_artist(Database_manager* db, char* id);
int database_manager_has_music(Database_manager* db, char* id);
int database_manager_has_user(Database_manager* db, char* id);
int database_manager_has_album(Database_manager* db, char* id);
//int database_manager_has_history(Database_manager* db, char* id);

artist_t* database_manager_alloc_new_artist(Database_manager* db);
music_t* database_manager_alloc_new_music(Database_manager* db);
user_t* database_manager_alloc_new_user(Database_manager* db);
album_t* database_manager_alloc_new_album(Database_manager* db);
history_t* database_manager_alloc_new_history(Database_manager* db);
void database_update_artist_duration(Database_manager* db, GArray* ids, duration_t* d);
void database_update_artist_num_albums(Database_manager* db, GArray* ids);
void database_update_artist_num_reproducoes(Database_manager* db, const char* music_id);

float get_artist_total_recipe(Database_manager* db,artist_t* artist);
void create_album_manager(Database_manager* db, const char* dataset_path);
void create_history_manager(Database_manager* db, const char* dataset_path);
void create_artist_manager(Database_manager* db, const char* dataset_path);
void create_music_manager_(Database_manager* db, const char* dataset_path);
void create_user_manager_(Database_manager* db, const char* dataset_path);



int validate_music(music_t * music, Database_manager* db);
int validate_user(user_t* user,Database_manager* db);

pool_t* database_get_history_allocator(Database_manager* db);

void free_database(Database_manager *db);

#endif
