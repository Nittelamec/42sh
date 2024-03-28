#include "lexer_case_2.h"

/*
 * cas d'une double quote
 */
struct Token *double_quote_case(char *input, size_t *position, char currentChar)
{
    size_t start = *position;
    int in_double_quote = (currentChar == '"') ? 1 : 0;
    currentChar = forward_modif_char(input, position);
    while (is_word_char(currentChar)
           || (isspace(currentChar) && in_double_quote))
    {
        if (currentChar == '"' && input[*position - 1] != '\\')
            in_double_quote = !in_double_quote;
        currentChar = forward_modif_char(input, position);
    }
    if (in_double_quote) // si pas de quote fermante
    {
        struct Token *null = malloc(sizeof(struct Token));
        null->type = NULL_TOKEN;
        null->value = NULL;
        return null;
    }
    size_t length = *position - start;
    char *word = strndup(input + start, length);
    return createToken(DOUBLE_QUOTE_WORD, word);
}

/*
 * cas d'une redirection to file
 */
struct Token *redirection_to_file_case(char *input, size_t *position,
                                       char currentChar)
{
    currentChar = forward_modif_char(input, position);
    if (currentChar == '>')
    {
        (*position)++;
        return createToken(STDIN_STDOUT, "<>");
    }
    else if (currentChar == '&')
    {
        (*position)++;
        return createToken(FILE_DUP_STDOUT, "<&");
    }
    else
    {
        return createToken(FILE_STDIN, "<");
    }
}

/*
 * cas d'une redirection to stdout
 */
struct Token *redirection_to_stdout_case(char *input, size_t *position,
                                         char currentChar)
{
    currentChar = forward_modif_char(input, position);
    if (currentChar == '>')
    {
        (*position)++;
        return createToken(STDOUT_FILE_ADD, ">>");
    }
    else if (currentChar == '&')
    {
        (*position)++;
        return createToken(STDOUT_STDERR_FILE_ADD, ">&");
    }

    else if (currentChar == '|')
    {
        (*position)++;
        return createToken(FORCE_STDOUT_FILE, ">|");
    }
    else
    {
        return createToken(STDOUT_FILE, ">");
    }
}

/*
 * cas d'une negation
 */
struct Token *negation_case(char *input, size_t *position, char currentChar)
{
    (*position)++;
    currentChar = input[*position];
    if (isspace(currentChar) || currentChar == '\0')
        return createToken(NEGATION, "!");
    struct Token *null = malloc(sizeof(struct Token));
    null->type = NULL_TOKEN;
    null->value = NULL;
    return null;
}

/*
 * Cas d'un IONUMBER
 */
struct Token *is_ionumber(char *input, size_t *position, char currentChar)
{
    size_t start = *position;
    while (isdigit(currentChar))
    {
        currentChar = forward_modif_char(input, position);
    }

    size_t length = *position - start;
    char *word = strndup(input + start, length);
    struct Token *token_suivant = lexer_peek(input, position);
    if (token_suivant->type == STDIN_STDOUT
        || token_suivant->type == FILE_DUP_STDOUT
        || token_suivant->type == FILE_STDIN
        || token_suivant->type == STDOUT_FILE_ADD
        || token_suivant->type == STDOUT_STDERR_FILE_ADD
        || token_suivant->type == FORCE_STDOUT_FILE
        || token_suivant->type == STDOUT_FILE)
    {
        freeToken(token_suivant);
        return createToken(IONUMBER, word);
    }
    else
    {
        freeToken(token_suivant);
        return createToken(WORD, word);
    }
}

/*
 * test si un mot est un assignemnt word
 */
int is_valid_equal(char *input, size_t *position)
{
    size_t start = *position;
    char currentChar = input[*position];
    while (currentChar != '=' && currentChar != '\0')
        currentChar = forward_modif_char(input, position);
    if (currentChar == '\0')
        return 0;
    if (isspace(input[*position - 1]) || isspace(input[*position + 1])
        || isspace(input[start]) || isdigit(input[start]))
        return 0;
    return 1;
}
