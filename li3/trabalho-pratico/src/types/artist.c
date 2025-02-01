#include "types/artist.h"

struct artist_t {
    char *id;
    char *name;
    float recipe_per_stream;
    GArray *id_constituent; 
    GArray *group;
    char *country;
    type_t type : 1;

    int total_discography_length; // in seconds
    int num_albums_individual;
    int num_reproducoes;
};

size_t artist_sizeof(void) {return sizeof(artist_t);}

guint artist_get_array_size(artist_t* artist){
    return artist->id_constituent->len;
}

int validate_artist(artist_t *artist) {
   
   guint len = artist_get_array_size(artist);

   if(!artist) {
      return 1;
   }
   int type = get_artist_type(artist);
   if(type == INDIVIDUAL) {
      if(len > 0) {
         return 1; // artista individual nao deve ter elementos ([])
      }
   } else if (type == GROUP){
      if(len == 0) {
         return 1; // artista grupo deve ter ids, pelo menos 1 
      }
   }
   return 0;
}

artist_t* artist_clone(artist_t* artist, pool_t* artist_allocator) {
    if (!artist || !artist_allocator) return NULL;


    artist_t* new_artist = artist_alloc(artist_allocator);
    if (!new_artist) return NULL;

    new_artist->id = strdup(artist->id);
    new_artist->name = strdup(artist->name);
    new_artist->country = strdup(artist->country);
    new_artist->total_discography_length = artist->total_discography_length;
    new_artist->recipe_per_stream = artist->recipe_per_stream;
    new_artist->type = artist->type;
    new_artist->num_albums_individual = artist->num_albums_individual;
    new_artist->num_reproducoes = artist->num_reproducoes;

    if (!new_artist->id || !new_artist->name || !new_artist->country) {
        free(new_artist->id);
        free(new_artist->name);
        free(new_artist->country);
        free(new_artist);
        return NULL;
    }

    new_artist->id_constituent = g_array_new(FALSE, FALSE, sizeof(char*));
    if (artist->id_constituent != NULL) {
        for (guint i = 0; i < artist->id_constituent->len; i++) {
            char *id = g_array_index(artist->id_constituent, char*, i);
            char *id_copy = strdup(id);
            if (id_copy) {
                g_array_append_val(new_artist->id_constituent, id_copy);
            }
        }
    }

    new_artist->group = g_array_new(FALSE, FALSE, sizeof(char*));
    if (artist->group != NULL) {
        for (guint i = 0; i < artist->group->len; i++) {
            char *id = g_array_index(artist->group, char*, i);
            char *id_copy = strdup(id);
            if (id_copy) {
                g_array_append_val(new_artist->group, id_copy);
            }
        }
    }
    return new_artist;
}



artist_t* artist_alloc(pool_t* artist_allocator){
    artist_t* new_artist = pool_alloc_item(artist_allocator);
    if(!new_artist)
        return NULL;
    new_artist->id_constituent = g_array_new(FALSE,FALSE,sizeof(char*));
    new_artist->group = g_array_new(FALSE, FALSE, sizeof(char*));
    new_artist->total_discography_length = 0;
    new_artist->num_albums_individual = 0;
    new_artist->num_reproducoes = 0;
    return new_artist;
}

char* get_artist_id(artist_t *artist_t) {
    return strdup(artist_t->id);
}

int set_artist_id(artist_t* artist, char *id) {
    if (!artist || !id) return 1;
    if(strlen(id) == 0) return 1;  

    if(artist->id){
        free(artist->id);
    }
    artist->id = strdup(id);
    return 0;
}

char* get_artist_name(artist_t *artist_t) {
    return strdup(artist_t->name);
}

int set_artist_name(artist_t* artist, char *name) {
    if(!artist || !name) return 1;
    if(strlen(name) == 0) return 1; 

    if(artist->name){
        free(artist->name);
    }
    artist->name = strdup(name);
    return 0;
}

float get_artist_recipe(artist_t *artist_t) {
    return artist_t->recipe_per_stream;
}

int set_artist_recipe(artist_t* artist, float recipe_per_stream) {
    if(!artist) return 1;

    if(recipe_per_stream <= 0) return 1;

    artist->recipe_per_stream = recipe_per_stream;
    return 0;
}
GArray* get_artist_id_constituents(artist_t *artist) {
    GArray *constituents_copy = g_array_new(FALSE, FALSE, sizeof(char*));
    if(artist->id_constituent->len == 0) return constituents_copy;
    for (guint i = 0; i < artist->id_constituent->len; i++) {
        char *id = g_array_index(artist->id_constituent, char*, i);
        char *id_copy = strdup(id);
        if (id_copy == NULL) {
            return NULL;  
        }
        g_array_append_val(constituents_copy, id_copy); 
    }
    return constituents_copy;
}

int create_artist_from_line(artist_t* artist, GArray* parsed_files){
    if(!artist){
        return 1;
    }
    if(parsed_files == NULL){
        return 1;
    }
    if(parsed_files->len == 0){
        return 1;
    }
    char* id = g_array_index(parsed_files, char*, 0);
    char* name = g_array_index(parsed_files, char*, 1);
    char* recipeS = g_array_index(parsed_files, char*, 3);
    char* id_constituents = g_array_index(parsed_files, char*, 4);
    char* country = g_array_index(parsed_files, char*, 5);
    char* type = g_array_index(parsed_files, char*, 6);

    if(set_artist_id(artist, id)){return 1;}
    if(set_artist_name(artist, name)){return 1;}
    float recipe = atof(recipeS);
    if(set_artist_recipe(artist, recipe)){return 1;}
    if(set_artist_id_constituents_from_string(artist, id_constituents)){return 1;}
    if(set_artist_country(artist, country)){return 1;}
    if(set_artist_type(artist, type)){return 1;}
    restart_discography_duration(artist);
    restart_artist_num_albums(artist);
    restart_artist_num_reproducoes(artist);

    return validate_artist(artist);
}

GArray* get_artist_group(artist_t* artist){
    GArray *group_cpy = g_array_new(FALSE, FALSE, sizeof(char*));
    if(artist->group->len == 0) return group_cpy;
    for (guint i = 0; i < artist->group->len; i++) {
        char *id = g_array_index(artist->group, char*, i);
        char *id_copy = strdup(id);
        if (id_copy == NULL) {
            return NULL;  
        }
        g_array_append_val(group_cpy, id_copy); 
    }
    return group_cpy;
}

void add_artist_participation(artist_t* artist, char* group_id){
    for(guint i = 0; i < artist->group->len; i++){
        if(strcmp(group_id, g_array_index(artist->group, char*, i)) == 0) return;
    }
    char* id = strdup(group_id);
    g_array_append_val(artist->group, id); 
}

int set_artist_id_constituents_from_string(artist_t* artist, char* str) {
    if (!artist || !str) return 1;

    for (guint i = 0; i < artist->id_constituent->len; i++) {
        free(g_array_index(artist->id_constituent, char*, i));
    }
    g_array_free(artist->id_constituent, TRUE);

    artist->id_constituent = string_to_array1(str);
    if(!artist->id_constituent){
        artist->id_constituent = g_array_new(FALSE,FALSE,sizeof(char*));
        return 1;
    }

    return 0;
}

char * get_artist_country(artist_t* artist_t) {
   return strdup(artist_t->country);
}

int set_artist_country(artist_t* artist, char* country) {
    if(!artist || !country) return 1;
    if(strlen(country) == 0) return 1;

    if(artist->country){
        free(artist->country);
    }
    artist->country = strdup(country);
    return 0;
}
int get_artist_type(artist_t* artist_t) {
   return artist_t->type;
}

char* get_artist_type_string(artist_t* artist_t) {
    if(artist_t->type == INDIVIDUAL) return strdup("individual");
    return strdup("group");
}

int set_artist_type(artist_t* artist, char* type) {
    if(!artist || !type) return 1;
    if(strlen(type) == 0) return 1;

    int len = strlen(type);

    for(int i = 0; i < len; i++){
        type[i] = tolower(type[i]);
    }

    if(strcmp(type,"individual") == 0) {artist->type = INDIVIDUAL;return 0;}
    if(strcmp(type,"group") == 0) {artist->type = GROUP;return 0;}

    return 1;
}

void increment_discography_duration(artist_t* artist, duration_t* music_duration) {
    artist->total_discography_length += duration_to_seconds(music_duration);
}

void restart_discography_duration(artist_t* artist){
    artist->total_discography_length = 0;
}

int get_artist_total_discography_length(artist_t* artist){
    return artist->total_discography_length;
}

void increment_artist_num_albums(artist_t* artist) {
    artist->num_albums_individual++;
}

void restart_artist_num_albums(artist_t* artist){
    artist->num_albums_individual = 0;
}

int get_artist_num_albums(artist_t* artist){
    return artist->num_albums_individual;
}

void increment_artist_num_reproducoes(artist_t* artist) {
    artist->num_reproducoes++;
}

void restart_artist_num_reproducoes(artist_t* artist){
    artist->num_reproducoes = 0;
}

int get_artist_num_reproducoes(artist_t* artist){
    return artist->num_reproducoes;
}


void __free_artist__(void* a) {
    artist_t* artist = (artist_t*)a;
    free(artist->id);
    free(artist->name);
    free(artist->country);

    for (guint i = 0; i < artist->id_constituent->len; i++) {
        free(g_array_index(artist->id_constituent, char*, i));
    }

    g_array_free(artist->id_constituent, TRUE);

    for (guint i = 0; i < artist->group->len; i++) {
        free(g_array_index(artist->group, char*, i));
    }

    g_array_free(artist->group, TRUE);
}