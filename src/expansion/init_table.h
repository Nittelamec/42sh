#ifndef INIT_TABLE_H
#define INIT_TABLE_H

#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "../ast/ast.h"

extern struct table *table;

struct variable
{
    char *key;
    char **value;
    struct variable *next_var;
};

struct table
{
    struct variable *var;
    size_t size;
};

struct table *init_table(size_t size);
struct table *table_insert(struct table *table, char *key, char **value);
void table_free(struct table *free_table, int argc);
struct table *init_full_table(size_t size, int argc, char **argv);
struct table *table_pop(struct table *table, char *key);

#endif /* INIT_TABLE_H */
