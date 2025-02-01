#ifndef QUERY2_H
#define QUERY2_H
#include "data_manager/database_manager.h"
#include "utils/compare.h"
#include "data_manager/artist_manager.h"
#include "types/artist.h" 
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <stdio.h>
#define MAX_RESULT_QUERY2 2100


char* query2 (Database_manager* db, const int N, char* country);

#endif