#include "variable.h"

char *is_there_var(char *string)
{
    size_t len = strlen(string);
    char *name_var = malloc(sizeof(char) * (len + 1));
    if (!name_var)
        return NULL;
    char precedentChar = '\0';
    for (size_t i = 0; i < len; i++)
    {
        if (i > 0)
            precedentChar = string[i - 1];

        char currentChar = string[i];
        if (currentChar == '$' && precedentChar != '\\')
        {
            size_t j = 0;
            if (string[i + 1] == '(')
            {
                i++;
                while (currentChar != ')' && currentChar != '\0' && i < len)
                {
                    currentChar = string[i + 1];
                    name_var[j] = currentChar;
                    i++;
                    j++;
                }
            }
            else if (string[i + 1] == '{')
            {
                i++;
                while (currentChar != '}' && currentChar != '\0' && i < len)
                {
                    currentChar = string[i + 1];
                    name_var[j] = currentChar;
                    i++;
                    j++;
                }
            }
            if (currentChar == '\0')
            {
                free(name_var);
                return NULL;
            }
            if (currentChar != '{' && currentChar != '(')
            {
                while (isspace(currentChar) == 0 && currentChar != '\0'
                       && currentChar != '{' && currentChar != ';')
                {
                    currentChar = string[i + 1];
                    name_var[j] = currentChar;
                    i++;
                    j++;
                }
            }
            name_var[j] = '\0';
            return name_var;
        }
    }
    free(name_var);
    return NULL;
}

/*char **get_table(char *name_var, struct table *table)
{
    if (table == NULL || name_var == NULL)
        return NULL;
    struct variable *current = table->var;
    while (current != NULL)
    {
        if (strcmp(current->key, name_var) == 0)
            return current->value;
        current = current->next_var;
    }
    return NULL;
}*/

// à retirer ou au moins à mettre dans un autre fichier, même si actuellement
int my_strcmp(char *s1, char *s2)
{
    size_t i = 0;
    while (s1[i] != '\0' && s2[i] != '\0')
    {
        if (s1[i] != s2[i])
            return 1;
        i++;
    }
    if (s1[i] != s2[i])
        return 1;
    return 0;
}

char **replace_name_by_value(char *name_var, struct table *table)
{
    if (table == NULL || name_var == NULL)
        return NULL;
    struct variable *current = table->var;
    while (current != NULL)
    {
        if (my_strcmp(current->key, name_var) == 0)
            return current->value;
        current = current->next_var;
    }
    return NULL;
}

char *error_free(char *name_var, char *new_word)
{
    free(name_var);
    free(new_word);
    return NULL;
}

int global_if_strcmp(char *name_var)
{
    if (strcmp(name_var, "*") == 0 || strcmp(name_var, "#") == 0
        || strcmp(name_var, "?") == 0 || strcmp(name_var, "$") == 0
        || strcmp(name_var, "RANDOM") == 0 || strcmp(name_var, "UID") == 0
        || strcmp(name_var, "IFS") == 0)
        return 1;
    return 0;
}

char *attribution_de_tmp2(char *name_var, char *tmp2, char **value)
{
    if (global_if_strcmp(name_var) == 1)
        tmp2 = (char *)value;
    else
        tmp2 = *value;
    return tmp2;
}

char *while_of_expand(char *word, char *new_word, int **flags,
                      struct table *table)
{
    while (word[(*flags)[0]] != '\0')
    {
        char precedentChar = ((*flags)[0] > 0) ? word[(*flags)[0] - 1] : '\0';
        if (word[(*flags)[0]] == '\'' && precedentChar != '\\')
        {
            (*flags)[2] = !(*flags)[2];
            (*flags)[0]++;
        }
        if (word[(*flags)[0]] == '$' && precedentChar != '\\' && !(*flags)[2])
        {
            size_t is_there_braces_or_parentheses = 0;
            if (word[(*flags)[0] + 1] == '(' || word[(*flags)[0] + 1] == '{')
                is_there_braces_or_parentheses = 1;
            size_t tmp = 2; // pour être du bon type
            char *name_var = is_there_var(word);
            char *big_name = malloc(
                sizeof(char)
                * (strlen(name_var) + tmp * is_there_braces_or_parentheses
                   + tmp)); // +2 pour les parenthèses ou accolades et +2 pour
                            // le \0 et le $
            if (!big_name)
                return error_free(name_var, new_word);
            strncpy(big_name, word + (*flags)[0],
                    strlen(word + (*flags)[0])
                        + 2 * is_there_braces_or_parentheses + 1);
            big_name[strlen(word + (*flags)[0])
                     + 2 * is_there_braces_or_parentheses] = '\0';
            char **value = replace_name_by_value(name_var, table);
            if (value == NULL)
            // si pas de valeur associé, on continue en passant le nom de
            // variable
            {
                error_free(name_var, big_name);
                return NULL;
            }
            char *tmp2 = attribution_de_tmp2(name_var, tmp2, value);
            (*flags)[0] += strlen(big_name);
            (*flags)[1] += strlen(tmp2);
            strcat(new_word, tmp2);
            error_free(name_var, big_name);
        }
        else
        {
            new_word[(*flags)[1]] = word[(*flags)[0]];
            new_word[(*flags)[1] + 1] = '\0';
            (*flags)[1]++;
            (*flags)[0]++;
        }
    }
    return new_word;
}

char *__expand_var(char *word, struct table *table)
{
    char *new_word = malloc(sizeof(char) * 15000);
    if (!new_word)
        return NULL;
    new_word[0] = '\0';
    int *flags = malloc(sizeof(int) * 3);
    if (!flags)
        return NULL;
    flags[0] = 0; // index de word
    flags[1] = 0; // index de new_word
    flags[2] = 0; // 0 si pas dans des simple_quote, 1 sinon
    new_word = while_of_expand(word, new_word, &flags, table);
    if (!new_word)
    {
        free(new_word);
        return strdup("");
    }
    new_word[flags[1]] = '\0';
    // free(word);
    free(flags);
    return new_word;
}

struct ast *expand_var(char **words, struct table *table, int argc)
{
    char **words_cpy = calloc(1, sizeof(char *));
    for (int k = 0; k < argc; k++)
    {
        words_cpy[k] = __expand_var(words[k], table);
        words_cpy = realloc(words_cpy, (k + 2) * sizeof(char *));
        words_cpy[k + 1] = NULL;
    }
    return init_ast_cmd(words_cpy);
}

/*#include "variable.h"
#include <stdio.h>

#include "init_table.h"

void print_words(char **words) {
    printf("Words: ");
    while (*words) {
        printf("%s ", *words);
        words++;
    }
    printf("\n");
}

int main() {
    struct ast_cmd *ast_cmd = malloc(sizeof(struct ast_cmd));
    char *str = strdup("$var");
    ast_cmd->words = malloc(2 * sizeof(char *));
    ast_cmd->base.type = AST_CMD;
    ast_cmd->words[0] = str;
    ast_cmd->words[1] = NULL;
    char **argv = malloc(2 * sizeof(char *));
    argv[0] = strdup("var");
    argv[1] = NULL;
    struct table *table = init_full_table(0, 1, argv);
    char *test = "test";
    table = table_insert(table, "var", &test);

    // Expansion des variables dans les mots
    ast_cmd = expand_var(ast_cmd, table, 1);

    // Vérification
    print_words(ast_cmd->words);

    // Libération de la mémoire
    free_ast(&ast_cmd->base);
    free(argv[0]);
    free(argv);
    table_free(table, 1);

    return 0;
}*/
