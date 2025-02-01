#include "programa-testes.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/resource.h>
#include "programa-testes.h"

#define MAX_PATH_SIZE 5096
#define RESULTADOS_DIR "resultados"
#define EXPECTED_DIR "../Dados-fase1/outputs_esperados"

int compare_files(const char *file1, const char *file2) {
    FILE *fp1 = fopen(file1, "r");
    FILE *fp2 = fopen(file2, "r");
    if (!fp1 || !fp2) {
        return -1;
    }

    char line1[256], line2[256];
    while (fgets(line1, sizeof(line1), fp1) && fgets(line2, sizeof(line2), fp2)) {
        if (strcmp(line1, line2) != 0) {
            fclose(fp1);
            fclose(fp2);
            return 0; // Files are different
        }
    }

    int end_of_file1 = !fgets(line1, sizeof(line1), fp1);
    int end_of_file2 = !fgets(line2, sizeof(line2), fp2);

    fclose(fp1);
    fclose(fp2);
    return end_of_file1 && end_of_file2; // Both files reached end (are equal)
}

void run_tests(void) {
    DIR *dir = opendir(RESULTADOS_DIR);
    if (!dir) {
        perror("Error opening resultados directory");
        return;
    }

    struct dirent *entry;
    int all_tests_passed = 1;

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            char result_file[MAX_PATH_SIZE];
            char expected_file[MAX_PATH_SIZE];

            snprintf(result_file, sizeof(result_file), "%s/%s", RESULTADOS_DIR, entry->d_name);
            snprintf(expected_file, sizeof(expected_file), "%s/%s", EXPECTED_DIR, entry->d_name);

            if (!compare_files(result_file, expected_file)) {
                printf("Test failed for file: %s\n", entry->d_name);
                all_tests_passed = 0;
            } 
        }
    }

    closedir(dir);
    if (all_tests_passed) {
        printf("All tests passed successfully!\n");
    } else {
        printf("Some tests failed.\n");
    }
}


int main(int argc, char** argv) {
    if (argc > 3) {
        printf("./programa-principal <path dataset> <input>");
        return 1;
    }

    Database_manager* db = newDatabase();
    char* datasetDir = argv[1];
    char* inputFile = argv[2];
    char* outputDir = OUTPUT_DIR;

    FILE *fp = fopen(inputFile, "r");

    database_create_artists_error_file();
    database_create_musics_error_file();
    database_create_users_error_file();

    if (fp == NULL) {
        perror("Error opening input file!\n");
        exit(1);
    }

    char *command = malloc(sizeof(char) * MAX_COMMAND_SIZE);

    struct timespec start, end;
    double elapsed;
    double total_time_query1 = 0.0;
    double total_time_query2 = 0.0;
    double total_time_query3 = 0.0;

    clock_gettime(CLOCK_REALTIME, &start);
    create_artist_manager(db, datasetDir);
    //parser(db, datasetDir, 1);
    clock_gettime(CLOCK_REALTIME, &end);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Artist parsing took %.6f seconds\n", elapsed);

    clock_gettime(CLOCK_REALTIME, &start);
    create_music_manager_(db, datasetDir);
    //parser(db, datasetDir, 2);
    clock_gettime(CLOCK_REALTIME, &end);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Music parsing took %.6f seconds\n", elapsed);

    clock_gettime(CLOCK_REALTIME, &start);
    create_user_manager_(db, datasetDir);
    //parser(db, datasetDir, 3);
    clock_gettime(CLOCK_REALTIME, &end);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("User parsing took %.6f seconds\n", elapsed);

    int command_number = 1;
    while (fgets(command, MAX_COMMAND_SIZE, fp) != NULL) {
        size_t len = strlen(command);
        if (len > 0 && command[len - 1] == '\n') {
            command[len - 1] = '\0';
        }
        
        char querie_str[MAX_COMMAND_SIZE];
        strcpy(querie_str, command);
        char *command_ptr = querie_str;
        char *querie_token = strsep(&command_ptr, " ");
        int querie_id = atoi(querie_token);

        clock_gettime(CLOCK_REALTIME, &start);
        
        execute_command(command, db, outputDir, command_number);
        
        clock_gettime(CLOCK_REALTIME, &end);
        elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

        switch (querie_id) {
            case 1:
                total_time_query1 += elapsed;
                break;
            case 2:
                total_time_query2 += elapsed;
                break;
            case 3:
                total_time_query3 += elapsed;
                break;
            default:
                printf("Unknown query ID: %d\n", querie_id);
        }

        command_number++;
    }

    printf("Total time for Query 1: %.6f seconds\n", total_time_query1);
    printf("Total time for Query 2: %.6f seconds\n", total_time_query2);
    printf("Total time for Query 3: %.6f seconds\n", total_time_query3);


    free(command);
    fclose(fp);
    free_database(db);

    struct rusage r_usage;
    getrusage(RUSAGE_SELF, &r_usage);
    #ifdef __APPLE__ // Para MacOS
        printf("\nMemory usage: %.2f MB\n", r_usage.ru_maxrss / (1024.0 * 1024.0));
    #else //Para restantes Sistemas
        printf("\nMemory usage: %.2f MB\n", r_usage.ru_maxrss / 1024.0);
    #endif

    run_tests();
    return 0;
}