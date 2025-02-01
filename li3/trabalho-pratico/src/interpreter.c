#include "interpreter.h"

#include "interpreter.h"

void command_interpreter(char *input, char **res) {
    res[0] = NULL;
    res[1] = NULL;
    res[2] = NULL;
    res[3] = NULL;

    char *input_copy = strdup(input);
    char *original_input_copy = input_copy;
    char *token;

    token = strsep(&input_copy, " ");
    if (token) {
        res[0] = strdup(token);

        token = strsep(&input_copy, " ");
        if (token) {
            res[1] = strdup(token);
        }
        int q = atoi(res[0]);
        if(q == 6){
            token = strsep(&input_copy, " ");
        } else {
            token = strsep(&input_copy, "\n");
        }
        if (token) {
            res[2] = strdup(token);
        }
        if(token && (q == 6)){
            token = strsep(&input_copy, "\n");
            if (token) {
                res[3] = strdup(token);
            }
        }
    }

    free(original_input_copy);
}

void free_command(char **res) {
    for (int i = 0; i < 4; i++) { // Atualizado para dar free ao res[3] também
        if (res[i]) {
            free(res[i]);
        }
    }
}
