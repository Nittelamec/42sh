#ifndef AST_TYPE_H
#define AST_TYPE_H

#include "../lexer/lexer.h"

//****************** Structure of the Abstract Syntax Tree. ********************
enum ast_type
{
    AST_CMD,
    AST_LIST,
    AST_IF,
    AST_REDIR,
    AST_PIPELINE,
    AST_NEG,
    AST_OPERATORS_AND,
    AST_OPERATORS_OR,
    AST_WHILE,
    AST_UNTIL,
    AST_FOR
};

struct ast
{
    enum ast_type type;
};

struct ast_cmd
{
    struct ast base;
    char **words; // end with a NULL
};

struct ast_list
{
    struct ast base;
    struct ast *child;
    struct ast_list *next; // simply linked ast list
};

struct ast_if
{
    struct ast base;
    struct ast *condition; // the condition
    struct ast *then_body; // the body of the then clause
    struct ast *else_body; // the body of the else, may be NULL
};

struct ast_redir
{
    struct ast base;
    enum TokenType redirect_type;
    int fd; // optional file descriptor, from 0 to 9 inclusive
    char *file;
    struct ast *child;
};

struct ast_pipeline
{
    struct ast base;
    struct ast *left;
    struct ast *right;
};

struct ast_neg
{
    struct ast base;
    struct ast *neg_cmd;
};

struct ast_operators_and
{
    struct ast base;
    struct ast *left;
    struct ast *right;
};

struct ast_operators_or
{
    struct ast base;
    struct ast *left;
    struct ast *right;
};

struct ast_while
{
    struct ast base;
    struct ast *condition;
    struct ast *body;
};

struct ast_until
{
    struct ast base;
    struct ast *condition;
    struct ast *body;
};

struct ast_for
{
    struct ast base;
    char *name;
    char **words; // dans quoi on itère, à expand
    struct ast *loop_body; // peut être NULL
};

//******************************************************************************

#endif /* !AST_TYPE_H */
