#include "ast.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../builtins/g_exito.h"
#include "../builtins/g_lasto.h"
#include "../parser/parser.h"

typedef int (*run_type)(struct ast *);

int run_ast(struct ast *ast)
{
    if (!ast)
        return 0;
    if (get_g_exito() >= 0)
        return get_g_exito();
    static const run_type functions[] = {
        [AST_CMD] = &run_cmd,         [AST_LIST] = &run_list,
        [AST_IF] = &run_if,           [AST_PIPELINE] = &run_pipeline,
        [AST_NEG] = &run_neg,         [AST_OPERATORS_AND] = &run_and,
        [AST_OPERATORS_OR] = &run_or, [AST_WHILE] = &run_while,
        [AST_UNTIL] = &run_until,     [AST_REDIR] = &run_redir,
        [AST_FOR] = &run_for
    }; // Simple lookup table for the run functions
    return (*functions[ast->type])(ast);
}

typedef void (*free_type)(struct ast *);

void free_ast(struct ast *ast)
{
    if (!ast)
        return;
    static const free_type ftypes[] = { [AST_CMD] = &free_cmd,
                                        [AST_LIST] = &free_list,
                                        [AST_IF] = &free_if,
                                        [AST_PIPELINE] = &free_pipeline,
                                        [AST_NEG] = &free_neg,
                                        [AST_WHILE] = &free_while_until,
                                        [AST_UNTIL] = &free_while_until,
                                        [AST_OPERATORS_AND] = &free_and,
                                        [AST_OPERATORS_OR] = &free_or,
                                        [AST_REDIR] = &free_redirection,
                                        [AST_FOR] = &free_for };
    (*ftypes[ast->type])(ast);
}
