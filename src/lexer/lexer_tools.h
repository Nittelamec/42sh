#ifndef LEXER_TOOLS_H
#define LEXER_TOOLS_H

#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer_def.h"

int is_reservedWord(char *value);
int *is_backslash_then_double_quote(char **value);
void rolCharAtPositions(char **input, int *position);
int is_word_char(char currentChar);
char forward_modif_char(char *input, size_t *position);
char *replaceCharAtPosition(char **input, int position, char newChar);
char *backslash_handle(char *input, size_t *position, char currentChar);

#endif /* LEXER_TOOLS_H */
