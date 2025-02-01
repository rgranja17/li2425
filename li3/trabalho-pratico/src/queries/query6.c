#include "queries/query6.h"

struct favourite_artists_t {
    char* artist_id;
    int n_musics;
    int listening_time;
};

struct favourite_album_t{
    char *album_id;
    int listening_time;
};

favourite_album_t* get_or_create_album(GArray *falbums_array,GHashTable *falbums_table,const char *album_id) {
    favourite_album_t *falbum = g_hash_table_lookup(falbums_table, album_id);
    if (falbum == NULL) {
        falbum = malloc(sizeof(favourite_album_t));
        falbum->album_id = strdup(album_id);
        falbum->listening_time = 0;

        g_array_append_val(falbums_array, falbum);

        g_hash_table_insert(falbums_table, falbum->album_id, falbum);
    }
    return falbum;
}

int compare_history_by_music_id(gpointer a, gpointer b) {
    history_t *history_a = (history_t *) a;
    history_t *history_b = (history_t *) b;

    char* music_id_a = get_history_music_id(history_a);
    char* music_id_b = get_history_music_id(history_b);
    int result = strcmp(music_id_a, music_id_b);

    free(music_id_a);
    free(music_id_b);

    return result;
}

favourite_artists_t* get_or_create_fartist(GArray *fartists_array,GHashTable *fartists_table,const char *artist_id) {
    favourite_artists_t *fartist = g_hash_table_lookup(fartists_table, artist_id);
    if (fartist == NULL) {
        fartist = malloc(sizeof(favourite_artists_t));
        fartist->artist_id = strdup(artist_id);
        fartist->n_musics = 0;
        fartist->listening_time = 0;

        g_array_append_val(fartists_array, fartist);

        g_hash_table_insert(fartists_table, fartist->artist_id, fartist);
    }
    return fartist;
}

int compare_fartists_by_listening_time(gpointer a, gpointer b) {
    favourite_artists_t *fartist_a = *(favourite_artists_t **)a;
    favourite_artists_t *fartist_b = *(favourite_artists_t **)b;

    if (fartist_a->listening_time > fartist_b->listening_time) {
        return -1;
    } else if (fartist_a->listening_time < fartist_b->listening_time) {
        return 1;
    }

    return strcmp(fartist_a->artist_id, fartist_b->artist_id);
}

int compare_favourite_albums(gpointer a, gpointer b) {
    favourite_album_t *album_a = *(favourite_album_t **)a;
    favourite_album_t *album_b = *(favourite_album_t **)b;

    if (album_a->listening_time != album_b->listening_time) {
        return album_b->listening_time - album_a->listening_time;
    }

    return strcmp(album_a->album_id, album_b->album_id);
}

int getGenreId(music_t* music){
    switch (get_music_genre(music)) {
        case BLUES:
            return 0;
        case CLASSICAL:
            return 1;
        case COUNTRY:
            return 2;
        case ELECTRONIC:
            return 3;
        case HIPHOP:
            return 4;
        case JAZZ:
            return 5;
        case METAL:
            return 6;
        case POP:
            return 7;
        case REGGAE:
            return 8;
        case ROCK:
            return 9;
        default:
            return -1;
    }
}

char* get_genre_to_string(int genre){
    switch (genre) {
        case BLUES:
            return strdup("Blues");
        case CLASSICAL:
            return strdup("Classical");
        case COUNTRY:
            return strdup("Country");
        case ELECTRONIC:
            return strdup("Electronic");
        case HIPHOP:
            return strdup("Hip Hop");
        case JAZZ:
            return strdup("Jazz");
        case METAL:
            return strdup("Metal");
        case POP:
            return strdup("Pop");
        case REGGAE:
            return strdup("Reggae");
        case ROCK:
            return strdup("Rock");
        default:
            return strdup("");
    }
}

int find_max (int array[], int size) {
    if (size <= 0) return -1;

    int max_value = array[0];
    int index = 0;

    for (int i = 1; i < size; i++) {
        if (array[i] > max_value) {
            max_value = array[i];
            index = i;
        }
    }

    return index;
}

char* query6(Database_manager* db, char* user_id, int year, int N){
    MusicManager* musicManager = getMusicManager(db);
    int hour[24] = {0};
    int listening_time = 0;
    int n_musics = 0;
    int genre_listening_time[10] = {0};
    int musics_by_day[366] = {0};
    GArray* fartists_array = g_array_new(FALSE, FALSE, sizeof(favourite_artists_t*));
    GHashTable* fartists_table = g_hash_table_new_full(g_str_hash, g_str_equal, free, free);

    GArray* falbums_array = g_array_new(FALSE, FALSE, sizeof(favourite_album_t*));
    GHashTable* falbums_table = g_hash_table_new_full(g_str_hash, g_str_equal, free, free);

    user_t* user = __get_user__(getUserManager(db), user_id);
    if(!user) return strdup("\n");
    GArray* user_history = get_user_history(user,database_get_history_allocator(db));
    g_array_sort(user_history, compare_history_by_music_id);

    char* last_music = NULL;
    last_music = strdup("");

    for(guint i = 0; i < user_history->len; i++){
        history_t* history = g_array_index(user_history,history_t*,i);
        timestamp_t* timestamp = get_history_timestamp(history);
        if (timestamp_get_year(timestamp) != year){
            __free_timestamp__(timestamp);
            continue;
        }
        char* music_id = get_history_music_id(history);
        music_t* music = get_music(musicManager, music_id);
        GArray* artist_ids = get_music_artist_ids(music);
        char* album_id = get_music_album_id(music);

        if(strcmp(last_music,music_id) == 0){
            for(guint j = 0; j < artist_ids->len; j++){
                char* artist_id = g_array_index(artist_ids,char*,j);
                favourite_artists_t* fartist = get_or_create_fartist(fartists_array, fartists_table, artist_id);
                duration_t* duration = get_history_duration(history);
                int time = duration_to_seconds(duration);
                fartist->listening_time = fartist->listening_time + time;
                listening_time = listening_time + time;
                hour[(int) timestamp_get_hour(timestamp)] += time;
                musics_by_day[timestamp_get_day_of_year(timestamp)-1]++;
                genre_listening_time[getGenreId(music)] += time;
                favourite_album_t* falbum = get_or_create_album(falbums_array, falbums_table, album_id);
                falbum->listening_time += time;
                free(duration);
            }
        } else {
            for(guint j = 0; j < artist_ids->len; j++){
                char* artist_id = g_array_index(artist_ids,char*,j);
                favourite_artists_t* fartist = get_or_create_fartist(fartists_array, fartists_table, artist_id);
                duration_t* duration = get_history_duration(history);
                fartist->n_musics++;
                n_musics++;
                int time = duration_to_seconds(duration);
                fartist->listening_time = fartist->listening_time + time;
                listening_time = listening_time + time;
                hour[(int) timestamp_get_hour(timestamp)] += time;
                musics_by_day[timestamp_get_day_of_year(timestamp)-1]++;
                genre_listening_time[getGenreId(music)] += time;
                favourite_album_t* falbum = get_or_create_album(falbums_array, falbums_table, album_id);
                falbum->listening_time += time;
                free(duration);
            }
        }

        for (guint i = 0; i < artist_ids->len; i++) {
            free(g_array_index(artist_ids, char*, i));  
        }
        g_array_free(artist_ids, TRUE);

        __free_timestamp__(timestamp);
        free(last_music);
        last_music = music_id;
        free(album_id);
    }

    g_array_sort(fartists_array, compare_fartists_by_listening_time);

    if (fartists_array->len == 0){
        return strdup("\n");
    }
    g_array_sort(falbums_array, compare_favourite_albums);
    char* favourite_album = strdup(g_array_index(falbums_array, favourite_album_t*, 0)->album_id);

    char* fartistT = strdup (g_array_index(fartists_array, favourite_artists_t*, 0)->artist_id);


    char* output = calloc(MAX_RESULT_QUERY6, sizeof(char));
    char temp[512];
    duration_t* duration = convert_seconds_to_duration(listening_time);
    char* day = day_of_year_to_timestamp(find_max(musics_by_day,366)+1, year);
    char* genre = genre_to_string(find_max(genre_listening_time, 10));
    char* duration_string = duration_to_string(duration);

    snprintf(temp, sizeof(temp), "%s;%d;%s;%s;%s;%s;%02d\n",duration_string, n_musics, fartistT, day, genre, favourite_album, find_max(hour, 24));
    strncat(output, temp, MAX_RESULT_QUERY6 - strlen(output) - 1);
    if (N>0){
        for (int i = 0; i<N && i < fartists_array->len ; i++){
            free(duration);
            free(duration_string);
            favourite_artists_t *fartist = g_array_index(fartists_array, favourite_artists_t*, i);
            duration = convert_seconds_to_duration(fartist->listening_time);
            duration_string = duration_to_string(duration);
            snprintf(temp, sizeof(temp), "%s;%d;%s\n",fartist->artist_id, fartist->n_musics, duration_string);
            strncat(output, temp, MAX_RESULT_QUERY6 - strlen(output) - 1);
        }
    }
    free(duration);
    //free(day);
    //free(genre);
    free(favourite_album);
    free(duration_string);

    for (guint i = 0; i < fartists_array->len; i++) {
        favourite_artists_t *fartist = g_array_index(fartists_array, favourite_artists_t*, i);
        if(fartist){
            free(fartist->artist_id);
            free(fartist);
        }
    }
    g_array_free(fartists_array, TRUE);
    //g_hash_table_destroy(fartists_table);

    for (guint i = 0; i < falbums_array->len; i++) {
        favourite_album_t *falbum = g_array_index(falbums_array, favourite_album_t*, i);
        if(falbum){
            free(falbum->album_id);
            free(falbum);
        }
    }
    g_array_free(falbums_array, TRUE);
    //g_hash_table_destroy(falbums_table);


    return output;
    
}