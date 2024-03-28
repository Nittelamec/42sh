#ifndef INIT_AST_H
#define INIT_AST_H

#include "ast.h"
#include "init_ast_2.h"

struct ast *init_ast_cmd(char **words);
struct ast *init_ast_list(struct ast *child);
struct ast *init_ast_while(struct ast *condition, struct ast *body);
struct ast *init_ast_if(struct ast *condtion, struct ast *body,
                        struct ast *else_body);
struct ast *init_ast_redir(int fd, char *file, struct ast *ast,
                           enum TokenType type);
struct ast *init_ast_pipeline(struct ast *first_command,
                              struct ast *second_command);
struct ast *init_ast_neg(struct ast *neg_cmd);
struct ast *init_ast_and(struct ast *left, struct ast *right);
struct ast *init_ast_or(struct ast *left, struct ast *right);

#endif /* !INIT_AST_H */
