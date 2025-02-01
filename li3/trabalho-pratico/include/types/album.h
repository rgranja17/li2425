#ifndef ALBUM_H
#define ALBUM_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <glib.h>

#include "utils/pool.h"
#include "utils/string.h"

typedef struct album album_t;

size_t album_sizeof(void);

album_t* album_alloc(pool_t* album_allocator);
album_t* album_clone(album_t* album, pool_t* album_allocator);

char* get_album_id(album_t* album);
char* get_album_title(album_t* album);
GArray* get_album_artist_ids(album_t* album);
int get_album_num_artists(album_t* album);
uint16_t get_album_year(album_t* album);

int set_album_id(album_t* album, char* id);
int set_album_title(album_t* album, char* title);
int set_album_artists_from_string(album_t* album, char* artist_list);
int set_album_artist(album_t* album, GArray* artists);
int set_album_year(album_t* album, int year);

int create_album_from_line(album_t* album, GArray* parsed_files);

void __free_album__(void *a);

#endif
