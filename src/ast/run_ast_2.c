#include "run_ast_2.h"

#include "../expansion/init_table.h"

extern struct table *table;

int run_until(struct ast *ast)
{
    assert(ast->type == AST_UNTIL);
    struct ast_until *ast_until = (struct ast_until *)ast;
    int ret = 0;
    while (run_ast(ast_until->condition))
        ret = run_ast(ast_until->body);
    return ret;
}

int redir_fd(struct ast_redir *ast_redir)
{
    if (ast_redir->fd != -1)
    {
        return ast_redir->fd;
    }
    enum TokenType type = ast_redir->redirect_type;
    if (type == FORCE_STDOUT_FILE || type == STDOUT_FILE_ADD
        || type == STDOUT_STDERR_FILE_ADD || type == STDOUT_FILE)
        return STDOUT_FILENO;
    return STDIN_FILENO;
}

int redir_mode(struct ast_redir *ast_redir)
{
    enum TokenType type = ast_redir->redirect_type;
    if (type == STDIN_STDOUT)
        return O_RDWR | O_CREAT | O_TRUNC;
    if (type == STDOUT_FILE_ADD)
        return O_WRONLY | O_CREAT | O_APPEND;
    if (type == FORCE_STDOUT_FILE || type == STDOUT_FILE
        || type == STDOUT_STDERR_FILE_ADD)
        return O_WRONLY | O_CREAT | O_TRUNC;
    if (type == FILE_STDIN || type == FILE_DUP_STDOUT)
        return O_RDONLY;
    return -1;
}

int fd_from_word(struct ast_redir *ast_redir)
{
    if (ast_redir->redirect_type == FILE_DUP_STDOUT
        || ast_redir->redirect_type == STDOUT_STDERR_FILE_ADD)
    {
        if (isdigit(ast_redir->file[0]))
        {
            return atoi(ast_redir->file);
        }
    }
    return -1;
}

int run_redir(struct ast *ast)
{
    assert(ast->type == AST_REDIR);
    struct ast_redir *ast_redir = (struct ast_redir *)ast;
    int mode = redir_mode(ast_redir);
    int fd_to_replace = redir_fd(ast_redir);
    int fd_open = fd_from_word(ast_redir);
    if (fd_open == -1)
        fd_open = open(ast_redir->file, mode, 0644);
    if (fd_open == -1)
    {
        fprintf(stderr, "run_redir: failed to open");
        exit(2);
    }
    int fd_save = dup(fd_to_replace);
    if (fd_save == -1)
    {
        close(fd_open);
        fprintf(stderr, "run_redir: failed to dup");
        exit(2);
    }
    int new_std = dup2(fd_open, fd_to_replace);
    if (new_std == -1)
    {
        close(fd_open);
        close(fd_save);
        fprintf(stderr, "run_redir: failed to dup2");
        exit(2);
    }
    close(fd_open);

    int res = run_ast(ast_redir->child);

    if (dup2(fd_save, new_std) == -1)
    {
        close(new_std);
        fprintf(stderr, "run_redir: failed to restore std");
        exit(2);
    }
    close(fd_save);

    return res;
}

int run_for(struct ast *ast)
{
    assert(ast->type == AST_FOR);
    struct ast_for *ast_for = (struct ast_for *)ast;

    size_t k = 0;
    while (ast_for->words[k])
        k++;
    struct ast_cmd *ast_cpy =
        (struct ast_cmd *)expand_var(ast_for->words, table, k);
    int q = expand_quote(ast_cpy->words);

    if (!q)
    {
        // prblm
    }
    int ret = 0;
    for (size_t i = 0; i < k; i++)
    {
        table = table_insert(table, ast_for->name, &ast_cpy->words[i]);
        ret = run_ast(ast_for->loop_body);
        table = table_pop(table, ast_for->name);
    }
    free_ast(&ast_cpy->base);

    return ret;
}
