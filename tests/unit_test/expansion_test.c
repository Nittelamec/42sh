#define _POSIX_C_SOURCE  200809L
#include <criterion/criterion.h>
#include <string.h>

#include "../../src/expansion/expansion.h"
#include "../../src/expansion/init_table.h"
#include "../../src/expansion/variable.h"
#include "../../src/ast/ast.h"

Test(simple_single_quote, test1)
{
    struct ast_cmd ast_cmd;
    ast_cmd.base.type = AST_CMD;
    char *str = strdup("pas'mal'ouai");
    char *str1 = strdup("pas''ouai");
    char *str2 = strdup("paslouai");
    char *str3 = strdup("'p'a's'oguai''''");
    char *str4 = strdup("''''");
    ast_cmd.words = malloc(6 * sizeof(char *));
    ast_cmd.words[0] = str;
    ast_cmd.words[1] = str1;
    ast_cmd.words[2] = str2;
    ast_cmd.words[3] = str3;
    ast_cmd.words[4] = str4;
    ast_cmd.words[5] = NULL;
    int res = expand_quote(&ast_cmd);
    cr_assert_eq(res, 0);
    cr_assert_str_eq(ast_cmd.words[0], "pasmalouai");
    cr_assert_str_eq(ast_cmd.words[1], "pasouai");
    cr_assert_str_eq(ast_cmd.words[2], "paslouai");
    cr_assert_str_eq(ast_cmd.words[3], "pasoguai");
    cr_assert_str_eq(ast_cmd.words[4], "");
    free(str);
    free(str1);
    free(str2);
    free(str3);
    free(str4);
    free(ast_cmd.words);
}

Test(simple_single_quote, test2)
{
    struct ast_cmd ast_cmd;
    ast_cmd.base.type = AST_CMD;
    char *str = strdup("test' 'test");
    char *str1 = strdup("test'avec backslash\\' test");
    char *str2 = strdup("p'l'e'i'n'd'e'q'u'o't'e's");
    ast_cmd.words = malloc(4 * sizeof(char *));
    ast_cmd.words[0] = str;
    ast_cmd.words[1] = str1;
    ast_cmd.words[2] = str2;
    ast_cmd.words[3] = NULL;
    int res = expand_quote(&ast_cmd);
    cr_assert_eq(res, 0);
    cr_assert_str_eq(ast_cmd.words[0], "test test");
    cr_assert_str_eq(ast_cmd.words[1], "testavec backslash\\ test");
    cr_assert_str_eq(ast_cmd.words[2], "pleindequotes");
    free(str);
    free(str1);
    free(str2);
    free(ast_cmd.words);
}

Test(simple_double_quote, test1)
{
    struct ast_cmd ast_cmd;
    ast_cmd.base.type = AST_CMD;
    char *str = strdup("pas\"mal\"ouai");
    char *str1 = strdup("pas\"\"ouai");
    char *str2 = strdup("paslouai");
    char *str3 = strdup("\"p\"a\"s\"oguai\"\"\"\"");
    char *str4 = strdup("\"\"\"\"");
    ast_cmd.words = malloc(6 * sizeof(char *));
    ast_cmd.words[0] = str;
    ast_cmd.words[1] = str1;
    ast_cmd.words[2] = str2;
    ast_cmd.words[3] = str3;
    ast_cmd.words[4] = str4;
    ast_cmd.words[5] = NULL;
    int res = expand_quote(&ast_cmd);
    cr_assert_eq(res, 0);
    cr_assert_str_eq(ast_cmd.words[0], "pasmalouai");
    cr_assert_str_eq(ast_cmd.words[1], "pasouai");
    cr_assert_str_eq(ast_cmd.words[2], "paslouai");
    cr_assert_str_eq(ast_cmd.words[3], "pasoguai");
    cr_assert_str_eq(ast_cmd.words[4], "");
    free(str);
    free(str1);
    free(str2);
    free(str3);
    free(str4);
    free(ast_cmd.words);
}

Test(simple_double_quote, test2)
{
    struct ast_cmd ast_cmd;
    ast_cmd.base.type = AST_CMD;
    char *str = strdup("test\" \"test");
    char *str1 = strdup("test\"avec backslash\\\" te\"st");
    char *str2 = strdup("p\"l\"e\"i\"n\"d\"e\"q\"u\"o\"t\"e\"s");
    ast_cmd.words = malloc(4 * sizeof(char *));
    ast_cmd.words[0] = str;
    ast_cmd.words[1] = str1;
    ast_cmd.words[2] = str2;
    ast_cmd.words[3] = NULL;
    int res = expand_quote(&ast_cmd);
    cr_assert_eq(res, 0);
    cr_assert_str_eq(ast_cmd.words[0], "test test");
    cr_assert_str_eq(ast_cmd.words[1], "testavec backslash\" test");
    cr_assert_str_eq(ast_cmd.words[2], "pleindequotes");
    free(str);
    free(str1);
    free(str2);
    free(ast_cmd.words);
}

Test(single_into_double_quote, test1)
{
    struct ast_cmd ast_cmd;
    ast_cmd.base.type = AST_CMD;
    char *str = strdup("pas\"mal'ou\"ai");
    char *str1 = strdup("pas\"test'de single'quote\"ouai");
    char *str2 = strdup("pa\"plei'ndequ'ote\"slouai");
    char *str3 = strdup("\"p''\"a\"s''\"oguai\"''\"\"''\"");
    char *str4 = strdup("\"''\"\"''\"");
    ast_cmd.words = malloc(6 * sizeof(char *));
    ast_cmd.words[0] = str;
    ast_cmd.words[1] = str1;
    ast_cmd.words[2] = str2;
    ast_cmd.words[3] = str3;
    ast_cmd.words[4] = str4;
    ast_cmd.words[5] = NULL;
    int res = expand_quote(&ast_cmd);
    cr_assert_eq(res, 0);
    cr_assert_str_eq(ast_cmd.words[0], "pasmal'ouai");
    cr_assert_str_eq(ast_cmd.words[1], "pastest'de single'quoteouai");
    cr_assert_str_eq(ast_cmd.words[2], "paplei'ndequ'oteslouai");
    cr_assert_str_eq(ast_cmd.words[3], "p''as''oguai''''");
    cr_assert_str_eq(ast_cmd.words[4], "''''");
    free(str);
    free(str1);
    free(str2);
    free(str3);
    free(str4);
    free(ast_cmd.words);
}

Test(single_into_double_quote, test2)
{
    struct ast_cmd ast_cmd;
    ast_cmd.base.type = AST_CMD;
    char *str = strdup("test\" 'ici'\"test");
    char *str1 = strdup("test\"avec 'backs\\'lash\\\" te\"st");
    char *str2 = strdup("p\"l''\"e\"i''\"n\"d''\"e\"q''\"u\"o''\"t\"e''\"s");
    ast_cmd.words = malloc(4 * sizeof(char *));
    ast_cmd.words[0] = str;
    ast_cmd.words[1] = str1;
    ast_cmd.words[2] = str2;
    ast_cmd.words[3] = NULL;
    int res = expand_quote(&ast_cmd);
    cr_assert_eq(res, 0);
    cr_assert_str_eq(ast_cmd.words[0], "test 'ici'test");
    cr_assert_str_eq(ast_cmd.words[1], "testavec 'backs\'lash\" test");
    cr_assert_str_eq(ast_cmd.words[2], "pl''ei''nd''eq''uo''te''s");
    free(str);
    free(str1);
    free(str2);
    free(ast_cmd.words);
}

Test(double_into_single_quote, test1)
{
    struct ast_cmd ast_cmd;
    ast_cmd.base.type = AST_CMD;
    char *str = strdup("pas'mal\"ou'ai");
    char *str1 = strdup("pas'test\"de double\"quote'ouai");
    char *str2 = strdup("pa'plei\"nd'eq'u\"ote'slouai");
    ast_cmd.words = malloc(4 * sizeof(char *));
    ast_cmd.words[0] = str;
    ast_cmd.words[1] = str1;
    ast_cmd.words[2] = str2;
    ast_cmd.words[3] = NULL;
    int res = expand_quote(&ast_cmd);
    cr_assert_eq(res, 0);
    cr_assert_str_eq(ast_cmd.words[0], "pasmal\"ouai");
    cr_assert_str_eq(ast_cmd.words[1], "pastest\"de double\"quoteouai");
    cr_assert_str_eq(ast_cmd.words[2], "paplei\"ndequ\"oteslouai");
    free(str);
    free(str1);
    free(str2);
    free(ast_cmd.words);
}

Test(double_into_single_quote, test2)
{
    struct ast_cmd ast_cmd;
    ast_cmd.base.type = AST_CMD;
    char *str = strdup("test' \"ici\"'test");
    char *str1 = strdup("test'avec \"backs\\\"lash' test");
    char *str2 = strdup("p'l\"'e'i\"'n'd\"'e'q\"'u'o\"'t'e\"'s");
    ast_cmd.words = malloc(4 * sizeof(char *));
    ast_cmd.words[0] = str;
    ast_cmd.words[1] = str1;
    ast_cmd.words[2] = str2;
    ast_cmd.words[3] = NULL;
    int res = expand_quote(&ast_cmd);
    cr_assert_eq(res, 0);
    cr_assert_str_eq(ast_cmd.words[0], "test \"ici\"test");
    cr_assert_str_eq(ast_cmd.words[1], "testavec \"backs\\\"lash test");
    cr_assert_str_eq(ast_cmd.words[2], "pl\"ei\"nd\"eq\"uo\"te\"s");
    free(str);
    free(str1);
    free(str2);
    free(ast_cmd.words);
}

Test(expansion_de_var, test1)
{
    struct ast_cmd *ast_cmd = malloc(sizeof(struct ast_cmd));
    char *str = strdup("$var");
    char *str1 = strdup("test$var");
    ast_cmd->words = malloc(3 * sizeof(char *));
    ast_cmd->base.type = AST_CMD;
    ast_cmd->words[0] = str;
    ast_cmd->words[1] = str1;
    ast_cmd->words[2] = NULL;
    char **argv = malloc(3 * sizeof(char *));
    argv[0] = strdup("var");
    argv[1] = strdup("test");
    argv[2] = NULL;
    struct table *table = init_full_table(0, 2, argv);
    char *test = "test";
    table = table_insert(table, "var", &test);
    ast_cmd = expand_var(ast_cmd, table, 2);
    cr_assert_str_eq(ast_cmd->words[0], "test");
    cr_assert_str_eq(ast_cmd->words[1], "testtest");
    free_ast(&ast_cmd->base);
    free(argv[0]);
    free(argv[1]);
    free(argv);
    table_free(table, 2);
}

Test(expansion_de_var, test2)
{
    struct ast_cmd *ast_cmd = malloc(sizeof(struct ast_cmd));
    char *str = strdup("$var");
    char *str1 = strdup("test$var");
    ast_cmd->words = malloc(3 * sizeof(char *));
    ast_cmd->base.type = AST_CMD;
    ast_cmd->words[0] = str;
    ast_cmd->words[1] = str1;
    ast_cmd->words[2] = NULL;
    char **argv = malloc(3 * sizeof(char *));
    argv[0] = strdup("var");
    argv[1] = strdup("test");
    argv[2] = NULL;
    struct table *table = init_full_table(0, 2, argv);
    char *test = "test";
    char *test1 = "lol";
    table = table_insert(table, "var", &test);
    table = table_insert(table, "var", &test1);
    ast_cmd = expand_var(ast_cmd, table, 2);
    cr_assert_str_eq(ast_cmd->words[0], "lol");
    cr_assert_str_eq(ast_cmd->words[1], "testlol");
    free_ast(&ast_cmd->base);
    free(argv[0]);
    free(argv[1]);
    free(argv);
    table_free(table, 2);
}