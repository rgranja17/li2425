#ifndef ARTIST_H
#define ARTIST_H

#include <glib.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "utils/string.h"
#include "utils/pool.h"
#include "utils/duration.h"

#include "types/music.h"

typedef enum type{
    INDIVIDUAL,
    GROUP
} type_t;

typedef struct artist_t artist_t;

artist_t* artist_alloc(pool_t* artist_allocator);
artist_t* artist_clone(artist_t* artist, pool_t* artist_allocator);

guint artist_get_array_size(artist_t* artist);
int create_artist_from_line(artist_t* artist, GArray* parsed_files);
void __free_artist__(void*artist_t);

size_t artist_sizeof(void);

char* get_artist_id(artist_t *artist_t);
int set_artist_id(artist_t *artist_t, char *id);

char* get_artist_name(artist_t *artist_t);
int set_artist_name(artist_t *artist_t, char *name);

float get_artist_recipe(artist_t *artist_t);
int set_artist_recipe(artist_t* artist, float recipe_per_stream);

GArray* get_artist_id_constituents(artist_t *artist_t);
int set_artist_id_constituents_from_string(artist_t* artist, char* str);

int create_artist_from_line(artist_t* artist, GArray* parsed_files);

char* get_artist_country(artist_t *artist_t);
int set_artist_country(artist_t *artist_t,char *country);

int get_artist_type(artist_t* artist_t);
char* get_artist_type_string(artist_t* artist_t);
int set_artist_type(artist_t *artist_t,char *type);

void increment_discography_duration(artist_t* artist, duration_t* music_duration);
void restart_discography_duration(artist_t* artist);
int get_artist_total_discography_length(artist_t* artist);

void increment_artist_num_albums(artist_t* artist);
void restart_artist_num_albums(artist_t* artist);
int get_artist_num_albums(artist_t* artist);

void add_artist_participation(artist_t* artist, char* group_id);
GArray* get_artist_group(artist_t* artist);
guint artist_get_array_size(artist_t* artist);


void increment_artist_num_reproducoes(artist_t* artist);
void restart_artist_num_reproducoes(artist_t* artist);
int get_artist_num_reproducoes(artist_t* artist);
GHashTable* get_artist_listening_time_table(artist_t* a);

void increment_artist_listening_time(artist_t* artist, char* week_key, int time);
int get_artist_listening_time(artist_t* artist, char* week_key);

#endif 