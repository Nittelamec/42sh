#ifndef LEXER_CASE_2_H
#define LEXER_CASE_2_H

#include <stddef.h>

#include "lexer.h"
#include "lexer_tools.h"

struct Token *double_quote_case(char *input, size_t *position,
                                char currentChar);
struct Token *redirection_to_file_case(char *input, size_t *position,
                                       char currentChar);
struct Token *redirection_to_stdout_case(char *input, size_t *position,
                                         char currentChar);
struct Token *negation_case(char *input, size_t *position, char currentChar);
struct Token *is_ionumber(char *input, size_t *position, char currentChar);
int is_valid_equal(char *input, size_t *position);

#endif /* LEXER_CASE_2_H */
