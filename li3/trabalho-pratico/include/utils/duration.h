#ifndef DURATION_H
#define DURATION_H

#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


typedef struct duration duration_t;

size_t duration_sizeof(void);
duration_t* duration_clone(duration_t* d);
int duration_set_hour(duration_t* duration, int hour);
int duration_set_minute(duration_t* duration, int minute);
int duration_set_seconds(duration_t* duration, int seconds);

uint8_t duration_get_hour(duration_t* duration);
uint8_t duration_get_minute(duration_t* duration);
uint8_t duration_get_seconds(duration_t* duration);

int duration_parse_from_string(duration_t* duration, const char* str);
int duration_to_seconds(duration_t* duration);

char* duration_to_string(duration_t* duration);
duration_t* convert_seconds_to_duration(int total_seconds);

duration_t* duration_struct_alloc(void);
void duration_free(duration_t* duration);
#endif