#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER_SIZE 1024
#define MAX_PATH_LENGTH 256

void log_error(FILE* file, const char* error);

void save_result(int command_number, const char *res, const char *outputDir,int alternative_format);

int database_create_artists_error_file(void);
int database_create_users_error_file(void);
int database_create_musics_error_file(void);
int database_create_history_error_file(void);
int database_create_album_error_file(void);


#endif
