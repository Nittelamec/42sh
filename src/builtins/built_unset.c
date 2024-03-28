#include "builtins.h"

int built_unset(char **argv)
{
    int argc = 0;
    while (argv[argc] != NULL)
        argc++;
    int i = 1;
    if (!strcmp(argv[i], "-f"))
    {
        if (strcmp(argv[i], "-v"))
            i++;
        while (i < argc)
        {
            table = table_pop(table, argv[i]);
            if (table == NULL)
                return 1;
            i++;
        }
        return 0;
    }
    else
        return 0;
}
