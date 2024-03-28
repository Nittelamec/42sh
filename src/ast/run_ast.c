#include "run_ast.h"

#include "../builtins/g_exito.h"
#include "../builtins/g_lasto.h"

/*
 * This file contains all the functions for the execution of the AST
 */

int is_builtin(struct ast_cmd *ast_cmd)
{
    int ret = -1;
    if (!strcmp("echo", *ast_cmd->words))
        ret = echo(ast_cmd->words + 1);
    if (!strcmp("true", *ast_cmd->words))
        ret = built_true();
    if (!strcmp("false", *ast_cmd->words))
        ret = built_false();
    if (!strcmp("exit", *ast_cmd->words))
        ret = built_exit(ast_cmd->words + 1);
    if (!strcmp("unset", *ast_cmd->words))
        ret = built_unset(ast_cmd->words);
    fflush(stdout);
    mod_g_lasto(ret);
    return ret;
}

int run_cmd(struct ast *ast)
{
    assert(ast->type == AST_CMD);
    struct ast_cmd *ast_cmd = (struct ast_cmd *)ast;
    char **argv = ast_cmd->words;
    int argc = 0;
    while (argv[argc] != NULL)
        argc++;
    // struct table *table = init_full_table(0, argc, argv); // utiliser argc et
    // argv
    struct ast_cmd *ast_cpy =
        (struct ast_cmd *)expand_var(ast_cmd->words, table, argc);
    int quote = expand_quote(ast_cpy->words);
    if (quote)
    {
        fprintf(stderr, "Unmatched quote\n");
        // table_free(table, argc);
        exit(2);
    }
    int ret;
    // Builtin scenario -> no fork()
    if ((ret = is_builtin(ast_cpy)) >= 0)
    {
        // table_free(table, argc);
        free_ast(&ast_cpy->base);
        return ret;
    }
    // Non builtin scenario -> execvp + fork()
    pid_t cpid;
    if ((cpid = fork()) < 0)
    {}; // error
    if (!cpid) // child
    {
        if ((ret = execvp(ast_cpy->words[0], ast_cpy->words)) < 0)
        {
            fprintf(stderr, "%s : command not found\n", ast_cpy->words[0]);
            exit(127);
        }
        // table_free(table, argc);
        return 0;
    }
    else // parent
    {
        int wstatus;
        if (waitpid(cpid, &wstatus, 0) < 0)
        {}
        fflush(stdout);
        mod_g_lasto(WEXITSTATUS(wstatus));
        // table_free(table, argc);
        free_ast(&ast_cpy->base);
        return WEXITSTATUS(wstatus);
    }
} // ne pas oublier de mettre à jour "$?" dans la table + free table

int run_list(struct ast *ast)
{
    assert(ast->type == AST_LIST);
    struct ast_list *ast_list = (struct ast_list *)ast;
    if (!ast_list->next)
        return run_ast(ast_list->child);
    run_ast(ast_list->child);
    return run_list(&ast_list->next->base);
}

int run_if(struct ast *ast)
{
    assert(ast->type == AST_IF);
    struct ast_if *ast_if = (struct ast_if *)ast;
    if (!run_ast(ast_if->condition))
        return run_ast(ast_if->then_body);
    else
        return run_ast(ast_if->else_body);
}

enum pipe_side
{
    PIPELEFT,
    PIPERIGHT
};

pid_t run_pipe_fork(struct ast *ast, int fds[2], enum pipe_side side)
{
    pid_t pid = fork();
    if (pid != 0)
        return pid;
    int fd_to_replace = side == PIPELEFT ? STDOUT_FILENO : STDIN_FILENO;
    int fd_pipe_end = side == PIPELEFT ? fds[1] : fds[0];
    // dup2(fd_pipe_end,fd_to_replace);
    if (dup2(fd_pipe_end, fd_to_replace) == -1)
    {
        fprintf(stderr, "fd_to_replace : %d\n fd_pipe_end : %d \n",
                fd_to_replace, fd_pipe_end);
        perror("dup2 failed\n");
        exit(2); //"run_pipe_fork : dup2 failed");
    }

    close(fds[0]);
    close(fds[1]);
    exit(run_ast(ast));
}

int run_pipeline(struct ast *ast)
{
    assert(ast->type == AST_PIPELINE);
    struct ast_pipeline *ast_pipeline = (struct ast_pipeline *)ast;

    // create pipe
    int fds[2];
    if (pipe(fds) == -1)
        errx(2, "run_pipeline: pipe failed");

    pid_t pid_left = run_pipe_fork(ast_pipeline->left, fds, PIPELEFT);
    pid_t pid_right = run_pipe_fork(ast_pipeline->right, fds, PIPERIGHT);

    if (pid_left == -1 || pid_right == -1)
    {
        close(fds[0]);
        close(fds[1]);
        errx(2, "run_pipeline : fork failed");
    }

    // close pipe
    close(fds[0]);
    close(fds[1]);

    // wait both proces
    int wstatus;
    waitpid(pid_left, &wstatus, 0);
    waitpid(pid_right, &wstatus, 0);

    // return exit code from right ast
    return WEXITSTATUS(wstatus);
}
/*
pid_t run_pipe_fork(struct ast *ast, int fds[2], enum pipe_side side)
{
    pid_t pid = fork();
    if (pid != 0)
        return pid;
    int fd_to_replace = side == PIPELEFT ? STDOUT_FILENO : STDIN_FILENO;
    int fd_pipe_end = side == PIPELEFT ? fds[1] : fds[0];
    //dup2(fd_pipe_end,fd_to_replace);
    if (dup2(fd_pipe_end, fd_to_replace) == -1)
    {
        fprintf(stderr,"fd_to_replace : %d\n fd_pipe_end : %d
\n",fd_to_replace,fd_pipe_end); perror("dup2 failed\n");
        exit(2);//"run_pipe_fork : dup2 failed");
    }

    close(fds[0]);
    close(fds[1]);
    return run_ast(ast);
}

int run_pipeline(struct ast *ast)
{
    assert(ast->type == AST_PIPELINE);
    struct ast_pipeline *ast_pipeline = (struct ast_pipeline *)ast;

    //create pipe
    int fds[2];
    if (pipe(fds) == -1)
        errx(2,"run_pipeline: pipe failed");

    pid_t pid_child = run_pipe_fork(ast_pipeline->right,fds,PIPERIGHT);
    if (dup2(fds[1], STDOUT_FILENO) < 0)
    {} //erreur des familles

    //close pipe
    close(fds[0]);
    close(fds[1]);

    run_ast(ast_pipeline->left);

    //wait both proces
    int wstatus;
    waitpid(pid_child, &wstatus, 0);

    //return exit code from right ast
    return WEXITSTATUS(wstatus);
}
*/
int run_neg(struct ast *ast)
{
    assert(ast->type == AST_NEG);
    struct ast_neg *ast_neg = (struct ast_neg *)ast;
    return !run_ast(ast_neg->neg_cmd);
}

int run_and(struct ast *ast)
{
    assert(ast->type == AST_OPERATORS_AND);
    struct ast_operators_and *ast_and = (struct ast_operators_and *)ast;
    int res_left = run_ast(ast_and->left);
    if (res_left != 0)
        return res_left;
    int res_right = run_ast(ast_and->right);
    return res_right;
}

int run_or(struct ast *ast)
{
    assert(ast->type == AST_OPERATORS_OR);
    struct ast_operators_or *ast_or = (struct ast_operators_or *)ast;
    int res_left = run_ast(ast_or->left);
    if (res_left == 0)
        return res_left;
    return run_ast(ast_or->right);
}

int run_while(struct ast *ast)
{
    assert(ast->type == AST_WHILE);
    struct ast_while *ast_while = (struct ast_while *)ast;
    int ret = 0;
    while (!run_ast(ast_while->condition))
        ret = run_ast(ast_while->body);
    return ret;
}
