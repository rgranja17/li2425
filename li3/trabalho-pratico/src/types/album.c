#include "types/album.h"

struct album{
    char* id;
    char* title;
    GArray* artist_ids;
    uint16_t year;
    //opcional: producers
};

size_t album_sizeof(void) {return sizeof(album_t);}

album_t* album_alloc(pool_t* album_allocator){
    album_t* new_album = pool_alloc_item(album_allocator);
    if(!new_album) 
        return NULL;
    new_album->artist_ids = g_array_new(FALSE, FALSE, sizeof(char*));
    if(!new_album->artist_ids){
        free(new_album);
        return NULL;
    }
    return new_album;
}

album_t* album_clone(album_t* album, pool_t* album_allocator){
    if (!album || !album_allocator) return NULL;
    album_t* new_album = album_alloc(album_allocator);
    if(!new_album)
        return NULL;

    if(!new_album->artist_ids){
        free(new_album);
        return NULL;
    }

    if(album->artist_ids->len > 0) {
        for(guint i = 0; i < album->artist_ids->len; i++){
            char* id = g_array_index (album->artist_ids, char* , i);
            char* id_cpy = strdup(id);
            g_array_append_val(new_album->artist_ids, id_cpy);
        }
    }
    new_album->year = get_album_year(album);
    new_album->id = get_album_id(album);
    new_album->title = get_album_title(album);

    return new_album;
}

char* get_album_id(album_t* album){
    return strdup(album->id);
}

char* get_album_title(album_t* album){
    return strdup(album->title);
}

GArray* get_album_artist_ids(album_t* album){
    GArray* artist_list = g_array_new(FALSE, FALSE, sizeof(char*));
    for(guint i = 0; i < album->artist_ids->len; i++){
        char* id = g_array_index(album->artist_ids, char*, i);
        char* id_cpy = strdup(id);
        g_array_append_val(artist_list, id_cpy);
    }
    return artist_list;
}

int get_album_num_artists(album_t* album){
    return album->artist_ids->len;
}

uint16_t get_album_year(album_t* album){
    return album->year;
}

int set_album_id(album_t* album, char* id){
    if(!album || !id) return 1;
    if(strlen(id) == 0) return 1;

    if(album->id){free(album->id);}
    album->id = strdup(id);
    return 0;
}
int set_album_title(album_t* album, char* title){
    if(!album || !title) return 1;
    if(!strlen(title)) return 1;

    if(album->title){free(album->title);}
    album->title = strdup(title);
    return 0;
}

int set_album_artists_from_string(album_t* album, char* artist_list){
    if(!album || !artist_list) return 1;

    for(guint i = 0; i < album->artist_ids->len; i++){
        free(g_array_index(album->artist_ids, char*, i));
    }
    g_array_free(album->artist_ids, TRUE);

    GArray* list = string_to_array1(artist_list);
    if(!list){
        album->artist_ids = g_array_new(FALSE, FALSE, sizeof(char*));
        return 1;
    }
    album->artist_ids = list;

    return 0;
}

int set_album_artists(album_t* album, GArray* artists){
    if(!album) return 1;
    if(artists == NULL) return 1;

    GArray* artist_list = g_array_new(FALSE, FALSE, sizeof(char*));
    for(guint i = 0; i<artists->len;i++){
        char* id = g_array_index(artists, char*, i);
        char* id_cpy = strdup(id);
        g_array_append_val(artist_list, id_cpy);
    }

    if(artist_list == NULL) return 1;

    for (guint i = 0; i < album->artist_ids->len; i++) {
        free(g_array_index(album->artist_ids, char*, i));
    }

    g_array_free(album->artist_ids, TRUE);
    album->artist_ids = artist_list;

    return 0;
}

int set_album_year(album_t* album, int year){
    album->year = year;
    return 0;
}

int create_album_from_line(album_t* album, GArray* parsed_files){
    if(parsed_files == NULL){
        return 1;
    }
    if(parsed_files->len == 0){
        return 1;
    }
    
    char* id = g_array_index(parsed_files, char*, 0);
    char* title = g_array_index(parsed_files, char*, 1);
    char* artists_id = g_array_index(parsed_files, char*, 2);
    char* yearS = g_array_index(parsed_files, char*, 3);


    if(set_album_id(album, id)){return 1;}
    if(set_album_title(album, title)){return 1;}
    if(set_album_artists_from_string(album, artists_id)){return 1;}
    int year = atoi(yearS);
    if(set_album_year(album, year)){return 1;}
    

    return 0;
}

void __free_album__(void *a) {
    if (!a) return;

    album_t* album = (album_t*)a;

    free(album->id);
    free(album->title);

    for (guint i = 0; i < album->artist_ids->len; i++) {
        free(g_array_index(album->artist_ids, char*, i));  
    }
    g_array_free(album->artist_ids, TRUE);
}