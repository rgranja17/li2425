#include "utils/duration.h"


struct duration{
    uint8_t hour;
    uint8_t minute;
    uint8_t seconds;
};

duration_t* duration_struct_alloc(void){
    return malloc(sizeof(duration_t));
}

size_t duration_sizeof(void) {return sizeof(duration_t);}

duration_t* duration_clone(duration_t* d){
    if(!d)
        return NULL;
    duration_t* duration = duration_struct_alloc();
    if(!duration)
        return NULL;
    
    duration_set_hour(duration, duration_get_hour(d));
    duration_set_minute(duration, duration_get_minute(d));
    duration_set_seconds(duration, duration_get_seconds(d));

    return duration;
}

void duration_free(duration_t* duration){
    if(!duration) return;
    free(duration);
}

int duration_set_hour(duration_t* duration, int hour){
    if (hour < 0 || hour > 99) return 1;
    duration->hour = hour;
    return 0;
}

int duration_set_minute(duration_t* duration, int minute){
    if(minute < 0 || minute > 59) return 1;
    duration->minute = minute;
    return 0;
}

int duration_set_seconds(duration_t* duration, int seconds){
    if(seconds < 0 || seconds > 59) return 1;
    duration->seconds = seconds;
    return 0;
}

uint8_t duration_get_hour(duration_t* duration){return duration->hour;}

uint8_t duration_get_minute(duration_t* duration){return duration->minute;}

uint8_t duration_get_seconds(duration_t* duration){return duration->seconds;}

int duration_parse_from_string(duration_t* duration, const char* str){
    if(!duration) return 1;
    if (strlen(str) == 0) return 1;

    int hour, minute, seconds;
    if (sscanf(str, "%d:%d:%d", &hour, &minute, &seconds) != 3) return 1;

    if (duration_set_hour(duration, hour)) return 1;
    if (duration_set_minute(duration, minute)) return 1;
    if (duration_set_seconds(duration, seconds)) return 1;

    return 0;
}

int duration_to_seconds(duration_t* duration){
    return ((duration->hour * 60 * 60) + (duration->minute * 60) + duration->seconds);
}

char* duration_to_string(duration_t* duration){
    char* str = malloc(20* sizeof(char*));
    int hour,minute,seconds;
    hour = duration_get_hour(duration);
    minute = duration_get_minute(duration);
    seconds = duration_get_seconds(duration);
    
    snprintf(str, 20* sizeof(char*), "%02d:%02d:%02d", hour,minute,seconds);

    return str;
}

 duration_t* convert_seconds_to_duration(int total_seconds) {
    duration_t* result = malloc(sizeof(duration_t));

    result->hour = total_seconds / 3600;
    total_seconds %= 3600;
    result->minute = total_seconds / 60;
    result->seconds = total_seconds % 60;

    return result;
}