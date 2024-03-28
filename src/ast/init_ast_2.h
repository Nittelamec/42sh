#ifndef INIT_AST_2_H
#define INIT_AST_2_H

#include "init_ast.h"

struct ast *init_ast_until(struct ast *condition, struct ast *body);
struct ast *init_ast_for(char *name, char **words, struct ast *loop_body);

#endif /* !INIT_AST_2_H */
