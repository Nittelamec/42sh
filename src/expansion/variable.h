#ifndef VARIABLE_H
#define VARIABLE_H

#define _POSIX_C_SOURCE 200809L

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "../ast/ast_type.h"
#include "init_table.h"

struct table;

struct ast *expand_var(char **words, struct table *table, int argc);

#endif /* VARIABLE_H */
