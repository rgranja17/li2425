#include "types/history.h"

struct history{
    char* music_id;
    timestamp_t* timestamp;
    duration_t* duration;
    platform_t platform;
};

size_t history_sizeof(void) {return sizeof(history_t);}

size_t historyPTR_sizeof(void){return sizeof(history_t*);}

history_t* history_alloc(pool_t* history_allocator){
    history_t* new_history = pool_alloc_item(history_allocator);
    if(!new_history)
        return NULL;
    new_history->duration = duration_struct_alloc();
    if(!new_history->duration){
        free(new_history);
        return NULL;
    }
    new_history->timestamp = timestamp_alloc();
    if(!new_history->timestamp){
        free(new_history->duration);
        free(new_history);
        return NULL;
    }
    return new_history;
}

/*history_t* history_clone(history_t* history, pool_t* history_allocator){
    if (!history || !history_allocator) return NULL;

    history_t* new_history = history_alloc(history_allocator);
    if(!new_history)
        return NULL;
    
    duration_t* duration = get_history_duration(history);
    timestamp_t* timestamp = get_history_timestamp(history);

    new_history->music_id = strdup(history->music_id);
    free(new_history->duration);
    new_history->duration = duration;
    __free_timestamp__(new_history->timestamp);
    new_history->timestamp = timestamp;
    new_history->platform = history->platform;

    return new_history;
}*/

history_t* history_clone(history_t* history, pool_t* history_allocator) {
    if (!history || !history_allocator) return NULL;
   // printf(" %s ", history->music_id);

    history_t* new_history = history_alloc(history_allocator);
    if(!new_history)
        return NULL;
    new_history->music_id = get_history_music_id(history);
    new_history->timestamp = timestamp_clone(history->timestamp);
    new_history->duration = duration_clone(history->duration);
    new_history->platform = history->platform;

    return new_history;
}

char* get_history_music_id(history_t* history){
    return strdup(history->music_id);
}

timestamp_t* get_history_timestamp(history_t* history){
    timestamp_t* time = timestamp_clone(history->timestamp);
    if(!time)
        return NULL;

    return time;
}

duration_t* get_history_duration(history_t* history){
    duration_t* duration = duration_clone(history->duration);
    if(!duration)
        return NULL;

    return duration;
}

int get_history_platform(history_t* history){
    return history->platform;
}

int set_history_music_id(history_t* history, char* id){
    if(!history || !id) return 1;
    if(!strlen(id)) return 1;

    if(history->music_id){
        free(history->music_id);
    }
    history->music_id = strdup(id);
    return 0;
}

int set_history_timestamp(history_t* history, timestamp_t* timestamp){
    if(!history || !timestamp) return 1;

    timestamp_t* new_timestamp = timestamp_clone(timestamp);
    if(!new_timestamp)
        return 1;
    __free_timestamp__(history->timestamp);
    history->timestamp = new_timestamp;
    return 0;
}

int set_history_timestamp_from_string(history_t* history, char* timestamp){
    if(!history || !timestamp) return 1;
    if(strlen(timestamp) == 0) return 1;

    timestamp_t* new_timestamp = timestamp_alloc();
    if(!new_timestamp)
        return 1;
    if(timestamp_parse_from_string(new_timestamp, timestamp)){
        __free_timestamp__(new_timestamp);
        return 1;
    }
    __free_timestamp__(history->timestamp);
    history->timestamp = new_timestamp;
    return 0;
}

int set_history_duration(history_t* history, duration_t* duration){
    if(!history || !duration) return 1;

    duration_t* new_duration = duration_clone(duration);
    if(!new_duration)
        return 1;
    duration_free(history->duration);
    history->duration = new_duration;
    return 0;
}

int set_history_duration_from_string(history_t* history, char* duration){
    if(!duration || !history) return 1;
    if(strlen(duration) == 0) return 1;

    duration_t* new_duration = duration_struct_alloc();
    if(!new_duration)
        return 1;
    if(duration_parse_from_string(new_duration, duration)){
        duration_free(new_duration);
        return 1;
    }
    duration_free(history->duration);
    history->duration = new_duration;
    return 0;
}

int get_history_duration_seconds(history_t* history){
    return duration_to_seconds(history->duration);
}


int set_history_platform(history_t* history, char* platform){
    if(!platform || !history) return 1;
    if(strlen(platform) == 0) return 1;

    int len = strlen(platform);

    for(int i = 0; i < len; i++){
        platform[i] = tolower(platform[i]);
    }

    if(strcmp(platform, "mobile") == 0) {
        history->platform = MOBILE;
    } else if(strcmp(platform, "desktop") == 0) {
        history->platform = DESKTOP;
    } else {
        return 1;
    }
    return 0;
}

int create_history_from_line(history_t* history, GArray* parsed_files){
    if(!history){
        return 1;
    }
    if(parsed_files == NULL){
        return 1;
    }
    if(parsed_files->len == 0){
        return 1;
    }
    char* music_id = g_array_index(parsed_files, char*, 2);
    char* timestamp = g_array_index(parsed_files, char*, 3);
    char* duration = g_array_index(parsed_files, char*, 4);
    char* platform = g_array_index(parsed_files, char*, 5);

    if(set_history_music_id(history, music_id)){return 1;}
    if(set_history_timestamp_from_string(history, timestamp)){return 1;}
    if(set_history_duration_from_string(history, duration)){return 1;}
    if(set_history_platform(history, platform)){return 1;}

    return 0;
}

void __free_history__(void *h){
    if (!h) return;

    history_t* history = (history_t*)h;

    free(history->music_id);
    __free_timestamp__(history->timestamp);
    free(history->duration);
}