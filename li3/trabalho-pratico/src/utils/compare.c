#include "utils/compare.h"

struct music_genre {
    genre_t genre;
    int likes_count;
};

struct artistduration {
    char* id;
    char* Name;
    int type;
    int DiscographyDuration;
    char* country;
};

const char* genre_to_string(genre_t genre) {
    switch (genre) {
        case BLUES: return "Blues";
        case CLASSICAL: return "Classical";
        case COUNTRY: return "Country";
        case ELECTRONIC: return "Electronic";
        case HIPHOP: return "Hip Hop";
        case JAZZ: return "Jazz";
        case METAL: return "Metal";
        case POP: return "Pop";
        case REGGAE: return "Reggae";
        case ROCK: return "Rock";
        default: return "Unknown";
    }
}

GArray* music_genre_alloc_array(void){
     GArray* music_genres = g_array_new(FALSE, FALSE, music_genre_sizeof());
    
    for(guint i = 0; i < 10; i++){
        music_genre_t* music_genre_struct = music_genre_alloc();
        switch (i) {
            case 0: set_music_genre_type(music_genre_struct, BLUES); break;
            case 1: set_music_genre_type(music_genre_struct, CLASSICAL); break;
            case 2: set_music_genre_type(music_genre_struct, COUNTRY); break;
            case 3: set_music_genre_type(music_genre_struct, ELECTRONIC); break;
            case 4: set_music_genre_type(music_genre_struct, HIPHOP); break;
            case 5: set_music_genre_type(music_genre_struct, JAZZ); break;
            case 6: set_music_genre_type(music_genre_struct, METAL); break;
            case 7: set_music_genre_type(music_genre_struct, POP); break;
            case 8: set_music_genre_type(music_genre_struct, REGGAE); break;
            case 9: set_music_genre_type(music_genre_struct, ROCK); break;
        }
        g_array_append_val(music_genres, music_genre_struct);
    }

    return music_genres;
}

void sum_like_count(GArray* origin, GArray* to_sum){
    for(guint i = 0; i < 10; i++){
        music_genre_t* aux1 = g_array_index(origin, music_genre_t*, i);
        music_genre_t* aux2 = g_array_index(to_sum, music_genre_t*, i);

        aux1->likes_count += aux2->likes_count;
    }
}

int compare_genre_likes(const void* a, const void* b) {
    const music_genre_t* genre_a = *(const music_genre_t**)a;
    const music_genre_t* genre_b = *(const music_genre_t**)b;

    if (genre_b->likes_count != genre_a->likes_count) {
        return genre_b->likes_count - genre_a->likes_count;
    }

    return (int)genre_a->genre - (int)genre_b->genre;
}

music_genre_t* music_genre_alloc(void){
    music_genre_t* music =  malloc(sizeof(music_genre_t));
    music->likes_count = 0;
    return music;
}

int get_music_genre_type(music_genre_t* genre){
    return genre->genre;
}

void set_music_genre_type(music_genre_t* genre, int type){
    genre->genre = type;
}

int get_music_likes(music_genre_t* genre){
    return genre->likes_count;
}

void increase_music_likes(music_genre_t* genre){
    genre->likes_count++;
}

size_t music_genre_sizeof(void) {
    return sizeof(music_genre_t);
}

void music_genre_free(music_genre_t* genre){
    if(genre){
        free(genre);
    }
}

int compare_discography_duration (const void* a, const void* b){
    const artistduration* discography_a = (const artistduration*) a;
    const artistduration* discography_b = (const artistduration*) b;

    if (discography_a->DiscographyDuration != discography_b->DiscographyDuration) {
        return discography_b->DiscographyDuration - discography_a->DiscographyDuration; 
    }

    return g_strcmp0(discography_a->Name, discography_b->Name);
}