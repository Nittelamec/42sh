#ifndef LEXER_DEF_H
#define LEXER_DEF_H

#include <stddef.h>

enum TokenType
{
    WORD, // 0
    OPERATOR, // 1
    NEWLINE, // 2 \n
    COMMENT, // 3 #
    IF, // 4
    THEN, // 5
    ELSE, // 6
    ELIF, // 7
    FI, // 8
    SEMICOLON, // 9 ;
    END_OF_FILE, // 10 \0
    NULL_TOKEN, // 11
    STDIN_STDOUT, // 12 <>
    FILE_DUP_STDOUT, // 13 <&
    FILE_STDIN, // 14 <
    STDOUT_FILE_ADD, // 15 >>
    STDOUT_STDERR_FILE_ADD, // 16 >&
    FORCE_STDOUT_FILE, // 17 >|
    STDOUT_FILE, // 18 >
    PIPE, // 19 |
    NEGATION, // 20 !
    WHILE, // 21
    UNTIL, // 22
    DO, // 23
    DONE, // 24
    AND, // 25 &&
    OR, // 26 ||
    FOR, // 27
    DOUBLE_QUOTE_WORD, // 28
    IONUMBER, // 29
    //    EXIT, // 30
    CD, // 31
    EXPORT, // 32
    CONTINUE, // 33
    BREAK, // 34
    UNSET, // 35
    DOT, // 36
    CASE, // 37
    ASSIGNMENT_WORD, // 38
    IN, // 39
};

struct Token
{
    enum TokenType type;
    char *value;
};

#endif /* !LEXER_DEF_H */
