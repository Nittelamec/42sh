#ifndef BUILTINS_H
#define BUILTINS_H

#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "../expansion/init_table.h"

extern struct table *table;

int built_true(void);
int built_false(void);
int echo(char **strings);
int built_exit(char **str);
int built_unset(char **argv);

#endif /* !BUILTINS_H */
