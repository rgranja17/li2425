#include "parser/parserGen.h"

// Função para dar free aos elementos do GArray
void free_garray_parsed_elements(GArray *array) {
    for (guint i = 0; i < array->len; i++) {
        g_free(g_array_index(array, char *, i));
    }
    g_array_set_size(array, 0); 
}

int process_line(FILE *file, GArray *array, char **lineCopy) {
    char *line = NULL;
    size_t len = 0;

    if (getline(&line, &len, file) != -1) {
        free_garray_parsed_elements(array);
        *lineCopy = strdup(line);
        line[strcspn(line, "\n")] = '\0'; 

        char *aux = line;
        char *token;

        while ((token = strsep(&aux, ";")) != NULL) {
            int len = strlen(token);
            if (len > 2) {
                token++;
                token[len - 2] = '\0';
            }
            char *element = strdup(token);
            g_array_append_val(array, element);
        }

        free(line);
        return 1;
    }

    free(line);
    return 0;
}


void free_garray_parsed(GArray *array) {
    for (guint i = 0; i < array->len; i++) {
        g_free(g_array_index(array, char *, i));
    }
    g_array_free(array, TRUE);
}