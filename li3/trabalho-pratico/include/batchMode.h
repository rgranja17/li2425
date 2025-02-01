#ifndef BATCHMODE_H
#define BATCHMODE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "data_manager/database_manager.h"
#include "interpreter.h"
#include "utils/output.h"
#include "queries/query1.h"
#include "queries/query2.h"
#include "queries/query3.h"
#include "queries/query5.h"
#include "queries/query6.h"


#define MAX_ARGS 20
#define MAX_COMMAND_SIZE 256 

void execute_command(char* command, Database_manager* db, char* output_file, int command_number);
void batchMode(char *inputFile, char *outputDirectory, Database_manager *db, char* dataset_path);

#endif
