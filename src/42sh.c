#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast/ast.h"
#include "io/io.h"
#include "parser/parser.h"

struct table *table = NULL;

int main(int argc, char **argv)
{
    char *input = processInput(argc, argv);
    if (!input)
    {
        fprintf(stderr, "usage: 42sh [OPTIONS] [SCRIPT] [ARGUMENTS ...]\n");
        exit(2);
    }
    size_t pos = 0;
    struct ast *ast = parse_input(input, &pos);
    int res = 0;
    res = run_ast(ast);
    free_ast(ast);
    free(input);
    return res;
}
