#ifndef AST_H
#define AST_H

#include "ast_type.h"

/*
 * Includes for the different ast types
 */
#include "free_ast.h"
#include "init_ast.h"
#include "run_ast.h"

/*
 * Print the AST when giving the "--pretty-print" argument, see README.txt
 */

/*
 * Run the AST.
 * It forks for non builtins function
 */
int run_ast(struct ast *ast);

/*
 * Destroy the entire AST
 */
void free_ast(struct ast *ast);

#endif /* !AST_H */
