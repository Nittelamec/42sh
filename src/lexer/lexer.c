#include "lexer.h"

#include "g_begin.h"
#include "lexer_def.h"

static struct Token keywordDictionary[] = {
    { IF, "if" },         { THEN, "then" }, { ELSE, "else" },
    { ELIF, "elif" },     { FI, "fi" },     { WHILE, "while" },
    { UNTIL, "until" },   { DO, "do" },     { DONE, "done" },
    { FOR, "for" },       { CASE, "case" }, { IN, "in" },
    { NULL_TOKEN, NULL },
};

/*
 * permet de liberer un token
 */
void freeToken(struct Token *token)
{
    if (token->type != NULL_TOKEN
        && (is_reservedWord(token->value) == 0
            || (is_reservedWord(token->value) == 1 && get_g_begin() == 0)))
        free(token->value);
    free(token);
}

/*
 * prend un token sans avancer le consommer dans le stream de token
 */
struct Token *lexer_peek(char *input, size_t *position)
{
    size_t save_pos = *position;
    struct Token *token = getNextToken(input, position);
    *position = save_pos;
    return token;
}

/*
 * Consumes a token
 */
void lexer_pop(char *input, size_t *position)
{
    static enum TokenType prev_type = NULL_TOKEN;
    struct Token *token = getNextToken(input, position);
    enum TokenType actual = token->type;
    freeToken(token);
    if (actual == NEWLINE || actual == SEMICOLON)
        set_g_begin();
    else if (actual == IF || actual == ELIF || actual == ELSE || actual == THEN
             || actual == FI || actual == DO || actual == WHILE
             || actual == UNTIL || actual == DONE || actual == NEGATION)
        set_g_begin();
    else if (prev_type == FOR)
        set_g_begin();
    else
        unset_g_begin();
    prev_type = actual;
}

/*
 * cree un Token
 */
struct Token *createToken(enum TokenType type, char *value)
{
    struct Token tmp;
    tmp.value = NULL;
    if (type == WORD && get_g_begin() == 1)
    {
        tmp = defineWord(value, keywordDictionary);
    }
    else if (type == DOUBLE_QUOTE_WORD)
    {
        int *indexOfBackSlash = is_backslash_then_double_quote(&value);
        rolCharAtPositions(&value, indexOfBackSlash);
        type = WORD;
        free(indexOfBackSlash);
    }
    struct Token *token = malloc(sizeof(struct Token));
    if (token == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    if (tmp.value != NULL)
    {
        token->type = tmp.type;
        token->value = tmp.value;
    }
    else
    {
        token->type = type;
        token->value = strdup(value);
    }
    if (token->value == NULL)
    {
        perror("strdup");
        exit(EXIT_FAILURE);
    }
    if (type == WORD || (is_reservedWord(value) == 1 && get_g_begin())
        || type == IONUMBER || type == ASSIGNMENT_WORD)
        free((void *)value);
    return token;
}

/*
 * retourne le prochain token
 */
struct Token *getNextToken(char *input, size_t *position)
{
    char currentChar = input[*position];
    while (currentChar == ' ' || currentChar == '\t')
        currentChar = forward_modif_char(input, position);
    if (currentChar == '\0')
        return createToken(END_OF_FILE, "\\0");
    else if (currentChar == '#')
        return comment_case(input, position, currentChar);

    else if (isdigit(currentChar))
        return is_ionumber(input, position, currentChar);

    else if (currentChar == '.')
        return dot_case(input, position, currentChar);

    else if (is_word_char(currentChar))
        return word_case(input, position, currentChar);

    else if (currentChar == '\n')
        return newline_case(position);

    else if (currentChar == ';')
        return semicolon_case(position);

    else if (currentChar == '\'')
        return single_quote_case(input, position, currentChar);

    else if (currentChar == '<')
        return redirection_to_file_case(input, position, currentChar);

    else if (currentChar == '>')
        return redirection_to_stdout_case(input, position, currentChar);

    else if (currentChar == '|')
        return pipe_case(input, position);

    else if (currentChar == '&')
        return ampersand_case(input, position);
    else if (currentChar == '!')
        return negation_case(input, position, currentChar);
    return createToken(NULL_TOKEN, NULL);
}

/*int main(int argc, char **argv)
{
    size_t position = 0;
    char *input = "a=b";
    struct Token *token = getNextToken(input, &position);
    while (token->type != END_OF_FILE && token->type != NULL_TOKEN)
    {
        printf("type: %d\n", token->type);
        printf("value: %s\n", token->value);
        freeToken(token);
        token = getNextToken(input, &position);
    }
    freeToken(token);
    return 0;
}*/
