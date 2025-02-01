#include "queries/query3.h"

char* query3(Database_manager* db, int min_age, int max_age){
    if(max_age - min_age < 0 || (max_age == 0 && min_age == 0)){
        return strdup("\n");
    }

    UserManager* user_manager = getUserManager(db);
    GArray* music_genres = music_genre_alloc_array();
    GArray* likes_by_age = get_user_likes_by_age(user_manager);
    if(max_age > (int)likes_by_age->len){
        max_age = likes_by_age->len - 1;
    }

    int same_age = 0;

    if(max_age == min_age){
        GArray* likes_by_single_age = g_array_index(likes_by_age, GArray*, max_age);
        sum_like_count(music_genres,likes_by_single_age);
        same_age = 1;
    }

    if(same_age == 0){
        for(guint i = max_age; (int)i >= min_age; i--){
            GArray* likes_by_single_age = g_array_index(likes_by_age, GArray*, i);
            sum_like_count(music_genres,likes_by_single_age);
        }
    }


    char* result = malloc(sizeof(char) * MAX_RESULT_QUERY3);
    if (!result) {
        for(guint i = 0; i < music_genres->len; i++){
            music_genre_free(g_array_index(music_genres, music_genre_t*, i));
        }
        g_array_free(music_genres, TRUE);
        free(result);
        return NULL;
    }
    
    result[0] = '\0';

    g_array_sort(music_genres, compare_genre_likes);

    int num_likes = 0; 
    for(guint i = 0; i < 10; i++){
        music_genre_t* genre = g_array_index(music_genres, music_genre_t*, i);
        num_likes += get_music_likes(genre);
    }

    if(num_likes == 0){
        for(guint j = 0; j < music_genres->len; j++){
            music_genre_t* aux = g_array_index(music_genres, music_genre_t*, j);
            music_genre_free(aux);
        }
        g_array_free(music_genres, TRUE);
        free(result);
        return strdup("\n");
    } 
    
    for (guint i = 0; i < music_genres->len; i++) {
        music_genre_t* entry = g_array_index(music_genres, music_genre_t*, i);
        char temp[128];
        int likes = get_music_likes(entry);
        snprintf(temp, sizeof(temp), "%s;%d\n", genre_to_string(get_music_genre_type(entry)), likes);
        strncat(result, temp, MAX_RESULT_QUERY3 - strlen(result) - 1);
    }

    for(guint j = 0; j < music_genres->len; j++){
        music_genre_t* aux = g_array_index(music_genres, music_genre_t*, j);
        music_genre_free(aux);
    }
    g_array_free(music_genres, TRUE);
    
    return result;
}