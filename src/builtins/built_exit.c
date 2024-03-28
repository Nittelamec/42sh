#include <ctype.h>
#include <err.h>
#include <stdlib.h>
#include <string.h>

#include "g_exito.h"
#include "g_lasto.h"

int built_exit(char **str)
{
    int n;
    int nbwords = 0;
    while (str[nbwords])
        nbwords++;
    if (nbwords > 1)
        errx(1, "exit: to many arguments to exit");
    if (!(*str))
        n = get_g_lasto();
    else
    {
        for (size_t i = 0; i < strlen(*str); i++)
        {
            if (!isdigit((*str)[i]))
                errx(2, "exit: need numerical argument");
        }
        n = atoi(*str);
    }
    mod_g_exito(n);
    return get_g_exito();
}
