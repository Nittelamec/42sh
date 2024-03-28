#include "init_table.h"

#include <stddef.h>
#include <stdlib.h>

#define MAX_STR_SIZE 10000

struct table *init_table(size_t size)
{
    struct table *init_table = malloc(sizeof(struct table));
    if (init_table == NULL)
        return NULL;
    init_table->size = size;
    init_table->var = NULL;
    return init_table;
}

int index_if_exists(struct table *table, char *key)
{
    if (table == NULL || key == NULL)
        return -1;
    struct variable *tmp = table->var;
    int i = 0;
    while (tmp != NULL)
    {
        if (strcmp(tmp->key, key) == 0)
            return i;
        tmp = tmp->next_var;
        i++;
    }
    return -1;
}

struct table *table_insert(struct table *table, char *key, char **value)
{
    if (table == NULL || key == NULL || value == NULL)
        return NULL;
    struct variable *new_var = malloc(sizeof(struct variable));
    new_var->key = key;
    new_var->value = value;
    new_var->next_var = NULL;
    struct variable *tmp = table->var;
    if (tmp == NULL)
    {
        table->var = new_var;
        table->size += 1;
        return table;
    }
    int index = index_if_exists(table, key);
    if (index != -1)
    {
        free(new_var);
        for (int i = 0; i < index; i++)
            tmp = tmp->next_var;
        tmp->value = value;
        return table;
    }
    while (tmp->next_var != NULL)
        tmp = tmp->next_var;
    tmp->next_var = new_var;
    table->size += 1;
    return table;
}

struct table *table_pop(struct table *table, char *key)
{
    if (table == NULL || key == NULL)
        return NULL;
    struct variable *tmp = table->var;
    struct variable *prev = NULL;
    while (tmp != NULL)
    {
        if (strcmp(tmp->key, key) == 0)
        {
            if (prev == NULL)
                table->var = tmp->next_var;
            else
                prev->next_var = tmp->next_var;
            free(tmp);
            table->size -= 1;
            return table;
        }
        prev = tmp;
        tmp = tmp->next_var;
    }
    return NULL;
}

void table_free(struct table *free_table, int argc)
{
    if (free_table == NULL)
        return;
    struct variable *current = free_table->var;
    int i = 0;
    while (current != NULL)
    {
        struct variable *tmp = current;
        current = current->next_var;
        if (i < argc)
            free(tmp->key);
        free(tmp);
        i++;
    }
    free(free_table);
}

char *my_itoa(int value, char *s)
{
    if (value == 0)
    {
        s[0] = '0';
        s[1] = '\0';
        return s;
    }
    int neg = 0;
    if (value < 0)
        neg = 1;
    int puiss = 0;
    int a = 0;
    if (neg)
        a = value * (-1);
    else
        a = value;
    while (a >= 1)
    {
        a = a / 10;
        puiss++;
    }
    if (neg)
    {
        s[0] = '-';
        a = value * (-1);
        for (int i = puiss; i >= 1; i--)
        {
            s[i] = (a % 10) + 48;
            a = a / 10;
        }
        s[puiss + 1] = '\0';
    }
    else
    {
        a = value;
        for (int i = puiss - 1; i >= 0; i--)
        {
            s[i] = (a % 10) + 48;
            a = a / 10;
        }
        s[puiss] = '\0';
    }
    return s;
}

void insert_param(struct table *table, int argc, char **argv)
{
    for (int i = 0; i < argc; i++)
    {
        char *arg2 = malloc(sizeof(char) * 5);
        if (arg2 == NULL)
        {
            table_free(table, argc);
            return;
        }
        char *num = malloc(sizeof(char) * 5);
        num = my_itoa(i + 1, num);
        if (num == NULL)
        {
            table_free(table, argc);
            return;
        }
        arg2[0] = '\0';
        strcat(arg2, num);
        free(num);
        table = table_insert(table, arg2, &(argv[i]));
    }
}

struct table *init_full_table(size_t size, int argc, char **argv)
{
    struct table *table = init_table(size);
    if (table == NULL)
        return NULL;

    insert_param(table, argc, argv);

    table = table_insert(table, "@", argv);

    // insérer "$*" en prenant argv avec tous les arguments comme un seul
    // argument
    char all_args[MAX_STR_SIZE * 10] = "";
    all_args[0] = '\0';
    for (int i = 0; i < argc - 1; i++)
    {
        strcat(all_args, argv[i]);
        strcat(all_args, " ");
    }
    strcat(all_args, argv[argc - 1]);
    table = table_insert(table, "*", (char **)&all_args);

    char arg[MAX_STR_SIZE] = "";
    arg[0] = '0';
    arg[1] = '\0';
    table = table_insert(table, "?", (char **)&arg);

    char arg3[MAX_STR_SIZE] = "";
    my_itoa(getppid(), arg3);
    table = table_insert(table, "$", (char **)&arg3);

    char arg4[MAX_STR_SIZE] = "";
    my_itoa(argc, arg4);
    table = table_insert(table, "#", (char **)&arg4);

    char arg5[MAX_STR_SIZE] = "";
    srand(time(NULL));
    my_itoa(rand() % 32768, arg5);
    table = table_insert(table, "RANDOM", (char **)&arg5);

    char arg6[MAX_STR_SIZE] = "";
    my_itoa(getuid(), arg6);
    table = table_insert(table, "UID", (char **)&arg6);

    char *arg7 = getenv("OLDPWD");
    table = table_insert(table, "OLDPWD", &arg7);

    char *arg8 = getenv("PWD");
    table = table_insert(table, "PWD", &arg8);

    char *arg9 = getenv("IFS");
    table = table_insert(table, "IFS", &arg9);

    return table;
}
