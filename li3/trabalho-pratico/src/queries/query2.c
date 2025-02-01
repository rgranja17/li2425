#include "queries/query2.h"

struct artistduration {
    char* id;
    char* Name;
    int type;
    int DiscographyDuration;
    char* country;
};

char* query2 (Database_manager* db, const int N, char* country){
    if (N <= 0) {
        return strdup("\n");
    }
    
    GHashTable* artist_table = __get_artists_hashTable(getArtistManager(db));
    if (!artist_table) {
        return strdup("\n");
    }

    GArray* sorted_discography = g_array_new(FALSE, FALSE, sizeof(artistduration));
    GHashTableIter artist_iter;
    gpointer artist_key, artist_value;

    g_hash_table_iter_init(&artist_iter, artist_table);

    while (g_hash_table_iter_next(&artist_iter, &artist_key, &artist_value)) {
        char* country_aux = get_artist_country(artist_value);
        if (country != NULL && strcmp(country_aux, country) != 0){
            free(country_aux);
            continue;
        } 
        
        artistduration artistDuration_entry;
        artistDuration_entry.id = get_artist_id(artist_value);
        artistDuration_entry.Name = get_artist_name(artist_value);
        artistDuration_entry.type = get_artist_type(artist_value);
        artistDuration_entry.DiscographyDuration = get_artist_total_discography_length(artist_value);
        artistDuration_entry.country = get_artist_country(artist_value);
        
        free(country_aux);
        g_array_append_val(sorted_discography, artistDuration_entry);
    }

    g_array_sort(sorted_discography, compare_discography_duration);

    char* result = malloc(sizeof(char) * MAX_RESULT_QUERY2);
    if (!result) {
        perror("Error allocating memory for result");
        for (guint i = 0; i < sorted_discography->len; i++) {
            artistduration* entry = &g_array_index(sorted_discography, artistduration, i);
            free(entry->id);
            free(entry->Name);
            free(entry->country);
        }
        g_array_free(sorted_discography, TRUE);
        exit(1);
    }
    result[0] = '\0';

    for (guint i = 0; (int)i < N && i < sorted_discography->len; i++) {
        artistduration entry = g_array_index(sorted_discography, artistduration, i);
        char temp[128];
        char type_str[11];
        if (entry.type == GROUP) {
            strcpy(type_str, "group");
        } else if (entry.type == INDIVIDUAL) {
            strcpy(type_str, "individual");
        }
        duration_t* d = convert_seconds_to_duration(entry.DiscographyDuration);
        char* duration = duration_to_string(d);
        snprintf(temp, sizeof(temp), "%s;%s;%s;%s\n", entry.Name, type_str, duration, entry.country);
        strncat(result, temp, MAX_RESULT_QUERY2 - strlen(result) - 1);
        free(duration);
        duration_free(d);
    }

    for (guint i = 0; i < sorted_discography->len; i++) {
        artistduration* entry = &g_array_index(sorted_discography, artistduration, i);
        free(entry->id);
        free(entry->Name);
        free(entry->country);
    }
    g_array_free(sorted_discography, TRUE);

    return result;
}
