#include "queries/query5.h"

char* query5(Database_manager* db, const char* user_id, int num_recommendations) {
    if (!db || !user_id || num_recommendations <= 0) {
        return strdup("\n");
    }

    UserManager* user_manager = getUserManager(db);
    GHashTable* user_table = get_user_table(user_manager);

    if (!g_hash_table_contains(user_table, user_id)) {
        return strdup("\n");
    }

    int num_users = g_hash_table_size(user_table);
    int num_genres = 10;

    int** genre_matrix = calloc(num_users, sizeof(int*));
    char** user_ids = calloc(num_users, sizeof(char*));
    char** genre_names = calloc(num_genres, sizeof(char*));

    if (!genre_matrix || !user_ids || !genre_names) {
        perror("Error allocating memory for matrix or arrays");
        exit(1);
    }

    const char* genre_array[] = {
        "Blues", "Classical", "Country", "Electronic", "Hip Hop",
        "Jazz", "Metal", "Pop", "Reggae", "Rock"
    };
    for (int i = 0; i < num_genres; i++) {
        genre_names[i] = strdup(genre_array[i]);
    }


    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, user_table);

    int user_index = 0;
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        user_t* user = (user_t*)value;

        user_ids[user_index] = get_user_name(user);

        genre_matrix[user_index] = calloc(num_genres, sizeof(int));

        GArray* user_history = get_user_history(user, get_history_allocator(user_manager));
        if (user_history) {
            for (guint i = 0; i < user_history->len; i++) {
                history_t* history = g_array_index(user_history, history_t*, i);
                if (history) {
                    char* music_id = get_history_music_id(history);
                    music_t* music = get_music(getMusicManager(db), music_id);

                    if (music) {
                        int genre = get_music_genre(music);
                        if (genre >= 0 && genre < num_genres) {
                            genre_matrix[user_index][genre]++;
                        }
                    }
                    free(music_id);
                }
            }

            for(guint i = 0; i < user_history->len; i++){
                __free_history__(g_array_index(user_history, history_t*, i));
            }
            g_array_free(user_history, TRUE);
        }

        user_index++;
    }

    char** recommended_users = recomendaUtilizadores(
        (char*)user_id, genre_matrix, user_ids, genre_names, num_users, num_genres, num_recommendations
    );
    char* result = malloc(256 * num_recommendations);
    if (!result) {
        perror("Error allocating memory for result");
        exit(1);
    }
    result[0] = '\0';

    for (int i = 0; i < num_recommendations; i++) {
        if (recommended_users[i]) {
            strcat(result, recommended_users[i]);
            strcat(result, "\n");
            free(recommended_users[i]);
        }
    }
    free(recommended_users);
    
    for (int i = 0; i < num_users; i++) {
        free(genre_matrix[i]);
    }
    free(genre_matrix);
    free(user_ids);
    
    for (int i = 0; i < num_genres; i++) {
        free(genre_names[i]);
    }
    free(genre_names);
    
    return result;
}
