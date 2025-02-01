#include "programa-interativo.h"

void showWelcomeMessage() {
    printf("\n===============================\n");
    printf(" Bem-vindo ao Programa Interativo\n");
    printf(" Desenvolvido por: Rodrigo Granja, Diogo Esteves, Gonçalo Alves\n");
    printf("===============================\n\n");
}

void printError(const char *message) {
    printf(RED "Erro: %s\n" RESET, message);
}

void printSuccess(const char *message) {
    printf(GREEN "Sucesso: %s\n" RESET, message);
}

void printInfo(const char *message) {
    printf(YELLOW "Info: %s\n" RESET, message);
}

void showHelp() {
    printf("\n--- Ajuda ---\n");
    printf("Digite um comando para executar uma query.\n");
    printf("Comandos especiais:\n");
    printf("  sair   - Termina o programa.\n");
    printf("  help   - Mostra esta mensagem de ajuda.\n");
    printf("----------------\n");
    printf("\nExemplos de queries:\n");
    printf("  1 A0008766         - Consulta por artista ou utilizador.\n");
    printf("  2 5 \"Portugal\"  - Consulta top N por país.\n");
    printf("  3 37 42           - Consulta de faixa etária.\n");
    printf("----------------\n");
}

void autoCompleteCommand(char *command) {
    if (strcmp(command, "1") == 0) {
        printf(YELLOW "Sugestão: Digite '1 A0008766'\n" RESET);
    } else if (strcmp(command, "2") == 0) {
        printf(YELLOW "Sugestão: Digite '2 5 \"Portugal\"'\n" RESET);
    } else if (strcmp(command, "3") == 0) {
        printf(YELLOW "Sugestão: Digite '3 37 42'\n" RESET);
    }
}

void loadDataset(Database_manager *db, const char *path) {
    printf("Carregando dataset a partir de: %s\n", path);

    create_artist_manager(db, path);
    printSuccess("Artistas carregados.");

    create_album_manager(db, path);
    printSuccess("Álbuns carregados.");

    create_music_manager_(db, path);
    printSuccess("Músicas carregadas.");

    create_user_manager_(db, path);
    printSuccess("Utilizadores carregados.");

    create_history_manager(db, path);
    printSuccess("Histórico carregado.");
}

void executeQueryInteractive(char *command, Database_manager *db, const char *outputDir) {
    static int command_number = 1;
    struct timespec start, end;

    if (!command || strlen(command) == 0) {
        printError("Comando vazio ou nulo.");
        return;
    }

    // Validação do comando
    char query_type;
    int valid = sscanf(command, "%c", &query_type);
    if (!valid || (query_type < '1' || query_type > '6')) {
        printError("Comando inválido. Digite 'help' para assistência.");
        return;
    }

    // Validação de subcomando, ex.: '1S', '2S'
    if (strlen(command) > 1 && command[1] != ' ' && command[1] != 'S') {
        printError("Comando inválido. Verifique o formato e tente novamente.");
        return;
    }

    clock_gettime(CLOCK_REALTIME, &start);
    execute_command(command, db, (char *)outputDir, command_number);
    clock_gettime(CLOCK_REALTIME, &end);

    double elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    char result_file_path[MAX_PATH_LENGTH];
    snprintf(result_file_path, sizeof(result_file_path), "%s/command%d_output.txt", outputDir, command_number);

    FILE *result_file = fopen(result_file_path, "r");
    if (result_file) {
        printf("\n--- Resultado da Query ---\n");
        char line[MAX_COMMAND_LENGTH];
        while (fgets(line, sizeof(line), result_file)) {
            printf("%s", line);
        }
        fclose(result_file);
        printf("--------------------------\n");
    } else {
        printError("Erro ao abrir o arquivo de resultado.");
    }

    printf("Tempo de execução: %.9f segundos\n", elapsed_time);
    command_number++;
}


void interactiveMode(Database_manager *db, const char *outputDir) {
    char command[MAX_COMMAND_LENGTH];
    printInfo("Sistema pronto para execução de queries.");

    while (1) {
        printf("\nDigite uma query (ou 'sair' para terminar, ou 'help' para ajuda): ");
        if (fgets(command, MAX_COMMAND_LENGTH, stdin) == NULL) {
            printError("Erro ao ler a query.");
            break;
        }

        command[strcspn(command, "\n")] = 0;

        if (strcmp(command, "sair") == 0) {
            printInfo("Encerrando o programa.");
            break;
        }

        if (strcmp(command, "help") == 0) {
            showHelp();
            continue;
        }

        autoCompleteCommand(command);
        executeQueryInteractive(command, db, outputDir);
    }
}

int main() {
    char datasetPath[MAX_PATH_LENGTH];
    showWelcomeMessage();

    Database_manager *db = newDatabase();
    if (!db) {
        printError("Erro ao inicializar o gerenciador de banco de dados.");
        return 1;
    }

    int datasetLoaded = 0; // Flag para verificar se o dataset foi carregado com sucesso

    while (!datasetLoaded) {
        printf("Introduza o caminho dos ficheiros de dados: ");
        if (fgets(datasetPath, MAX_PATH_LENGTH, stdin) == NULL) {
            printError("Erro ao ler o caminho dos ficheiros de dados.");
            free_database(db);
            return 1;
        }

        datasetPath[strcspn(datasetPath, "\n")] = 0;
        printf("\nVerificando o caminho fornecido...\n");
        if (access(datasetPath, F_OK) != 0) { 
            printError("O caminho fornecido não existe ou não é acessível. Tente novamente.");
            continue;
        }

        printf("Carregando dataset...\n");
        loadDataset(db, datasetPath);

        // Assume que o carregamento foi bem-sucedido se nenhuma mensagem de erro foi exibida
        datasetLoaded = 1;
    }

    interactiveMode(db, "resultados");
    free_database(db);

    printf(GREEN "\nPrograma finalizado com sucesso. Até breve!\n" RESET);
    return 0;
}

