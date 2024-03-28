#define _POSIX_C_SOURCE 200809L

#include "parser_2.h"

#include <string.h>

/**************************Annexe-functions************************************/

int is_redirect_op(enum TokenType type)
{
    return (type == STDIN_STDOUT || type == FILE_DUP_STDOUT
            || type == FILE_STDIN || type == STDOUT_FILE_ADD
            || type == STDOUT_STDERR_FILE_ADD || type == FORCE_STDOUT_FILE
            || type == STDOUT_FILE);
}

void verify_zero_or_more(char *input, size_t *position,
                         enum TokenType type_to_check)
{
    struct Token *next_token = lexer_peek(input, position);
    while (next_token->type == type_to_check)
    {
        lexer_pop(input, position);
        freeToken(next_token);
        next_token = lexer_peek(input, position);
    }
    freeToken(next_token);
}

static void free_and_pop(char *input, size_t *position, struct Token *token)
{
    freeToken(token);
    lexer_pop(input, position);
}

static void type_error(struct Token *token)
{
    fprintf(stderr, "42sh: syntax error near unexpected token %d", token->type);
    freeToken(token);
    exit(2);
}

/**************************Parsing-functions***********************************/

struct ast *parse_input(char *input, size_t *position)
{
    struct Token *next_token = lexer_peek(input, position);
    if (next_token->type == NEWLINE || next_token->type == END_OF_FILE)
    {
        lexer_pop(input, position);
        freeToken(next_token);
        return NULL;
    }
    freeToken(next_token);
    struct ast *ast = parse_list(input, position);
    next_token = lexer_peek(input, position);
    if (next_token->type == NEWLINE || next_token->type == END_OF_FILE)
    {
        freeToken(next_token);
        return ast;
    }
    else
    {
        fprintf(stderr, "42sh: syntax error near unexpected token %d",
                next_token->type);
        freeToken(next_token);
        exit(2);
    }
}

struct ast *push_ast_list(struct ast *ast, struct ast *next);
struct ast *parse_list(char *input, size_t *position)
{
    struct ast *child = parse_and_or(input, position);
    struct ast *ast_list = init_ast_list(child);

    struct Token *next_token = lexer_peek(input, position);
    while (next_token->type == SEMICOLON)
    {
        lexer_pop(input, position);
        child = parse_and_or(input, position);
        if (!child)
            break;
        struct ast *next = init_ast_list(child);
        ast_list = push_ast_list(ast_list, next);
        freeToken(next_token);
        next_token = lexer_peek(input, position);
    }
    freeToken(next_token);
    return ast_list;
}

struct ast *parse_and_or(char *input, size_t *position)
{
    struct ast *ast_res = parse_pipeline(input, position);
    struct Token *next_token = lexer_peek(input, position);
    while (next_token->type == AND || next_token->type == OR)
    {
        lexer_pop(input, position);
        verify_zero_or_more(input, position, NEWLINE);
        struct ast *ast_right = parse_pipeline(input, position);
        if (next_token->type == AND)
            ast_res = init_ast_and(ast_res, ast_right);
        else
            ast_res = init_ast_or(ast_res, ast_right);
        freeToken(next_token);
        next_token = lexer_peek(input, position);
    }
    freeToken(next_token);
    return ast_res;
}

struct ast *parse_pipeline(char *input, size_t *position)
{
    struct Token *next_token = lexer_peek(input, position);
    int neg = 0;
    if (next_token->type == NEGATION)
    {
        neg = 1;
        lexer_pop(input, position);
    }
    freeToken(next_token);
    struct ast *ast_left = parse_command(input, position);
    next_token = lexer_peek(input, position);
    if (next_token->type == PIPE)
    {
        freeToken(next_token);
        lexer_pop(input, position);
        verify_zero_or_more(input, position, NEWLINE);
        struct ast *ast_right = parse_pipeline(input, position);
        struct ast *res = init_ast_pipeline(ast_left, ast_right);
        return (neg) ? init_ast_neg(res) : res;
    }
    freeToken(next_token);
    return (neg) ? init_ast_neg(ast_left) : ast_left;
}

static void blabla(struct Token **next_token, char ***words, char *input,
                   size_t *position)
{
    size_t i = 0;
    while ((*next_token)->type == WORD)
    {
        (*words)[i++] = strdup((*next_token)->value);
        (*words) = realloc(*words, (i + 1) * sizeof(char *));
        (*words)[i] = NULL; // the array is always NULL terminated
        free_and_pop(input, position, *next_token);
        (*next_token) = lexer_peek(input, position);
    }
}

struct ast *parse_rule_for(char *input, size_t *position)
{
    struct Token *next_token = lexer_peek(input, position);
    if (next_token->type != FOR)
        type_error(next_token);
    free_and_pop(input, position, next_token);

    next_token = lexer_peek(input, position);
    if (next_token->type != WORD)
        type_error(next_token);
    char *name = strdup(next_token->value);
    free_and_pop(input, position, next_token);

    verify_zero_or_more(input, position, NEWLINE);
    next_token = lexer_peek(input, position);

    char **words = NULL;
    if (next_token->type == IN)
    {
        words = calloc(1, sizeof(char *));
        free_and_pop(input, position, next_token);
        next_token = lexer_peek(input, position);
        blabla(&next_token, &words, input, position);
        if (next_token->type != SEMICOLON && next_token->type != NEWLINE)
            type_error(next_token);
        free_and_pop(input, position, next_token);
    }
    else if (next_token->type == SEMICOLON)
        free_and_pop(input, position, next_token);

    verify_zero_or_more(input, position, NEWLINE);
    next_token = lexer_peek(input, position);
    if (next_token->type != DO)
        type_error(next_token);
    free_and_pop(input, position, next_token);
    struct ast *loop_body = parse_compound_list(input, position);
    if (!loop_body)
        type_error(next_token);

    next_token = lexer_peek(input, position);
    if (next_token->type != DONE)
        type_error(next_token);
    free_and_pop(input, position, next_token);

    return init_ast_for(name, words, loop_body);
}
