#ifndef QUERY6_H
#define QUERY6_H
#include "data_manager/database_manager.h"
#include "data_manager/user_manager.h"
#include "types/user.h" 
#include "types/music.h"
#include "types/history.h"
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <stdio.h>
#include "utils/date.h"
#include "utils/compare.h"
#include "utils/pool.h"
#include "utils/timestamp.h"
#include "utils/duration.h"

#define MAX_RESULT_QUERY6 1000

typedef struct favourite_artists_t favourite_artists_t;
typedef struct favourite_album_t favourite_album_t;

favourite_album_t* get_or_create_album(GArray *falbums_array,GHashTable *falbums_table,const char *album_id);
favourite_artists_t* get_or_create_fartist(GArray *fartists_array,GHashTable *fartists_table,const char *artist_id);

int compare_history_by_music_id(gpointer a, gpointer b);
int compare_fartists_by_listening_time(gpointer a, gpointer b);
int compare_favourite_albums(gpointer a, gpointer b);

int getGenreId(music_t* music);
char* get_genre_to_string(int genre);
int find_max (int array[], int size);

char* query6(Database_manager* db, char* user_id, int year, int N);

#endif