#ifndef FREE_AST_H
#define FREE_AST_H

#include "ast.h"

void free_cmd(struct ast *ast);
void free_list(struct ast *ast);
void free_if(struct ast *ast);
void free_redirection(struct ast *ast);
void free_pipeline(struct ast *ast);
void free_neg(struct ast *ast);
void free_while_until(struct ast *ast);
void free_or(struct ast *ast);
void free_for(struct ast *ast);
void free_and(struct ast *ast);

#endif /* !FREE_AST_H */
