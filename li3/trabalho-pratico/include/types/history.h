#ifndef HISTORY_H
#define HISTORY_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <glib.h>

#include "utils/timestamp.h"
#include "utils/duration.h"
#include "utils/pool.h"

typedef enum platform{
    DESKTOP,
    MOBILE
}platform_t;

typedef struct history history_t;
size_t history_sizeof(void);
size_t historyPTR_sizeof(void);
history_t* history_alloc(pool_t* history_allocator);
history_t* history_clone(history_t* history, pool_t* history_allocator);

char* get_history_id(history_t* history);
char* get_history_user_id(history_t* history);
char* get_history_music_id(history_t* history);
timestamp_t* get_history_timestamp(history_t* history);
duration_t* get_history_duration(history_t* history);
int get_history_platform(history_t* history);

int set_history_id(history_t* history, char* id);
int set_history_user_id(history_t* history, char* id);
int set_history_music_id(history_t* history, char* id);
int set_history_timestamp(history_t* history, timestamp_t* timestamp);
int set_history_timestamp_from_string(history_t* history, char* timestamp);
int set_history_duration(history_t* history, duration_t* duration);
int set_history_duration_from_string(history_t* history, char* duration);
int set_history_platform(history_t* history, char* platform);

int create_history_from_line(history_t* history, GArray* parsed_files);

void __free_history__(void *h);


#endif