#ifndef PARSER_2_H
#define PARSER_2_H

#include "parser.h"

char *token_to_char(struct Token *token);
int is_redirect_op(enum TokenType type);
void verify_zero_or_more(char *input, size_t *position,
                         enum TokenType type_to_check);
struct ast *parse_input(char *input, size_t *position);
struct ast *push_ast_list(struct ast *ast, struct ast *next);
struct ast *parse_list(char *input, size_t *position);
struct ast *parse_and_or(char *input, size_t *position);
struct ast *parse_pipeline(char *input, size_t *position);
struct ast *parse_rule_for(char *input, size_t *position);

#endif /* PARSER_2_H */
