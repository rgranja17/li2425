#include "utils/string.h"

// working 100%!!
GArray* string_to_array(char* str) {
    if (!str) return NULL;

    int len = strlen(str);
    GArray* new_array = g_array_new(FALSE,FALSE,sizeof(char*));

    if(str[1] != '[' || str[len - 2] != ']') {
        g_array_free(new_array, TRUE);
        return NULL;
    }
    // removing " and []
    str += 2;
    len -= 2;
    str[len - 2] = '\0';

    char* id;
    
    while ((id = strsep(&str, ",")) != NULL) {

        while (*id == ' ' || *id == '\'') id++;

        int id_len = strlen(id);
        if (id_len > 0 && id[id_len - 1] == '\'') {
            id[id_len - 1] = '\0';
        }

        if (strlen(id) > 0) {
            char* id_cpy = strdup(id);
            g_array_append_val(new_array, id_cpy);
        }
    }


    return new_array;
}

GArray* string_to_array1(char* str) {
    if (!str) return NULL;

    int len = strlen(str);
    GArray* new_array = g_array_new(FALSE,FALSE,sizeof(char*));

    if(str[0] != '[' || str[len - 1] != ']') {
        g_array_free(new_array, TRUE);
        return NULL;
    }
    // removing " and []
    str += 1;
    len -= 1;
    str[len - 1] = '\0';

    char* id;
    
    while ((id = strsep(&str, ",")) != NULL) {

        while (*id == ' ' || *id == '\'') id++;

        int id_len = strlen(id);
        if (id_len > 0 && id[id_len - 1] == '\'') {
            id[id_len - 1] = '\0';
        }

        if (strlen(id) > 0) {
            char* id_cpy = strdup(id);
            g_array_append_val(new_array, id_cpy);
        }
    }


    return new_array;
}
