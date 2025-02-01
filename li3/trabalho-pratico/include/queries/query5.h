#ifndef QUERY_5
#define QUERY_5
#include "../recomendador/recomendador.h"
#include "data_manager/database_manager.h"
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* query5(Database_manager* db, const char* user_id, int num_recommendations);

#endif