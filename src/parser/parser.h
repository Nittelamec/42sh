#ifndef PARSER_H
#define PARSER_H

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "../ast/ast.h"
#include "../lexer/lexer.h"
#include "parser_2.h"

/*
 * The functions below parse the input according to the Shell grammar
 */
struct ast *parse_else_clause(char *input, size_t *position);
struct ast *parse_shell_command(char *input, size_t *position);
struct ast *parse_rule_if(char *input, size_t *position);
struct ast *parse_redirection(char *input, size_t *position);
struct ast *parse_compound_list(char *input, size_t *position);
struct ast *parse_command(char *input, size_t *position);
struct ast *parse_simple_command(char *input, size_t *position);
struct ast *parse_rule_until(char *input, size_t *position);
struct ast *parse_rule_while(char *input, size_t *position);
struct ast *parse_prefix(char *input, size_t *position);

#endif /* !PARSER_H */
