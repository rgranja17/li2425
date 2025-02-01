#include "queries/query1.h"

char* query1(Database_manager* db, const char* id) {
    if(id[0] == 'U'){

        UserManager* user_manager = getUserManager(db);
        user_t *user = __get_user__(user_manager, id);

        if (user == NULL) {
            return strdup("\n");
        }

        char* result = malloc(sizeof(char) * MAX_RESULT_QUERY1);
        if (result == NULL) {
            perror("Error allocating memory for result");
            exit(1);
        }

        int age = get_user_age(user);
        char* email = get_user_email(user);
        char* first_name = get_user_first_name(user);
        char* last_name = get_user_last_name(user);
        char* country = get_user_country(user);

        snprintf(result, MAX_RESULT_QUERY1, "%s;%s;%s;%d;%s\n", email, first_name, last_name, age, country);

        free(email);
        free(first_name);
        free(last_name);
        free(country);

        return result;

    } else if (id[0] == 'A') {
        ArtistManager* artist_manager = getArtistManager(db);
        artist_t *artist = __get_artist__(artist_manager, id);

        if (artist == NULL) {
            return strdup("\n");
        }

        char* result = malloc(sizeof(char) * MAX_RESULT_QUERY1);
        if (result == NULL) {
            perror("Error allocating memory for result");
            exit(1);
        }

        char* name = get_artist_name(artist);
        char* type = get_artist_type_string(artist);
        char* country = get_artist_country(artist);
        int num_albums_individual = get_artist_num_albums(artist);
        float total_recipe = get_artist_total_recipe(db, artist);

        snprintf(result, MAX_RESULT_QUERY1, "%s;%s;%s;%d;%.2f\n", name, type, country, num_albums_individual, total_recipe);

        free(name);
        free(type);
        free(country);

        return result;
    }

    return strdup("\n");
}