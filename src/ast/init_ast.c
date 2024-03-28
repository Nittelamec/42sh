#include "init_ast.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "../lexer/lexer.h"

/*
 * This file contains all the functions to initialize the AST nodes
 */

struct ast *init_ast_cmd(char **words)
{
    struct ast_cmd *ast_cmd = malloc(sizeof(struct ast_cmd));
    if (ast_cmd == NULL)
    {
        perror("init_ast_cmd : malloc failed");
        exit(EXIT_FAILURE);
    }
    struct ast base = { AST_CMD };
    ast_cmd->base = base;
    ast_cmd->words = words;
    return &ast_cmd->base;
}

struct ast *push_ast_list(struct ast *ast, struct ast *next)
{
    assert(ast->type == AST_LIST);
    struct ast_list *ast_list = (struct ast_list *)ast;
    while (ast_list->next)
        ast_list = ast_list->next;
    ast_list->next = (struct ast_list *)next;
    return ast;
}

struct ast *init_ast_list(struct ast *child)
{
    if (!child)
        return NULL;
    struct ast_list *ast_list = malloc(sizeof(struct ast_list));
    if (ast_list == NULL)
    {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    struct ast base = { AST_LIST };
    ast_list->base = base;
    ast_list->child = child;
    ast_list->next = NULL;
    return &ast_list->base;
}

struct ast *init_ast_if(struct ast *condition, struct ast *body,
                        struct ast *else_body)
{
    struct ast_if *ast_if = malloc(sizeof(struct ast_if));
    if (ast_if == NULL)
    {
        perror("init_ast_if : malloc failed");
        exit(EXIT_FAILURE);
    }
    struct ast base = { AST_IF };
    ast_if->base = base;
    ast_if->condition = condition;
    ast_if->then_body = body;
    ast_if->else_body = else_body;
    return &ast_if->base;
}

struct ast *init_ast_redir(int fd, char *file, struct ast *child,
                           enum TokenType type)
{
    struct ast_redir *ast_redir = malloc(sizeof(struct ast_redir));
    if (ast_redir == NULL)
    {
        perror("init_ast_redir : malloc failed");
        exit(EXIT_FAILURE);
    }
    struct ast base = { AST_REDIR };
    ast_redir->base = base;
    ast_redir->fd = fd;
    ast_redir->file = file;
    ast_redir->redirect_type = type;
    ast_redir->child = child;
    return &ast_redir->base;
}

struct ast *init_ast_pipeline(struct ast *left, struct ast *right)
{
    struct ast_pipeline *ast_pipeline = malloc(sizeof(struct ast_pipeline));
    if (ast_pipeline == NULL)
    {
        perror("init_ast_pipeline : malloc failed");
        exit(EXIT_FAILURE);
    }
    struct ast base = { AST_PIPELINE };
    ast_pipeline->base = base;
    ast_pipeline->left = left;
    ast_pipeline->right = right;
    return &ast_pipeline->base;
}

struct ast *init_ast_neg(struct ast *neg_cmd)
{
    struct ast_neg *ast_neg = malloc(sizeof(struct ast_neg));
    if (ast_neg == NULL)
    {
        perror("init_ast_neg : malloc failed");
        exit(EXIT_FAILURE);
    }
    struct ast base = { AST_NEG };
    ast_neg->base = base;
    ast_neg->neg_cmd = neg_cmd;
    return &ast_neg->base;
}

struct ast *init_ast_and(struct ast *left, struct ast *right)
{
    struct ast_operators_and *ast_and =
        malloc(sizeof(struct ast_operators_and));
    if (ast_and == NULL)
    {
        perror("init_ast_and: malloc failed");
        exit(EXIT_FAILURE);
    }
    struct ast base = { AST_OPERATORS_AND };
    ast_and->base = base;
    ast_and->left = left;
    ast_and->right = right;
    return &ast_and->base;
}

struct ast *init_ast_or(struct ast *left, struct ast *right)
{
    struct ast_operators_or *ast_or = malloc(sizeof(struct ast_operators_or));
    if (ast_or == NULL)
    {
        perror("init_ast_or : malloc failed");
        exit(EXIT_FAILURE);
    }
    struct ast base = { AST_OPERATORS_OR };
    ast_or->base = base;
    ast_or->left = left;
    ast_or->right = right;
    return &ast_or->base;
}

struct ast *init_ast_while(struct ast *condition, struct ast *body)
{
    struct ast_while *ast_while = malloc(sizeof(struct ast_while));
    if (ast_while == NULL)
    {
        perror("init_ast_while : malloc failed");
        exit(EXIT_FAILURE);
    }
    struct ast base = { AST_WHILE };
    ast_while->base = base;
    ast_while->condition = condition;
    ast_while->body = body;
    return &ast_while->base;
}
