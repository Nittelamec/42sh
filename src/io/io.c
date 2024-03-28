#include "io.h"

#include <stdio.h>
#include <string.h>

char *processInput(int argc, char **argv)
{
    char *result = NULL;
    FILE *input = NULL;
    if (argc > 1)
    {
        if (strcmp(argv[1], "-c") != 0)
        {
            input = fopen(argv[1], "r");
            if (input == NULL)
                return NULL;
            fseek(input, 0, SEEK_END);
            long length = ftell(input);
            fseek(input, 0, SEEK_SET);
            result = malloc(length + 1);
            if (result != NULL)
            {
                if (!fread(result, sizeof(char), length, input))
                {}
                result[length] = '\0';
            }
            fclose(input);
            table = init_full_table(0, argc - 2, argv + 2);
        }
        else
        {
            if (argc > 2)
                result = strdup(argv[2]);
            table = init_full_table(0, argc - 3, argv + 3);
        }
    }
    else
    {
        FILE *tmp = tmpfile();
        if (tmp == NULL)
            return NULL;
        int c;
        while ((c = fgetc(stdin)) != EOF)
            fputc(c, tmp);
        fseek(tmp, 0, SEEK_END);
        long length = ftell(tmp);
        fseek(tmp, 0, SEEK_SET);
        result = malloc(length + 1);
        if (result != NULL)
        {
            if (!fread(result, sizeof(char), length, tmp))
            {}
            result[length] = '\0';
        }
        fclose(tmp);
        table = init_full_table(0, 0, &result);
    }
    return result;
}

/*#include "../lexer/lexer.h"

int main(int argc, char **argv) {
    // Appeler processInput pour obtenir une chaîne à partir du fichier
    char *input = processInput(argc, argv);

    // Initialiser une position pour getNextToken
    size_t position = 0;

    // Boucler pour obtenir et afficher tous les tokens
    while (1) {
        struct Token *token = getNextToken(input, &position);

        // Vérifier si on a atteint la fin du fichier
        if (token == NULL) {
            break;
        }

        // Afficher le type et la valeur du token
        printf("Type: %d, Value: %s\n", token->type, token->value);

        // Libérer la mémoire allouée pour le token
        freeToken(token);
    }

    // Libérer la mémoire allouée pour l'entrée
    free(input);

    return 0;
}*/
