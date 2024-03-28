#include "builtins.h"

int echo_2(int *flags, char **strings)
{
    size_t nb_words = 0;
    while (strings[nb_words])
        nb_words++;
    size_t index = 0;
    char *str = strings[index];
    size_t len = 0;
    if (str)
        len = strlen(str);
    for (size_t j = 0; j < nb_words; j++)
    {
        for (size_t i = 0; i < len; i++)
        {
            if (flags[1] == 0 || str[i] != '\\')
                putchar(str[i]);
            else if (flags[1] == 1 && str[i] == '\\')
            {
                if (str[i + 1] == 'n')
                {
                    putchar('\n');
                    i++;
                }
                else if (str[i + 1] == 't')
                {
                    putchar('\t');
                    i++;
                }
                else if (str[i + 1] == '\\')
                {
                    putchar('\\');
                    i++;
                }
            }
            else if (flags[1] == 1 && str[i] == '\n')
                putchar('\n');
            else if (flags[1] == 1 && str[i] == '\t')
                putchar('\t');
            else if (flags[1] == 1 && str[i] == '\v')
                putchar('\v');
            else if (flags[1] == 1 && str[i] == '\f')
                putchar('\f');
            else if (flags[1] == 1 && str[i] == '\r')
                putchar('\r');
        }
        str = strings[++index];
        if (str)
        {
            putchar(' ');
            len = strlen(str);
        }
    }
    return flags[0];
}

int echo(char **strings)
{
    int flags[3] = {
        0, 0, 0
    }; // flags[0] = -n, flags[1] = -e, flags[2] = no more flags
    size_t index = 0;
    int w = 0;
    char *str = strings[index];
    while (str && flags[2] == 0)
    {
        size_t len = strlen(str);
        if (str[0] == '-' && len > 1)
        {
            size_t i = 1;
            while (i < len)
            {
                if (str[i] != 'n' && str[i] != 'e' && str[i] != 'E')
                {
                    flags[2] = 1;
                    break;
                }
                i++;
            }
            i = 1;
            if (flags[2] == 0)
            {
                while (i < len)
                {
                    if (str[i] == 'n')
                        flags[0] = 1;
                    else if (str[i] == 'e')
                        flags[1] = 1;
                    else if (str[i] == 'E')
                        flags[1] = 0;
                    i++;
                }
            }
        }
        else
        {
            flags[2] = 1;
            w = 1;
        }
        str = strings[++index];
    }
    flags[0] = echo_2(flags, strings + index - w);
    if (flags[0] == 0)
        putchar('\n');
    return 0;
}
