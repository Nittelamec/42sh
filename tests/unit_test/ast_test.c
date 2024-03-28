#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <stdio.h>

#include "../../src/ast/ast.h"

Test(ast, print_parse_cmd)
{
    cr_redirect_stdout();
    char *string[] = { "echo", "foo", "bar", NULL };
    struct ast *ast = init_ast_cmd(string);
    print_ast(ast);
    fflush(NULL);
    cr_expect_stdout_eq_str("LIST:^\ [ \"echo\", \"foo\", \"bar\" ]\n");
}

Test(ast, one_print_parse_cmd)
{
    cr_redirect_stdout();
    char *string[] = { "echo", NULL };
    struct ast *ast = init_ast_cmd(string);
    print_ast(ast);
    fflush(NULL);
    cr_expect_stdout_eq_str("CMD: [ \"echo\" ]\n");
}
