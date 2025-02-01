#include "utils/date.h"

struct date{
    uint8_t day;
    uint8_t month;
    uint16_t year;
};

size_t date_sizeof(void) {return sizeof(date_t);}

int date_set_day(date_t* date, int day){
    if(day < 1 || day > 31) return 1;
    date->day = day;
    return 0;
}

int date_set_month(date_t* date, int month){
    if(month < 1 || month > 12) return 1;
    date->month = month;
    return 0;
} 

int date_set_year(date_t* date, int year){
    if(year > 9999 || year < 0) return 1;
    date->year = year;
    return 0;
}

uint8_t date_get_day(date_t* date) {return date->day;}

uint8_t date_get_month(date_t* date) {return date->month;}

uint16_t date_get_year(date_t* date) {return date->year;}

int date_parse_from_str(date_t* date, const char* str){
    if (!date || !str) return 1;
    if(strlen(str) == 0) return 1;

    int day,month,year;
    if(sscanf(str,"%d/%d/%d",&year,&month,&day) != 3) return 1;

    if(date_set_day(date,day)) return 1;
    if(date_set_month(date,month)) return 1;
    if(date_set_year(date,year)) return 1;

    if(year > 2024) return 1;
    if(month > 9 && year == 2024) return 1;
    if(day > 9 && month == 9 && year == 2024) return 1;

    return 0;
}


int calculate_age(date_t* birth_date) {
    struct date current_date = {9, 9, 2024};

    int age = current_date.year - date_get_year(birth_date);

    if (current_date.month < date_get_month(birth_date) || 
       (current_date.month == date_get_month(birth_date) && current_date.day < date_get_day(birth_date))) {
        age--;
    }

    return age;
}

