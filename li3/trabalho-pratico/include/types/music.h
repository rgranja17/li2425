#ifndef MUSIC_H
#define MUSIC_H

#include <glib.h>
#include <stdint.h>
#include <inttypes.h>
#include "utils/duration.h"
#include "utils/pool.h"
#include "utils/string.h"

typedef struct music_t music_t;

typedef enum genre {
    BLUES,
    CLASSICAL,
    COUNTRY,
    ELECTRONIC,
    HIPHOP,
    JAZZ,
    METAL,
    POP,
    REGGAE,
    ROCK
} genre_t;


music_t* music_alloc(pool_t* music_allocator);
music_t* music_clone(music_t* music, pool_t* music_allocator);

guint music_get_array_size(music_t* music);

void __free_music__(void *music);

size_t music_sizeof(void);
char* get_music_id(music_t *music);
int set_music_id(music_t *music,char *id);

char* get_music_title(music_t *music_t);
int set_music_title(music_t *music,char *title);

GArray* get_music_artist_ids(music_t *music_t);
int set_music_artist_ids_from_string(music_t *music, char* str);
int set_music_artist_ids(music_t *music, GArray* artists);

char* get_music_duration_str(music_t *music_t);
duration_t* get_music_duration_struct(music_t* music);
int set_music_duration_from_string(music_t *music, char* duration_str);
int set_music_duration(music_t *music, duration_t* duration);

int get_music_genre(music_t *music_t);
int set_music_genre(music_t *music, char *genre);

uint16_t get_music_year(music_t *music);
int set_music_year(music_t *music, int year);

char* get_music_album_id(music_t *music);
int set_music_album_id(music_t *music,char *album_id);

int create_music_from_line(music_t* music, GArray* parsed_files);

#endif