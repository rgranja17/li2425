#ifndef USER_H
#define USER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <glib.h>

#include "utils/date.h"
#include "utils/string.h"
#include "utils/pool.h"
#include "types/history.h"

typedef enum stype {
    PREMIUM,
    NORMAL
} stype_t;

typedef struct user_t user_t;

size_t user_sizeof(void);

user_t* user_alloc(pool_t* user_allocator);
//user_t* user_clone(user_t* user, pool_t* user_allocator);
user_t* user_clone(user_t* user, pool_t* user_allocator, pool_t* history_allocator);

guint user_get_array_size(user_t* user);

void __free_user__(void *user_t);

char* get_user_name(user_t *user_t);
int set_user_name(user_t *user, char *name);

char* get_user_email(user_t *user_t);
int set_user_email1(user_t *user, char *email);

char* get_user_first_name(user_t *user_t);
int set_user_first_name(user_t *user_t, char *first_name);

char* get_user_last_name(user_t *user_t);
int set_user_last_name(user_t *user, char *last_name);

char* get_user_birth_date_str(user_t *user_t);
date_t* get_user_birth_date_struct(user_t* user);
int set_user_birth_date_from_string(user_t *user,char *birth_date);
int set_user_birth_date(user_t *user,date_t *birth_date);

char* get_user_country(user_t *user_t);
int set_user_country(user_t *user, char *country);

int get_user_subscription_type(user_t *user_t);
int set_user_subscription_type(user_t *user,char *subscription_type);

int get_user_age(user_t* user);

GArray* get_user_liked_music_ids(user_t *user_t);
int set_user_liked_music_ids_from_string(user_t *user, char* str);
int set_user_liked_music_ids(user_t *user, GArray* liked_musics);

GArray* get_user_history(user_t* user, pool_t* history_allocator);
void add_user_history(user_t* user, history_t* history);

int create_user_from_line(user_t* artist, GArray* parsed_files);

#endif 