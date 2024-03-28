#include "free_ast.h"

#include <assert.h>
#include <stdlib.h>

/*
 * This file contains all the functions to free the AST
 */

void free_cmd(struct ast *ast)
{
    assert(ast->type == AST_CMD);
    struct ast_cmd *ast_cmd = (struct ast_cmd *)ast;
    size_t i = 0;
    while (ast_cmd->words[i])
    {
        free(ast_cmd->words[i]);
        i++;
    }
    free(ast_cmd->words);
    free(ast_cmd);
}

void free_list(struct ast *ast)
{
    if (!ast)
        return;
    assert(ast->type == AST_LIST);
    struct ast_list *ast_list = (struct ast_list *)ast;
    free_ast(ast_list->child);
    free_list(&ast_list->next->base);
    free(ast_list);
}

void free_if(struct ast *ast)
{
    assert(ast->type == AST_IF);
    struct ast_if *ast_if = (struct ast_if *)ast;
    free_ast(ast_if->condition);
    free_ast(ast_if->then_body);
    free_ast(ast_if->else_body);
    free(ast_if);
}

void free_redirection(struct ast *ast)
{
    assert(ast->type == AST_REDIR);
    struct ast_redir *ast_redir = (struct ast_redir *)ast;
    free(ast_redir->file);
    free_ast(ast_redir->child);
    free(ast_redir);
}

void free_pipeline(struct ast *ast)
{
    assert(ast->type == AST_PIPELINE);
    struct ast_pipeline *ast_pipeline = (struct ast_pipeline *)ast;
    free_ast(ast_pipeline->left);
    free_ast(ast_pipeline->right);
    free(ast_pipeline);
}

void free_neg(struct ast *ast)
{
    assert(ast->type == AST_NEG);
    struct ast_neg *ast_neg = (struct ast_neg *)ast;
    free_ast(ast_neg->neg_cmd);
    free(ast_neg);
}

void free_and(struct ast *ast)
{
    assert(ast->type == AST_OPERATORS_AND);
    struct ast_operators_and *ast_and = (struct ast_operators_and *)ast;
    free_ast(ast_and->left);
    free_ast(ast_and->right);
    free(ast_and);
}

void free_while_until(struct ast *ast)
{
    assert(ast->type == AST_WHILE || ast->type == AST_UNTIL);
    if (ast->type == AST_WHILE)
    {
        struct ast_while *ast_while = (struct ast_while *)ast;
        free_ast(ast_while->condition);
        free_ast(ast_while->body);
        free(ast_while);
    }
    else if (ast->type == AST_UNTIL)
    {
        struct ast_until *ast_until = (struct ast_until *)ast;
        free_ast(ast_until->condition);
        free_ast(ast_until->body);
        free(ast_until);
    }
}

void free_or(struct ast *ast)
{
    assert(ast->type == AST_OPERATORS_OR);
    struct ast_operators_or *ast_or = (struct ast_operators_or *)ast;
    free_ast(ast_or->left);
    free_ast(ast_or->right);
    free(ast_or);
}

void free_for(struct ast *ast)
{
    assert(ast->type == AST_FOR);
    struct ast_for *ast_for = (struct ast_for *)ast;
    size_t i = 0;
    while (ast_for->words[i])
    {
        free(ast_for->words[i]);
        i++;
    }
    free(ast_for->name);
    free(ast_for->words);
    free_ast(ast_for->loop_body);

    free(ast_for);
}
