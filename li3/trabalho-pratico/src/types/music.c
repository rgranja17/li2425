#include "types/music.h"


struct music_t {
    char *id;
    char *title; 
    GArray *artist_ids;
    duration_t* duration;
    genre_t genre;
    uint16_t year;
    char *album_id;
};

music_t* music_alloc(pool_t* music_allocator){
    music_t* new_music = pool_alloc_item(music_allocator);
    if(!new_music)
        return NULL;
    new_music->artist_ids = g_array_new(FALSE,FALSE,sizeof(char*));
    new_music->duration = duration_struct_alloc();
    return new_music;
}

music_t* music_clone(music_t* music, pool_t* music_allocator) {
    if (!music || !music_allocator) return NULL;

    music_t* new_music = music_alloc(music_allocator);
    if (!new_music) return NULL;

    new_music->id = get_music_id(music);
    new_music->title = get_music_title(music);
    new_music->album_id = get_music_album_id(music);

    if (!new_music->artist_ids) {
        free(new_music->id);
        free(new_music->title);
        free(new_music->album_id);
        free(new_music);
        return NULL;
    }

    if (music->artist_ids != NULL) {
        for (guint i = 0; i < music->artist_ids->len; i++) {
            char *id = g_array_index(music->artist_ids, char*, i);
            char *id_copy = strdup(id);
            if (id_copy) {
                g_array_append_val(new_music->artist_ids, id_copy);
            }
        }
    }

    new_music->genre = get_music_genre(music);
    new_music->year = get_music_year(music);

    return new_music;
}

guint music_get_array_size(music_t* music){
    return music->artist_ids->len;
}

size_t music_sizeof(void) {return sizeof(music_t);}

char* get_music_id(music_t *music) {
    return strdup(music->id);
}

char* get_music_title(music_t *music) {
    return strdup(music->title);
}

GArray* get_music_artist_ids(music_t *music) {
    GArray *artists_copy = g_array_new(FALSE, FALSE, sizeof(char*));
    for (guint i = 0; i < music->artist_ids->len; i++) {
        char *id = g_array_index(music->artist_ids, char*, i);
        char* id_cpy = strdup(id);
        g_array_append_val(artists_copy, id_cpy); 
    }
    return artists_copy;
}

char* get_music_duration_str(music_t *music) {
    char* duration = malloc(20 * sizeof(char));
    if (duration == NULL) {
        perror("Memory allocation failed");
        exit(1);
    }

    int hour = duration_get_hour(music->duration);
    int minute = duration_get_minute(music->duration);
    int seconds = duration_get_seconds(music->duration);

    snprintf(duration, 20, "%04d:%02d:%02d", hour, minute, seconds);

    return duration;
}

duration_t* get_music_duration_struct(music_t* music){
    duration_t* new_duration = malloc(duration_sizeof());
    if(!new_duration)
        return NULL;
    duration_set_hour(new_duration, (int)duration_get_hour(music->duration));
    duration_set_minute(new_duration, (int)duration_get_minute(music->duration));
    duration_set_seconds(new_duration, (int)duration_get_seconds(music->duration));
    return new_duration;
}

int get_music_genre(music_t *music) {
    return music->genre;
}

uint16_t get_music_year(music_t *music) {
    return music->year;
}

char* get_music_album_id(music_t *music) {
    return strdup(music->album_id);
}



int set_music_id(music_t *music,char *id) {
    if(!id || !music) return 1;
    if(strlen(id) == 0) return 1;

    if(music->id){
        free(music->id);
    }
    music->id = strdup(id);
    return 0;
}

int set_music_title(music_t *music,char *title) {
    if(!title || !music) return 1;
    if(strlen(title) == 0) return 1;

    if(music->title){
        free(music->title);
    }
    music->title = strdup(title);
    return 0;
}

int set_music_album_id(music_t *music,char *album_id) {
    if(!album_id || !music) return 1;
    if(strlen(album_id) == 0) return 1;

    if(music->album_id){
        free(music->album_id);
    }
    music->album_id = strdup(album_id);
    return 0;
}

int set_music_artist_ids_from_string(music_t *music, char* str) { 
    if (!music || !str) return 1;

    for (guint i = 0; i < music->artist_ids->len; i++) {
        free(g_array_index(music->artist_ids, char*, i));
    }
    g_array_free(music->artist_ids, TRUE);

    music->artist_ids = string_to_array1(str);
    if(!music->artist_ids){
        music->artist_ids = g_array_new(FALSE,FALSE,sizeof(char*));
        return 1;
    }

    return 0;
}
int set_music_artist_ids(music_t *music, GArray* artists) {
    if(!music) return 1;
    if(artists == NULL) return 1;

    GArray* artist_list = g_array_new(FALSE, FALSE, sizeof(char*));
    for(guint i = 0; i<artists->len;i++){
        char* id = g_array_index(artists, char*, i);
        char* id_cpy = strdup(id);
        g_array_append_val(artist_list, id_cpy);
    }

    if(artist_list == NULL) return 1;

    for (guint i = 0; i < music->artist_ids->len; i++) {
        free(g_array_index(music->artist_ids, char*, i));
    }

    g_array_free(music->artist_ids, TRUE);
    music->artist_ids = artist_list;

    return 0;
}

int set_music_duration_from_string(music_t *music, char* duration_str) {
    if(!duration_str || !music) return 1;

    duration_t* duration = duration_struct_alloc();
    if(!duration) return 1;
    if(duration_parse_from_string(duration,duration_str)) {
        duration_free(duration);
        return 1;
    }
    if(music->duration){
        duration_free(music->duration);
    }
    music->duration = duration;
    return 0;
}

int set_music_duration(music_t *music, duration_t* duration) {
    if(!duration || !music) return 1;

    duration_t* durationC = duration_clone(duration);

    if(music->duration){
        duration_free(music->duration);
    }
    music->duration = durationC;
    return 0;
}

int set_music_genre(music_t *music, char *genre) {
    if(!genre || !music) return 1;
    if(strlen(genre) == 0) return 1;

    if(strcmp(genre,"Metal") == 0) music->genre = METAL;
    if(strcmp(genre,"Jazz") == 0) music->genre = JAZZ;
    if(strcmp(genre,"Rock") == 0) music->genre = ROCK;
    if(strcmp(genre,"Reggae") == 0) music->genre = REGGAE;
    if(strcmp(genre,"Classical") == 0) music->genre = CLASSICAL;
    if(strcmp(genre,"Hip Hop") == 0) music->genre = HIPHOP;
    if(strcmp(genre,"Blues") == 0) music->genre = BLUES;
    if(strcmp(genre,"Pop") == 0) music->genre = POP;
    if(strcmp(genre,"Country") == 0) music->genre = COUNTRY;
    if(strcmp(genre,"Electronic") == 0) music->genre = ELECTRONIC;
    return 0;
}

int set_music_year(music_t *music, int year){
    if(!music) return 1;

    if(year <= 0) return 1;
    music->year = year;
    return 0;
}


int create_music_from_line(music_t* music, GArray* parsed_files){
    if(!music){
        return 1;
    }
    if(parsed_files == NULL){
        return 1;
    }
    if(parsed_files->len == 0){
        return 1;
    }
    char* id = g_array_index(parsed_files, char*, 0);
    char* title = g_array_index(parsed_files, char*, 1);
    char* artists_ids = g_array_index(parsed_files, char*, 2);
    char* album_id = g_array_index(parsed_files, char*, 3);
    char* duration = g_array_index(parsed_files, char*, 4);
    char* genre = g_array_index(parsed_files, char*, 5);
    char* yearS = g_array_index(parsed_files, char*, 6);

    if(set_music_id(music, id)){return 1;}
    if(set_music_title(music, title)){return 1;}
    if(set_music_artist_ids_from_string(music, artists_ids)){return 1;}
    if(set_music_album_id(music, album_id)){return 1;}
    if(set_music_duration_from_string(music, duration)){return 1;}
    if(set_music_genre(music, genre)){return 1;}
    int year = atoi(yearS);
    if(set_music_year(music, year)){return 1;}

    return 0;
}


void __free_music__(void *music) {
    music_t* m = (music_t*) music; 

    free(m->id);
    free(m->title);
    free(m->album_id);
    duration_free(m->duration);

 
    for (guint i = 0; i < m->artist_ids->len; i++) {
        free(g_array_index(m->artist_ids, char*, i));  
    }
    g_array_free(m->artist_ids, TRUE);

}