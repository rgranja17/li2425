#ifndef QUERY3_H
#define QUERY3_H
#include "data_manager/database_manager.h"
#include "data_manager/user_manager.h"
#include "types/user.h" 
#include "types/music.h"
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <stdio.h>
#include "utils/date.h"
#include "utils/compare.h"
#include "utils/pool.h"

#define MAX_RESULT_QUERY3 1000

void insertion_sort_music_genres(GArray *array);
char* query3(Database_manager* db, int min_age, int max_age);

#endif