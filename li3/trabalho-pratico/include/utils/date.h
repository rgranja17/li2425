#ifndef DATE_H
#define DATE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <inttypes.h>


typedef struct date date_t;

size_t date_sizeof(void);

int date_set_day(date_t* date, int day);
int date_set_month(date_t* date, int month);
int date_set_year(date_t* date, int year);

uint8_t date_get_day(date_t* date);
uint8_t date_get_month(date_t* date);
uint16_t date_get_year(date_t* date);

int date_parse_from_str(date_t* date, const char* str);

int calculate_age(date_t* birth_date);

#endif
