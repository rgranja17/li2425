#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "utils/date.h"

typedef struct timestamp timestamp_t;


timestamp_t* timestamp_alloc(void);
timestamp_t* timestamp_clone(timestamp_t* timestamp);
int timestamp_set_year(timestamp_t* time, int year);
int timestamp_set_month(timestamp_t* time, int month);
int timestamp_set_day(timestamp_t* time, int day);

int timestamp_set_hour(timestamp_t* time, int hour);
int timestamp_set_minutes(timestamp_t* time, int minutes);
int timestamp_set_seconds(timestamp_t* time, int seconds);

int timestamp_get_year(timestamp_t* time);
int timestamp_get_month(timestamp_t* time);
int timestamp_get_day(timestamp_t* time);
int timestamp_get_hour(timestamp_t* time);
int timestamp_get_minutes(timestamp_t* time);
int timestamp_get_seconds(timestamp_t* time);
date_t* get_time_stamp_date(timestamp_t* t);
int get_timestamp_duration_in_seconds(timestamp_t* t);
int timestamp_parse_from_string(timestamp_t* timestamp,char* time);
void __free_timestamp__(void *t);

int timestamp_get_day_of_year(timestamp_t* t);
char* day_of_year_to_timestamp(int day_of_year, int year);

#endif