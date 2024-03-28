#include "init_ast_2.h"

struct ast *init_ast_until(struct ast *condition, struct ast *body)
{
    struct ast_until *ast_until = malloc(sizeof(struct ast_until));
    if (ast_until == NULL)
    {
        perror("init_ast_until : malloc failed");
        exit(EXIT_FAILURE);
    }
    struct ast base = { AST_UNTIL };
    ast_until->base = base;
    ast_until->condition = condition;
    ast_until->body = body;
    return &ast_until->base;
}

struct ast *init_ast_for(char *name, char **words, struct ast *loop_body)
{
    struct ast_for *ast_for = malloc(sizeof(struct ast_for));
    if (!ast_for)
    {
        perror("init_ast_until : malloc failed");
        exit(EXIT_FAILURE);
    }
    struct ast base = { AST_FOR };
    ast_for->base = base;
    ast_for->name = name;
    if (!words)
    {
        words = calloc(1, sizeof(char *));
        *words = strdup("$@");
    }
    ast_for->words = words;
    ast_for->loop_body = loop_body;

    return &ast_for->base;
}
