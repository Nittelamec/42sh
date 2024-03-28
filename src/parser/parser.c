#define _POSIX_C_SOURCE 200809L
#include "parser.h"

#include "../expansion/init_table.h"

extern struct table *table;

struct ast *parse_command(char *input, size_t *position)
{
    struct Token *next_token = lexer_peek(input, position);
    if (next_token->type == WORD || is_redirect_op(next_token->type)
        || next_token->type == ASSIGNMENT_WORD
        || next_token->type
            == IONUMBER) // maybe mettre plus de tyype MAYBE IONUMBER
    {
        freeToken(next_token);
        return parse_simple_command(input, position);
    }
    else if (next_token->type == IF || next_token->type == WHILE
             || next_token->type == UNTIL || next_token->type == FOR)
    {
        freeToken(next_token);
        struct ast *ast_shell_command = parse_shell_command(input, position);
        struct ast_redir *ast_redir =
            (struct ast_redir *)parse_redirection(input, position);
        struct ast_redir *ast_redir_pred = ast_redir;
        struct ast_redir *ast_redir_root = ast_redir;
        while (ast_redir != NULL)
        {
            ast_redir->child = parse_redirection(input, position);
            ast_redir_pred = ast_redir;
            ast_redir = (struct ast_redir *)ast_redir->child;
        }
        if (ast_redir_pred == NULL)
            return ast_shell_command;
        ast_redir_pred->child = ast_shell_command;
        return &ast_redir_root->base;
    }
    else
    {
        freeToken(next_token);
        return NULL;
    }
}

struct ast *parse_shell_command(char *input, size_t *position)
{
    struct Token *next_token = lexer_peek(input, position);
    if (next_token->type == IF)
    {
        freeToken(next_token);
        return parse_rule_if(input, position);
    }
    else if (next_token->type == WHILE)
    {
        freeToken(next_token);
        return parse_rule_while(input, position);
    }
    else if (next_token->type == FOR)
    {
        freeToken(next_token);
        return parse_rule_for(input, position);
    }
    else
    {
        freeToken(next_token);
        return parse_rule_until(input, position);
    }
}

static void add_variable(char *words_to_assign)
{
    char *key = strtok(words_to_assign, "=");
    char *value = strtok(NULL, "");
    table = table_insert(table, key, &value);
}

struct ast *parse_prefix(char *input, size_t *position)
{
    struct Token *next_token = lexer_peek(input, position);
    if (next_token->type == ASSIGNMENT_WORD)
    {
        add_variable(next_token->value);
        freeToken(next_token);
        lexer_pop(input, position);
        return NULL;
    }
    freeToken(next_token);
    return parse_redirection(input, position);
}

/******************************************************************************/

static struct ast_redir *check_redir(char *input, size_t *position)
{
    struct ast_redir *ast_redir =
        (struct ast_redir *)parse_prefix(input, position);
    struct ast_redir *ast_redir_root = ast_redir;
    while (ast_redir != NULL)
    {
        ast_redir->child = parse_redirection(input, position);
        ast_redir = (struct ast_redir *)ast_redir->child;
    }

    return ast_redir_root;
}

static struct ast_redir *check_redir_word(char *input, size_t *position,
                                          char ***words)
{
    int i = 0;
    int first_initialize = 0;
    struct Token *next_token = lexer_peek(input, position);
    struct ast_redir *ast_redir2 = NULL;
    struct ast_redir *ast_redir_root2 = NULL;
    while (next_token->type == WORD || ast_redir2 != NULL
           || (!first_initialize
               && (is_redirect_op(next_token->type)
                   || next_token->type == IONUMBER))) // IONUMBER?
    {
        if (next_token->type == WORD)
        {
            (*words)[i++] = strdup(next_token->value);
            (*words) = realloc(*words, (i + 1) * sizeof(char *));
            (*words)[i] = NULL; // the array is always NULL terminated
            lexer_pop(input, position);
        }
        else
        {
            if (first_initialize)
            {
                ast_redir2->child = parse_redirection(input, position);
                ast_redir2 = (struct ast_redir *)ast_redir2->child;
            }
            else
            {
                ast_redir2 = (struct ast_redir *)parse_prefix(input, position);
                ast_redir_root2 = ast_redir2;
            }
            first_initialize = 1;
        }
        freeToken(next_token);
        next_token = lexer_peek(input, position);
    }
    freeToken(next_token);

    return ast_redir_root2;
}

static void push_ast_redir(struct ast_redir *redir, struct ast *child)
{
    while (redir->child)
    {
        assert(redir->child->type == AST_REDIR);
        redir = (struct ast_redir *)redir->child;
    }
    redir->child = child;
}

static struct ast *raccommodage(struct ast_redir *redir1,
                                struct ast_redir *redir2, struct ast *cmd)
{
    if (!redir1)
    {
        if (!redir2)
            return cmd;
        push_ast_redir(redir2, cmd);
        return &redir2->base;
    }
    else if (!redir2)
    {
        push_ast_redir(redir1, cmd);
        return &redir1->base;
    }
    else
    {
        push_ast_redir(redir1, &redir2->base);
        push_ast_redir(redir1, cmd);
        return &redir1->base;
    }
}

struct ast *parse_simple_command(char *input, size_t *position)
{
    struct ast_redir *ast_redir = check_redir(input, position);
    struct Token *next_token = lexer_peek(input, position);
    if (next_token->type != WORD)
    {
        freeToken(next_token);
        if (!ast_redir)
            return NULL;
        return &ast_redir->base;
    }
    freeToken(next_token);
    char **words = calloc(1, sizeof(char *));
    struct ast_redir *ast_redir2 = check_redir_word(input, position, &words);
    struct ast *ast_cmd = init_ast_cmd(words);
    struct ast *result = raccommodage(ast_redir, ast_redir2, ast_cmd);

    return result;
}

/******************************************************************************/

struct ast *parse_redirection(char *input, size_t *position)
{
    struct Token *next_token = lexer_peek(input, position);
    int fd = -1; // No fd precised for now
    if (next_token->type == IONUMBER)
    {
        fd = atoi(next_token->value); // get the file descriptor
        lexer_pop(input, position);
    }

    freeToken(next_token);
    next_token = lexer_peek(input, position);

    if (!is_redirect_op(next_token->type) && fd == -1)
    {
        freeToken(next_token);
        return NULL;
    }
    enum TokenType type = next_token->type;
    lexer_pop(input, position);
    freeToken(next_token);
    next_token = lexer_peek(input, position);

    if (next_token->type != WORD)
    {
        fprintf(stderr, "42sh: syntax error near unexpected token %d",
                next_token->type);
        freeToken(next_token);
        exit(2);
    }
    char *word = strdup(next_token->value);
    lexer_pop(input, position);
    freeToken(next_token);

    return init_ast_redir(fd, word, NULL, type);
}

struct ast *parse_rule_if(char *input, size_t *position)
{
    struct Token *next_token = lexer_peek(input, position);
    if (next_token->type != IF)
    {
        fprintf(stderr, "42sh: syntax error near unexpected token %d",
                next_token->type);
        freeToken(next_token);
        exit(2);
    }
    freeToken(next_token);
    lexer_pop(input, position);
    struct ast *condition = parse_compound_list(input, position);
    next_token = lexer_peek(input, position);
    if (next_token->type != THEN || !condition)
    {
        fprintf(stderr, "42sh: syntax error near unexpected token %d",
                next_token->type);
        freeToken(next_token);
        free_ast(condition);
        exit(2);
    }
    freeToken(next_token);
    lexer_pop(input, position);
    struct ast *body = parse_compound_list(input, position);
    struct ast *else_body = parse_else_clause(input, position); // may be null
    next_token = lexer_peek(input, position);
    if (next_token->type != FI || !body)
    {
        fprintf(stderr, "42sh: syntax error near unexpected token %d",
                next_token->type);
        freeToken(next_token);
        free_ast(condition);
        free_ast(body);
        free_ast(else_body);
        exit(2);
    }
    freeToken(next_token);
    lexer_pop(input, position);
    return init_ast_if(condition, body, else_body);
}

struct ast *parse_else_clause(char *input, size_t *position)
{
    struct Token *next_token = lexer_peek(input, position);
    if (next_token->type == ELSE)
    {
        freeToken(next_token);
        lexer_pop(input, position);
        return parse_compound_list(input, position);
    }
    else if (next_token->type == ELIF)
    {
        freeToken(next_token);
        lexer_pop(input, position);
        struct ast *elif_condition = parse_compound_list(input, position);
        next_token = lexer_peek(input, position);
        if (next_token->type != THEN)
        {
            fprintf(stderr, "42sh: syntax error near unexpected token %d",
                    next_token->type);
            freeToken(next_token);
            free_ast(elif_condition);
            exit(2);
        }
        freeToken(next_token);
        lexer_pop(input, position);
        struct ast *elif_body = parse_compound_list(input, position);
        struct ast *elif_else_body =
            parse_else_clause(input, position); // may be null
        return init_ast_if(elif_condition, elif_body, elif_else_body);
    }
    freeToken(next_token);
    return NULL;
}

struct ast *parse_compound_list(char *input, size_t *position)
{
    verify_zero_or_more(input, position, NEWLINE);
    struct ast *child = parse_and_or(input, position);
    struct ast *ast_list = init_ast_list(child);
    struct Token *next_token = lexer_peek(input, position);
    while (next_token->type == SEMICOLON || next_token->type == NEWLINE)
    {
        lexer_pop(input, position);
        verify_zero_or_more(input, position, NEWLINE);
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

struct ast *parse_rule_while(char *input, size_t *position)
{
    struct Token *next_token = lexer_peek(input, position);
    if (next_token->type != WHILE)
    {
        fprintf(stderr, "42sh: syntax error near unexpected token %d",
                next_token->type);
        freeToken(next_token);
        exit(2);
    }
    lexer_pop(input, position);
    freeToken(next_token);
    struct ast *while_cond = parse_compound_list(input, position);
    next_token = lexer_peek(input, position);
    if (next_token->type != DO || !while_cond)
    {
        fprintf(stderr, "42sh: syntax error near unexpected token %d",
                next_token->type);
        freeToken(next_token);
        exit(2);
    }
    lexer_pop(input, position);
    freeToken(next_token);
    struct ast *while_body = parse_compound_list(input, position);
    next_token = lexer_peek(input, position);
    if (next_token->type != DONE || !while_body)
    {
        fprintf(stderr, "42sh: syntax error near unexpected token %d",
                next_token->type);
        freeToken(next_token);
        exit(2);
    }
    lexer_pop(input, position);
    freeToken(next_token);
    return init_ast_while(while_cond, while_body);
}

struct ast *parse_rule_until(char *input, size_t *position)
{
    struct Token *next_token = lexer_peek(input, position);
    if (next_token->type != UNTIL)
    {
        fprintf(stderr, "42sh: syntax error near unexpected token %d",
                next_token->type);
        freeToken(next_token);
        exit(2);
    }
    lexer_pop(input, position);
    freeToken(next_token);
    struct ast *until_cond = parse_compound_list(input, position);
    next_token = lexer_peek(input, position);
    if (next_token->type != DO || !until_cond)
    {
        fprintf(stderr, "42sh: syntax error near unexpected token %d",
                next_token->type);
        freeToken(next_token);
        exit(2);
    }
    freeToken(next_token);
    lexer_pop(input, position);
    struct ast *until_body = parse_compound_list(input, position);
    next_token = lexer_peek(input, position);
    if (next_token->type != DONE || !until_body)
    {
        fprintf(stderr, "42sh: syntax error near unexpected token %d",
                next_token->type);
        freeToken(next_token);
        exit(2);
    }
    lexer_pop(input, position);
    freeToken(next_token);
    return init_ast_until(until_cond, until_body);
}
