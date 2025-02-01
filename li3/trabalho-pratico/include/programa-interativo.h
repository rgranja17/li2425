#ifndef PROGRAMA_INTERATIVO_H
#define PROGRAMA_INTERATIVO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "data_manager/database_manager.h"
#include "batchMode.h"
#include "time.h"
#include "interpreter.h"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"

#define MAX_PATH_LENGTH 256
#define MAX_COMMAND_LENGTH 1024
void showWelcomeMessage();
void printError(const char *message);
void printSuccess(const char *message);
void printInfo(const char *message);
void showHelp();
void autoCompleteCommand(char *command);
void loadDataset(Database_manager *db, const char *path);
void executeQueryInteractive(char *command, Database_manager *db, const char *outputDir);
void interactiveMode(Database_manager *db, const char *outputDir);

#endif