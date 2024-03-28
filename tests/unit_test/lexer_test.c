#include <criterion/criterion.h>

#include "../../src/io/io.h"
#include "../../src/lexer/lexer.h"

struct Token *token = NULL;

Test(lexer, getToken1)
{
    char *test = "echo hello";
    size_t position = 0;
    token = getNextToken(test, &position);
    cr_assert_eq(token->type, WORD);
    cr_assert_str_eq(token->value, "echo");
    freeToken(token);

    token = getNextToken(test, &position);
    cr_assert_eq(token->type, WORD);
    cr_assert_str_eq(token->value, "hello");
    freeToken(token);
}

Test(lexer, getToken2)
{
    size_t position2 = 0;
    char *test2 = "echo hello;";
    token = getNextToken(test2, &position2);
    cr_assert_eq(token->type, WORD);
    cr_assert_str_eq(token->value, "echo");
    freeToken(token);

    token = getNextToken(test2, &position2);
    cr_assert_eq(token->type, WORD);
    cr_assert_str_eq(token->value, "hello");
    freeToken(token);

    token = getNextToken(test2, &position2);
    cr_assert_eq(token->type, SEMICOLON);
    cr_assert_str_eq(token->value, ";");
    freeToken(token);
}

Test(lexer, getToken3)
{
    size_t position3 = 0;
    char *test3 = "echo ;hello #test\n";
    token = getNextToken(test3, &position3);
    cr_assert_eq(token->type, WORD);
    cr_assert_str_eq(token->value, "echo");
    freeToken(token);

    token = getNextToken(test3, &position3);
    cr_assert_eq(token->type, SEMICOLON);
    cr_assert_str_eq(token->value, ";");
    freeToken(token);

    token = getNextToken(test3, &position3);
    cr_assert_eq(token->type, WORD);
    cr_assert_str_eq(token->value, "hello");
    freeToken(token);
}

Test(lexer, getToken4)
{
    size_t position4 = 0;
    char *test4 = "echo if\t;hello fi then else #test\n";
    token = getNextToken(test4, &position4);
    cr_assert_eq(token->type, WORD);
    cr_assert_str_eq(token->value, "echo");
    freeToken(token);

    token = getNextToken(test4, &position4);
    cr_assert_eq(token->type, IF);
    cr_assert_str_eq(token->value, "if");
    freeToken(token);

    token = getNextToken(test4, &position4);
    cr_assert_eq(token->type, SEMICOLON);
    cr_assert_str_eq(token->value, ";");
    freeToken(token);

    token = getNextToken(test4, &position4);
    cr_assert_eq(token->type, WORD);
    cr_assert_str_eq(token->value, "hello");
    freeToken(token);

    token = getNextToken(test4, &position4);
    cr_assert_eq(token->type, FI);
    cr_assert_str_eq(token->value, "fi");
    freeToken(token);

    token = getNextToken(test4, &position4);
    cr_assert_eq(token->type, THEN);
    cr_assert_str_eq(token->value, "then");
    freeToken(token);

    token = getNextToken(test4, &position4);
    cr_assert_eq(token->type, ELSE);
    cr_assert_str_eq(token->value, "else");
    freeToken(token);

    token = getNextToken(test4, &position4);
    cr_assert_eq(token->type, NEWLINE);
    cr_assert_str_eq(token->value, "\\n");
    freeToken(token);
}

/*Test(processInput_and_getNextToken, should_return_expected_token)
{
    int argc = 2;
    char *argv[2] = { "./42sh", "script.sh" };
    size_t position = 0;
    char *input = processInput(argc, argv);
    struct Token *token = getNextToken(input, &position);
    cr_assert_eq(token->type, IF);
    cr_assert_str_eq(token->value, "if");
    freeToken(token);

    token = getNextToken(input, &position);
    cr_assert_eq(token->type, THEN);
    cr_assert_str_eq(token->value, "then");
    freeToken(token);

    token = getNextToken(input, &position);
    cr_assert_eq(token->type, NEWLINE);
    cr_assert_str_eq(token->value, "\n");
    freeToken(token);

    token = getNextToken(input, &position);
    cr_assert_eq(token->type, ELSE);
    cr_assert_str_eq(token->value, "else");
    freeToken(token);
    free(input);
}*/

Test(redirect_stdin_stdout, should_return_expected_token)
{
    size_t position = 0;
    char *test = "<>";
    token = getNextToken(test, &position);
    cr_assert_eq(token->type, STDIN_STDOUT);
    cr_assert_str_eq(token->value, "<>");
    freeToken(token);
}

Test(redirect_file_dup_stdout, should_return_expected_token)
{
    size_t position = 0;
    char *test = "<&";
    token = getNextToken(test, &position);
    cr_assert_eq(token->type, FILE_DUP_STDOUT);
    cr_assert_str_eq(token->value, "<&");
    freeToken(token);
}

Test(redirect_file_stdin, should_return_expected_token)
{
    size_t position = 0;
    char *test = "<";
    token = getNextToken(test, &position);
    cr_assert_eq(token->type, FILE_STDIN);
    cr_assert_str_eq(token->value, "<");
    freeToken(token);
}

Test(redirect_stdout_file_add, should_return_expected_token)
{
    size_t position = 0;
    char *test = ">>";
    token = getNextToken(test, &position);
    cr_assert_eq(token->type, STDOUT_FILE_ADD);
    cr_assert_str_eq(token->value, ">>");
    freeToken(token);
}

Test(redirect_stdout_stderr_file_add, should_return_expected_token)
{
    size_t position = 0;
    char *test = ">&";
    token = getNextToken(test, &position);
    cr_assert_eq(token->type, STDOUT_STDERR_FILE_ADD);
    cr_assert_str_eq(token->value, ">&");
    freeToken(token);
}

Test(redirect_force_stdout_file, should_return_expected_token)
{
    size_t position = 0;
    char *test = ">|";
    token = getNextToken(test, &position);
    cr_assert_eq(token->type, FORCE_STDOUT_FILE);
    cr_assert_str_eq(token->value, ">|");
    freeToken(token);
}

Test(redirect_stdout_file, should_return_expected_token)
{
    size_t position = 0;
    char *test = ">";
    token = getNextToken(test, &position);
    cr_assert_eq(token->type, STDOUT_FILE);
    cr_assert_str_eq(token->value, ">");
    freeToken(token);
}

Test(redirect_tricky_case_1, should_return_expected_token)
{
    size_t position = 0;
    char *test = ">>>&>|><><&<";
    token = getNextToken(test, &position);
    cr_assert_eq(token->type, STDOUT_FILE_ADD);
    cr_assert_str_eq(token->value, ">>");
    freeToken(token);

    token = getNextToken(test, &position);
    cr_assert_eq(token->type, STDOUT_STDERR_FILE_ADD);
    cr_assert_str_eq(token->value, ">&");
    freeToken(token);

    token = getNextToken(test, &position);
    cr_assert_eq(token->type, FORCE_STDOUT_FILE);
    cr_assert_str_eq(token->value, ">|");
    freeToken(token);

    token = getNextToken(test, &position);
    cr_assert_eq(token->type, STDOUT_FILE);
    cr_assert_str_eq(token->value, ">");
    freeToken(token);

    token = getNextToken(test, &position);
    cr_assert_eq(token->type, STDIN_STDOUT);
    cr_assert_str_eq(token->value, "<>");
    freeToken(token);

    token = getNextToken(test, &position);
    cr_assert_eq(token->type, FILE_DUP_STDOUT);
    cr_assert_str_eq(token->value, "<&");
    freeToken(token);

    token = getNextToken(test, &position);
    cr_assert_eq(token->type, FILE_STDIN);
    cr_assert_str_eq(token->value, "<");
    freeToken(token);
}

Test(pipe_case, should_return_expected_token)
{
    size_t position = 0;
    char *test = "|";
    token = getNextToken(test, &position);
    cr_assert_eq(token->type, PIPE);
    cr_assert_str_eq(token->value, "|");
    freeToken(token);
}

Test(negation_case, should_return_expected_token)
{
    size_t position = 0;
    char *test = "!";
    token = getNextToken(test, &position);
    cr_assert_eq(token->type, NEGATION);
    cr_assert_str_eq(token->value, "!");
    freeToken(token);
}

Test(while_case, should_return_expected_token)
{
    size_t position = 0;
    char *test = "while";
    token = getNextToken(test, &position);
    cr_assert_eq(token->type, WHILE);
    cr_assert_str_eq(token->value, "while");
    freeToken(token);
}

Test(single_quote_case_1, should_return_expected_token)
{
    size_t position = 0;
    char *test = "'hello'";
    token = getNextToken(test, &position);
    cr_assert_eq(token->type, WORD);
    cr_assert_str_eq(token->value, "'hello'");
    freeToken(token);
}

Test(single_quote_case_2, should_return_expected_token)
{
    size_t position = 0;
    char *test = "'hello' 'world' until";
    token = getNextToken(test, &position);
    cr_assert_eq(token->type, WORD);
    cr_assert_str_eq(token->value, "'hello'");
    freeToken(token);

    token = getNextToken(test, &position);
    cr_assert_eq(token->type, WORD);
    cr_assert_str_eq(token->value, "'world'");
    freeToken(token);

    token = getNextToken(test, &position);
    cr_assert_eq(token->type, UNTIL);
    cr_assert_str_eq(token->value, "until");
    freeToken(token);
}

Test(double_quote_simple_case, should_return_expected_token)
{
    size_t position = 0;
    char *test = "\"hello\"";
    token = getNextToken(test, &position);
    cr_assert_eq(token->type, DOUBLE_QUOTE_WORD);
    cr_assert_str_eq(token->value, "\"hello\"");
    freeToken(token);
}

Test(double_quote_tricky_case, should_return_expected_token)
{
    size_t position = 0;
    char *test = "\"hello \\\" world\"";
    token = getNextToken(test, &position);
    cr_assert_eq(token->type, DOUBLE_QUOTE_WORD);
    cr_assert_str_eq(token->value, "\"hello \" world\"");
    freeToken(token);

}

Test(and_easy_case, should_return_expected_token)
{
    size_t position = 0;
    char *test = "&&";
    token = getNextToken(test, &position);
    cr_assert_eq(token->type, AND);
    cr_assert_str_eq(token->value, "&&");
    freeToken(token);
}

Test(or_easy_case, should_return_expected_token)
{
    size_t position = 0;
    char *test = "||";
    token = getNextToken(test, &position);
    cr_assert_eq(token->type, OR);
    cr_assert_str_eq(token->value, "||");
    freeToken(token);
}

Test(single_quote_after_word, should_return_expected_token)
{
    size_t position = 0;
    char *test = "hello'world'";
    token = getNextToken(test, &position);
    cr_assert_eq(token->type, WORD);
    cr_assert_str_eq(token->value, "hello'world'");
    freeToken(token);
}

Test(single_quote_before_word, should_return_expected_token)
{
    size_t position = 0;
    char *test = "'test'world";
    token = getNextToken(test, &position);
    cr_assert_eq(token->type, WORD);
    cr_assert_str_eq(token->value, "'test'world");
    freeToken(token);
}

Test(backslash_newline, should_return_expected_token)
{
    size_t position = 0;
    char *test = "\\n";
    token = getNextToken(test, &position);
    cr_assert_eq(token->type, WORD);
    cr_assert_str_eq(token->value, "\\n");
    freeToken(token);
}

Test(backslash_single_quote, should_return_expected_token)
{
    size_t position = 0;
    char *test = "\\'test're'ici";
    token = getNextToken(test, &position);
    cr_assert_eq(token->type, WORD);
    cr_assert_str_eq(token->value, "\\'test're'ici");
    freeToken(token);
}

Test(backslash_double_quote, should_return_expected_token)
{
    size_t position = 0;
    char *test = "\\\"test\"re\"ici";
    token = getNextToken(test, &position);
    cr_assert_eq(token->type, DOUBLE_QUOTE_WORD);
    cr_assert_str_eq(token->value, "\"test\"re\"ici");
    freeToken(token);
}

Test(backslash_then_single_quote_non_valid, should_return_expected_token)
{
    size_t position = 0;
    char *test = "\\''test're'ici";
    token = getNextToken(test, &position);
    cr_assert_eq(token->type, NULL_TOKEN);
    cr_assert_eq(token->value, NULL);
    freeToken(token);
}

Test(backslash_then_double_quote_non_valid, should_return_expected_token)
{
    size_t position = 0;
    char *test = "\\\"\"test\"re\"ici";
    token = getNextToken(test, &position);
    cr_assert_eq(token->type, NULL_TOKEN);
    cr_assert_eq(token->value, NULL);
    freeToken(token);
}

Test(all_reserved_words, should_return_expected_token)
{
    size_t position = 0;
    char *test = "if then else elif fi while until do done for exit cd export continue break unset";
    token = getNextToken(test, &position);
    cr_assert_eq(token->type, IF);
    cr_assert_str_eq(token->value, "if");
    freeToken(token);

    token = getNextToken(test, &position);
    cr_assert_eq(token->type, THEN);
    cr_assert_str_eq(token->value, "then");
    freeToken(token);

    token = getNextToken(test, &position);
    cr_assert_eq(token->type, ELSE);
    cr_assert_str_eq(token->value, "else");
    freeToken(token);

    token = getNextToken(test, &position);
    cr_assert_eq(token->type, ELIF);
    cr_assert_str_eq(token->value, "elif");
    freeToken(token);

    token = getNextToken(test, &position);
    cr_assert_eq(token->type, FI);
    cr_assert_str_eq(token->value, "fi");
    freeToken(token);

    token = getNextToken(test, &position);
    cr_assert_eq(token->type, WHILE);
    cr_assert_str_eq(token->value, "while");
    freeToken(token);

    token = getNextToken(test, &position);
    cr_assert_eq(token->type, UNTIL);
    cr_assert_str_eq(token->value, "until");
    freeToken(token);

    token = getNextToken(test, &position);
    cr_assert_eq(token->type, DO);
    cr_assert_str_eq(token->value, "do");
    freeToken(token);

    token = getNextToken(test, &position);
    cr_assert_eq(token->type, DONE);
    cr_assert_str_eq(token->value, "done");
    freeToken(token);

    token = getNextToken(test, &position);
    cr_assert_eq(token->type, FOR);
    cr_assert_str_eq(token->value, "for");
    freeToken(token);

    token = getNextToken(test, &position);
    cr_assert_eq(token->type, WORD);
    cr_assert_str_eq(token->value, "exit");
    freeToken(token);

    token = getNextToken(test, &position);
    cr_assert_eq(token->type, WORD);
    cr_assert_str_eq(token->value, "cd");
    freeToken(token);

    token = getNextToken(test, &position);
    cr_assert_eq(token->type, WORD);
    cr_assert_str_eq(token->value, "export");
    freeToken(token);

    token = getNextToken(test, &position);
    cr_assert_eq(token->type, WORD);
    cr_assert_str_eq(token->value, "continue");
    freeToken(token);

    token = getNextToken(test, &position);
    cr_assert_eq(token->type, WORD);
    cr_assert_str_eq(token->value, "break");
    freeToken(token);

    token = getNextToken(test, &position);
    cr_assert_eq(token->type, WORD);
    cr_assert_str_eq(token->value, "unset");
    freeToken(token);
}

Test(dot_simple_case, should_return_expected_token)
{
    size_t position = 0;
    char *test = ".";
    token = getNextToken(test, &position);
    cr_assert_eq(token->type, DOT);
    cr_assert_str_eq(token->value, ".");
    freeToken(token);
}

Test(not_dot_case, should_return_expected_token)
{
    size_t position = 0;
    char *test = ".a";
    token = getNextToken(test, &position);
    cr_assert_eq(token->type, WORD);
    cr_assert_str_eq(token->value, ".a");
    freeToken(token);
}

Test(many_dot_case, should_return_expected_token)
{
    size_t position = 0;
    char *test = "...";
    token = getNextToken(test, &position);
    cr_assert_eq(token->type, WORD);
    cr_assert_str_eq(token->value, "...");
    freeToken(token);
}

Test(io_number_case, should_return_expected_token)
{
    size_t position = 0;
    char *test = "1>";
    token = getNextToken(test, &position);
    cr_assert_eq(token->type, IONUMBER);
    cr_assert_str_eq(token->value, "1");
    freeToken(token);

    token = getNextToken(test, &position);
    cr_assert_eq(token->type, STDOUT_FILE);
    cr_assert_str_eq(token->value, ">");
    freeToken(token);
}

Test(while_do_done_etc, should_return_expected_token_extra)
{
    size_t position = 0;
    char *test = "while true; do echo foo; done";
    token = getNextToken(test, &position);
    cr_assert_eq(token->type, WHILE);
    cr_assert_str_eq(token->value, "while");
    freeToken(token);

    token = getNextToken(test, &position);
    cr_assert_eq(token->type, WORD);
    cr_assert_str_eq(token->value, "true");
    freeToken(token);

    token = getNextToken(test, &position);
    cr_assert_eq(token->type, SEMICOLON);
    cr_assert_str_eq(token->value, ";");
    freeToken(token);

    token = getNextToken(test, &position);
    cr_assert_eq(token->type, DO);
    cr_assert_str_eq(token->value, "do");
    freeToken(token);

    token = getNextToken(test, &position);
    cr_assert_eq(token->type, WORD);
    cr_assert_str_eq(token->value, "echo");
    freeToken(token);

    token = getNextToken(test, &position);
    cr_assert_eq(token->type, WORD);
    cr_assert_str_eq(token->value, "foo");
    freeToken(token);

    token = getNextToken(test, &position);
    cr_assert_eq(token->type, SEMICOLON);
    cr_assert_str_eq(token->value, ";");
    freeToken(token);

    token = getNextToken(test, &position);
    cr_assert_eq(token->type, DONE);
    cr_assert_str_eq(token->value, "done");
    freeToken(token);
}

Test(assignment_word_with_equal, should_return_expected_token)
{
    size_t position = 0;
    char *test = "a=b";
    token = getNextToken(test, &position);
    cr_assert_eq(token->type, ASSIGNMENT_WORD);
    cr_assert_str_eq(token->value, "a=b");
    freeToken(token);
}