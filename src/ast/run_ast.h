#ifndef RUN_AST_H
#define RUN_AST_H

#include <assert.h>
#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../builtins/builtins.h"
#include "../expansion/expansion.h"
#include "../expansion/init_table.h"
#include "../expansion/variable.h"
#include "ast.h"
#include "run_ast_2.h"

int run_cmd(struct ast *ast);
int run_list(struct ast *ast);
int run_if(struct ast *ast);
int run_pipeline(struct ast *ast);
int run_neg(struct ast *ast);
int run_and(struct ast *ast);
int run_or(struct ast *ast);
int run_for(struct ast *ast);
int run_while(struct ast *ast);
int run_until(struct ast *ast);
int run_redir(struct ast *ast);

#endif /* !RUN_AST_H */
