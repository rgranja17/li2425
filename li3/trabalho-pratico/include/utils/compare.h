#ifndef COMPARE_GENRE_H
#define COMPARE_GENRE_H

#include <glib.h>
#include <string.h>
#include "types/music.h"


typedef struct music_genre music_genre_t;


typedef struct GenreLikes GenreLikes;
typedef struct artistduration artistduration;

music_genre_t* music_genre_alloc(void);
int get_music_genre_type(music_genre_t* genre);
int get_music_likes(music_genre_t* genre);
void increase_music_likes(music_genre_t* genre);
size_t music_genre_sizeof(void);
void music_genre_free(music_genre_t* genre);
void set_music_genre_type(music_genre_t* genre, int type);
const char* genre_to_string(genre_t genre);
void sum_like_count(GArray* origin, GArray* to_sum);
GArray* music_genre_alloc_array(void);

int compare_genre_likes(const void* a, const void* b);
int compare_discography_duration (const void* a, const void* b);
#endif
