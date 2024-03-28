#include "lexer_tools.h"

static struct Token keywordDictionary[] = {
    { IF, "if" },         { THEN, "then" }, { ELSE, "else" },
    { ELIF, "elif" },     { FI, "fi" },     { WHILE, "while" },
    { UNTIL, "until" },   { DO, "do" },     { DONE, "done" },
    { FOR, "for" },       { CASE, "case" }, { IN, "in" },
    { NULL_TOKEN, NULL },
};

/*
 * test si le mot est dans la liste réservée
 */
int is_reservedWord(char *value)
{
    for (int i = 0; keywordDictionary[i].value != NULL; i++)
    {
        if (strcmp(value, keywordDictionary[i].value) == 0)
            return 1;
    }
    return 0;
}

/*
 * dans le cas de double quote word, test la position de potentiel '\' puis '"'
 */
int *is_backslash_then_double_quote(char **value)
{
    int position = 0;
    int indice = 0;
    int *count = malloc(sizeof(int) * strlen(*value) / 2);
    char currentChar = (*value)[position];
    while (currentChar != '\0')
    {
        if (currentChar == '"'
            && (position != 0
                && (*value)[position - 1]
                    == '\\')) // enregistre la position de '\"'
        {
            count[indice] = position - 1;
            indice++;
        }
        position++;
        currentChar = (*value)[position];
    }
    count[indice] = -1;
    return count;
}

/*
 * supprime un caractère dans une string
 */
void rolCharAtPositions(char **input, int *position)
{
    int i = 0;
    while (position[i] != -1)
    {
        int j = position[i];
        while ((*input)[j] != '\0')
        {
            (*input)[j] = (*input)[j + 1];
            j++;
        }
        i++;
    }
}

/*
 * test si le caractère est un caractère de mot
 */
int is_word_char(char currentChar)
{
    return (
        (currentChar == '-') || (currentChar == '.') || (currentChar == '/')
        || isalnum(currentChar) || (currentChar == '+') || (currentChar == '%')
        || (currentChar == '@') || (currentChar == '~') || (currentChar == '^')
        || (currentChar == ',') || (currentChar == '*') || (currentChar == ':')
        || (currentChar == '?') || (currentChar == '_') || (currentChar == '[')
        || (currentChar == ']') || (currentChar == '$') || (currentChar == '=')
        || (currentChar == '#') || (currentChar == '\'') || (currentChar == '"')
        || (currentChar == '{') || (currentChar == '}') || currentChar == '\\');
}

/*
 * Incrémente *position et renvoit le caractère suivant à mettre dans
 * currentChar
 */
char forward_modif_char(char *input, size_t *position)
{
    (*position)++;
    return input[*position];
}

/*
 * remplace un caractère à position dans input
 */
char *replaceCharAtPosition(char **input, int position, char newChar)
{
    char *res = malloc(sizeof(char) * strlen(*input));
    if (res == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    int i = 0;
    while ((*input)[i] != '\0')
    {
        if (i == position)
            res[i] = newChar;
        else
            res[i] = (*input)[i];
        i++;
    }
    res[i] = '\0';
    *input = res;
    return res;
}

/*
 * gérer les isspace non espace pour juste afficher le caractère
 */
char *backslash_handle(char *input, size_t *position, char currentChar)
{
    if (currentChar == '\n')
        return replaceCharAtPosition(&input, *position, 'n');
    else if (currentChar == '\t')
        return replaceCharAtPosition(&input, *position, 't');
    else if (currentChar == '\v')
        return replaceCharAtPosition(&input, *position, 'v');
    else if (currentChar == '\f')
        return replaceCharAtPosition(&input, *position, 'f');
    else if (currentChar == '\r')
        return replaceCharAtPosition(&input, *position, 'r');
    else
        return input;
}
