#ifndef QUERY1_H
#define QUERY1_H
#include "data_manager/database_manager.h"
#include "data_manager/user_manager.h"
#include "data_manager/artist_manager.h"
#include "types/user.h"
#include "types/artist.h" 
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <stdio.h>
#define MAX_RESULT_QUERY1 1000

char* query1(Database_manager* db, const char* id);

#endif 
