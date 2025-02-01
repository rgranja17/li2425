#include "batchMode.h"

void execute_command(char* command, Database_manager* db, char* output_file, int command_number) {
    char *result_command[4];
    command_interpreter(command, result_command);

    int querie_id = atoi(result_command[0]);
    int alternative_format = (strchr(result_command[0],'S') != NULL);

    char **args = malloc(sizeof(char*) * MAX_ARGS);
    int args_size = 0;

    if (result_command[2] != NULL && strcmp(result_command[2], "") != 0) {
        char *token = result_command[2];
        int inside_quotes = 0;

        while (*token != '\0' && args_size < MAX_ARGS) {
            while (*token == ' ') token++;

            if (*token == '\"') {
                inside_quotes = 1;
                token++;
            }
            args[args_size++] = token;

            while (*token != '\0' && (*token != ' ' || inside_quotes)) {
                if (*token == '\"') inside_quotes = !inside_quotes;
                token++;
            }
            if (*token != '\0') {
                *token = '\0';
                token++;
            }
            if (args[args_size - 1][strlen(args[args_size - 1]) - 1] == '\"') {
                args[args_size - 1][strlen(args[args_size - 1]) - 1] = '\0';
            }
        }
    }

    switch (querie_id) {
        case 1: {
            char* res_1 = query1(db, result_command[1]);
            save_result(command_number, res_1, output_file,alternative_format);
            free(res_1);
            break;
        }
        case 2: {
            int n = atoi(result_command[1]);
            char* country = result_command[2];
            if (country != NULL){
                country++;
            }

            char* res_2 = query2(db, n, country);
            save_result(command_number, res_2, output_file,alternative_format);
            free(res_2);
            break;
        }
        case 3: {
            int min_age = atoi(result_command[1]);
            int max_age = atoi(result_command[2]);
            
            char* res_3 = query3(db, min_age, max_age);
            save_result(command_number, res_3, output_file,alternative_format);
            free(res_3);
            break;
        }
        case 5: {
            char* user_id = result_command[1];
            int num_recommendations = atoi(result_command[2]);

            char* res_5 = query5(db, user_id, num_recommendations);
            save_result(command_number, res_5, output_file, alternative_format);
            free(res_5);
            break;
        }
        case 6: {
            char* user_id = result_command[1];
            int year = atoi(result_command[2]);
            int N = 0;
            char* nString = result_command[3];
            if (nString != NULL){
                N = atoi(nString);
            }
            
            char* res_6 = query6(db, user_id, year, N);
            save_result(command_number, res_6, output_file,alternative_format);
            free(res_6);
            break;
        }
       break;
    }

    free_command(result_command);
    free(args);
}

void batchMode(char *inputFile, char *outputDirectory, Database_manager *db, char* dataset_path) {
    FILE *fp = fopen(inputFile, "r");

    database_create_artists_error_file();
    database_create_musics_error_file();
    database_create_users_error_file();
    database_create_history_error_file();

    if (fp == NULL) {
        perror("Error opening input file!\n");
        exit(1);
    }

    char *command = malloc(sizeof(char) * MAX_COMMAND_SIZE);

    create_artist_manager(db,dataset_path);
    printf("Artist Parsed\n");
    create_album_manager(db,dataset_path);
    printf("album Parsed\n");
    create_music_manager_(db,dataset_path);
    printf("music Parsed\n");
    create_user_manager_(db, dataset_path);
    printf("users Parsed\n");
    create_history_manager(db,dataset_path);
    printf("history Parsed\n");

    int command_number = 1; 
    while (fgets(command, MAX_COMMAND_SIZE, fp) != NULL) {
        size_t len = strlen(command);
        if (len > 0 && command[len - 1] == '\n') {
            command[len - 1] = '\0'; 
        }

        execute_command(command, db, outputDirectory, command_number); 
        command_number++; 
    }

    free(command);
    fclose(fp);
}