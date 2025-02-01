#include "utils/timestamp.h"

struct timestamp{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minutes;
    uint8_t seconds;
};

timestamp_t* timestamp_alloc(void){
    timestamp_t* timestamp = malloc(sizeof(timestamp_t));

    if(!timestamp) return NULL;

    timestamp->year = 0;
    timestamp->month = 0;
    timestamp->day = 0;
    timestamp->hour = 0;
    timestamp->minutes = 0;
    timestamp->seconds = 0;

    return timestamp;
}

timestamp_t* timestamp_clone(timestamp_t* timestamp){
    if(!timestamp)
        return NULL;
    
    timestamp_t* time = timestamp_alloc();
    if(!time)
        return NULL;

    time->year = timestamp->year;
    time->month = timestamp->month;
    time->day = timestamp->day;

    time->hour = timestamp->hour;
    time->minutes = timestamp->minutes;
    time->seconds = timestamp->seconds;

    return time;
}

int timestamp_get_year(timestamp_t* time){
    return time->year;
}

int timestamp_get_month(timestamp_t* time){
    return time->month;
}

int timestamp_get_day(timestamp_t* time){
    return time->day;
}

int timestamp_get_hour(timestamp_t* time){
    return time->hour;
}

int timestamp_get_minutes(timestamp_t* time){
    return time->minutes;
}

int timestamp_get_seconds(timestamp_t* time){
    return time->seconds;
}

int timestamp_set_year(timestamp_t* time, int year){
    if (year < 0) return 1;
    time->year = year;
    return 0;
}

int timestamp_set_month(timestamp_t* time, int month){
    if (month < 1 || month > 12) return 1;
    time->month = month;
    return 0;
}

int timestamp_set_day(timestamp_t* time, int day){
    if (day < 1 || day > 31) return 1; // Simplified validation
    time->day = day;
    return 0;
}

int timestamp_set_hour(timestamp_t* time, int hour){
    if (hour < 0 || hour > 23) return 1;
    time->hour = hour;
    return 0;
}

int timestamp_set_minutes(timestamp_t* time, int minutes){
    if(minutes < 0 || minutes > 59) return 1;
    time->minutes = minutes;
    return 0;
}

int timestamp_set_seconds(timestamp_t* time, int seconds){
    if(seconds < 0 || seconds > 59) return 1;
    time->seconds = seconds;
    return 0;
}

int get_timestamp_duration_in_seconds(timestamp_t* t){
    return (t->hour * 3600 + t->minutes * 60 + t->seconds);
}

int timestamp_get_day_of_year(timestamp_t* t) {
    int year = timestamp_get_year(t);
    int month = timestamp_get_month(t);
    int day = timestamp_get_day(t);
    int days_in_month[12] = {31, (!(year % 4) ? 29 : 28), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int day_of_year = 0;

    for (int i = 0; i < month - 1; i++) {
        day_of_year += days_in_month[i];
    }

    day_of_year += day;

    return day_of_year;
}

char* day_of_year_to_timestamp(int day_of_year, int year) {
    static char date_str[11];

    int days_in_month[12] = {31, (!(year % 4) ? 29 : 28), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    int month = 0;
    int day = 0;

    while (day_of_year > days_in_month[month]) {
        day_of_year -= days_in_month[month];
        month++;
    }
    
    day = day_of_year;

    snprintf(date_str, sizeof(date_str), "%d/%02d/%02d", year, month + 1, day);
    
    return date_str;
}

int timestamp_parse_from_string(timestamp_t* timestamp, char* time){
    if (!timestamp || !time) return 1;
    if(strlen(time) == 0) return 1;

    int day, month, year;
    int hour, minutes, seconds;
    if(sscanf(time, "%d/%d/%d %d:%d:%d", &year, &month, &day, &hour, &minutes, &seconds) != 6) return 1;

    if(year > 2024) return 1;
    if(month > 9 && year == 2024) return 1;
    if(day > 9 && month == 9 && year == 2024) return 1;

    if(timestamp_set_year(timestamp, year)) return 1;
    if(timestamp_set_month(timestamp, month)) return 1;
    if(timestamp_set_day(timestamp, day)) return 1;
    if(timestamp_set_hour(timestamp, hour)) return 1;
    if(timestamp_set_minutes(timestamp, minutes)) return 1;
    if(timestamp_set_seconds(timestamp, seconds)) return 1;

    return 0;
}

void __free_timestamp__(void *t){
    free((timestamp_t*) t);
}