#ifndef PARSERGEN_H
#define PARSERGEN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

void free_garray_parsed_elements(GArray *array);
int process_line(FILE *file, GArray *array, char **lineCopy);
void free_garray_parsed(GArray *array);

#endif
