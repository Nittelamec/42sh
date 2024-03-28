#include "lexer_case_1.h"

/*
 * return le token associé au mot réservé
 */
struct Token defineWord(char *value, struct Token *dictionary)
{
    for (int i = 0; dictionary[i].value != NULL; i++)
    {
        if (strcmp(value, dictionary[i].value) == 0)
            return dictionary[i];
    }
    struct Token nullToken = { NULL_TOKEN, NULL };
    return nullToken;
}

/*
 * cas d'un commentaire
 */
struct Token *comment_case(char *input, size_t *position, char currentChar)
{
    while (currentChar != '\n' && currentChar != '\0')
        currentChar = forward_modif_char(input, position);
    return getNextToken(input, position);
}

/*
 * cas d'une single quote
 */
struct Token *single_quote_case(char *input, size_t *position, char currentChar)
{
    size_t start = *position;
    int in_single_quote = (currentChar == '\'') ? 1 : 0;
    currentChar = forward_modif_char(input, position);
    while (is_word_char(currentChar)
           || (isspace(currentChar) && in_single_quote))
    {
        if (currentChar == '\''
            && (input[*position - 1] != '\\' || in_single_quote == 1))
            in_single_quote = !in_single_quote;
        currentChar = forward_modif_char(input, position);
    }
    if (in_single_quote) // si pas de quote fermante
    {
        struct Token *null = malloc(sizeof(struct Token));
        null->type = NULL_TOKEN;
        null->value = NULL;
        return null;
    }
    size_t length = *position - start;
    char *word = strndup(input + start, length);
    (*position)++;
    return createToken(WORD, word);
}

/*
 * cas d'un mot
 */
struct Token *word_case(char *input, size_t *position, char currentChar)
{
    size_t start = *position;
    while (is_word_char(currentChar))
    {
        if (currentChar == '"')
        {
            *position = start;
            return double_quote_case(input, position, input[*position]);
        }
        else if (currentChar == '\'')
        {
            *position = start;
            return single_quote_case(input, position, input[*position]);
        }
        else if (currentChar == '=' && *position != start)
        {
            *position = start;
            int is_valid = is_valid_equal(input, position);
            while (currentChar != '\0' && !isspace(currentChar)
                   && currentChar != ';')
                currentChar = forward_modif_char(input, position);
            if (is_valid)
            {
                size_t length = *position - start;
                char *word = strndup(input + start, length);
                return createToken(ASSIGNMENT_WORD, word);
            }
        }
        currentChar = forward_modif_char(input, position);
    }
    size_t length = *position - start;
    char *word = strndup(input + start, length);
    return createToken(WORD, word);
}

/*
 * cas d'une newline
 */
struct Token *newline_case(size_t *position)
{
    (*position)++;
    return createToken(NEWLINE, "\\n");
}

/*
 * cas d'un semicolon
 */
struct Token *semicolon_case(size_t *position)
{
    (*position)++;
    return createToken(SEMICOLON, ";");
}

/*
 * cas d'un pipe
 */
struct Token *pipe_case(char *input, size_t *position)
{
    if (input[*position + 1] == '|')
    {
        (*position) += 2;
        return createToken(OR, "||");
    }
    (*position)++;
    return createToken(PIPE, "|");
}

/*
 * cas d'un ampersand
 */
struct Token *ampersand_case(char *input, size_t *position)
{
    if (input[*position + 1] == '&')
    {
        (*position) += 2;
        return createToken(AND, "&&");
    }
    else
    {
        (*position)++;
        return createToken(NULL_TOKEN, NULL);
    }
}

/*
 * cas d'un point
 */
struct Token *dot_case(char *input, size_t *position, char currentChar)
{
    if (!isspace(input[*position + 1]) && input[*position + 1] != '\0')
        return word_case(input, position, currentChar);
    (*position)++;
    return createToken(DOT, ".");
}
