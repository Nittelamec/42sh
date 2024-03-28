#ifndef IO_H
#define IO_H

#define _POSIX_C_SOURCE 200809L

#include <stdlib.h>
#include <string.h>

#include "../expansion/init_table.h"
#include "../lexer/lexer_tools.h"

extern struct table *table;

char *processInput(int argc, char **argv);

#endif /* IO_H */
