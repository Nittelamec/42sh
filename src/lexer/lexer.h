#ifndef LEXER_H
#define LEXER_H

#define _POSIX_C_SOURCE 200809L

#include <ctype.h>
#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer_case_1.h"
#include "lexer_case_2.h"

struct Token *getNextToken(char *input, size_t *position);
void freeToken(struct Token *token);
struct Token *lexer_peek(char *input, size_t *position);
void lexer_pop(char *input, size_t *position);
struct Token *createToken(enum TokenType type, char *value);

#endif /* !LEXER_H */
