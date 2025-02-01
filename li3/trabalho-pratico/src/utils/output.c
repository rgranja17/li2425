#include "utils/output.h"


int database_create_users_error_file(void){
    FILE* file = NULL;
    file = fopen("resultados/users_errors.csv","w");
    if(!file)
        return 1;
    fprintf(file, "\"username\";\"email\";\"first_name\";\"last_name\";\"birth_date\";\"country\";\"subscription_type\";\"liked_songs_id\"\n");
    fclose(file);
    return 0;
}
int database_create_album_error_file(void){
    FILE* file = NULL;
    file = fopen("resultados/album_errors.csv","w");
    if(!file)
        return 1;
    fprintf(file, "\"id\";\"title\";\"artist_id\";\"year\";\"producers\"\n");
    fclose(file);
    return 0;
}
int database_create_musics_error_file(void){
    FILE* file = NULL;
    file = fopen("resultados/musics_errors.csv","w");
    if(!file)
        return 1;
    fprintf(file, "\"id\";\"title\";\"artist_id\";\"album_id\";\"duration\";\"genre\";\"year\";\"lyrics\"\n");
    fclose(file);
    return 0;
}
int database_create_artists_error_file(void){
    FILE* file = NULL;
    file = fopen("resultados/artists_errors.csv","w");
    if(!file)
        return 1;
    fprintf(file, "\"id\";\"name\";\"description\";\"recipe_per_stream\";\"id_constituent\";\"country\";\"type\"\n");
    fclose(file);
    return 0;
}
int database_create_history_error_file(void){
    FILE* file = NULL;
    file = fopen("resultados/history_errors.csv","w");
    if(!file)
        return 1;
    fprintf(file, "\"id\";\"user_id\";\"music_id\";\"timestamp\";\"duration\";\"platform\"\n");
    fclose(file);
    return 0;
}

void save_result(int command_number, const char *res, const char *outputDir,int alternative_format) {
    char output_file[256]; 

    if(outputDir[strlen(outputDir) - 1] == '/') {
        snprintf(output_file, sizeof(output_file), "%scommand%d_output.txt", outputDir, command_number);
    } else {
        snprintf(output_file, sizeof(output_file), "%s/command%d_output.txt", outputDir, command_number);
    }
    FILE *fp = fopen(output_file, "w");
    if(fp == NULL) {
        perror("Error opening output file");
        exit(1);
    }

    char *output_res = strdup(res);

    if(alternative_format) {
        for(char* p = output_res; *p != '\0'; p++) {
            if(*p==';') *p = '=';
        }
    }

    fprintf(fp, "%s", output_res);

    if (fclose(fp) != 0) {
        perror("Error closing output file");
    }

    free(output_res);
}


void log_error(FILE* file, const char* error){
    char* line_cp = strdup(error);
    if (!error)
        printf("error opening file\n");
    
    fprintf(file,"%s",line_cp);
    
    free(line_cp);
}
