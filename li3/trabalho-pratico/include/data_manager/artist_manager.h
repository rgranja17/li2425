#ifndef ARTIST_MANAGER_H
#define ARTIST_MANAGER_H

#include "types/artist.h"

#define ARTIST_BLOCK_LENGTH 1005

typedef struct ArtistManager ArtistManager;


ArtistManager* __create_artist_manager__(void);
void __free_artist_manager__(ArtistManager *manager);
void __add_artist__(ArtistManager* manager, artist_t* artist);
artist_t* __get_artist__(ArtistManager *manager, const char *id);
void __remove_artist__(ArtistManager* manager, const char* id);
pool_t* get_artist_allocator(ArtistManager* manager);

int artist_manager_has_key(ArtistManager* artist_mng, char* key);
void update_artist_discography_duration(ArtistManager* manager,char* key ,duration_t* duration);
void update_artist_num_albums(ArtistManager* manager,char* key);
void update_artist_num_reproducoes(ArtistManager* manager,char* key);

void increase_artist_listening_time(ArtistManager* manager,char* artist_id ,char* week_key, int time);
int get_artist_listening_time_per_week(ArtistManager* manager, char* artist_id, char* week_key);
GHashTable *get_top10Table (ArtistManager* m);
GHashTable* __get_artists_hashTable(ArtistManager* manager);
void initTop10(ArtistManager* m);

#endif