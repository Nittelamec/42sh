#ifndef LEXER_CASE_1_H
#define LEXER_CASE_1_H

#define _POSIX_C_SOURCE 200809L

#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>

#include "lexer.h"
#include "lexer_case_2.h"
#include "lexer_tools.h"

struct Token defineWord(char *value, struct Token *dictionary);
struct Token *comment_case(char *input, size_t *position, char currentChar);
struct Token *single_quote_case(char *input, size_t *position,
                                char currentChar);
struct Token *word_case(char *input, size_t *position, char currentChar);
struct Token *newline_case(size_t *position);
struct Token *semicolon_case(size_t *position);
struct Token *pipe_case(char *input, size_t *position);
struct Token *ampersand_case(char *input, size_t *position);
struct Token *dot_case(char *input, size_t *position, char currentChar);

#endif /* LEXER_CASE_1_H */
