#ifndef INTERPRETER_H
#define INTERPRETER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <ctype.h>

void command_interpreter(char *input, char **res);

void free_command(char **res);

#endif 
