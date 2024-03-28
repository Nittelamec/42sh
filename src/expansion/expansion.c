#define _POSIX_C_SOURCE 200809L

#include "expansion.h"

void rolChar(char **str, size_t index)
{
    size_t len = strlen(*str);
    for (size_t i = index; i < len; i++)
        (*str)[i] = (*str)[i + 1];
}

size_t my_strlen(char *str)
{
    size_t i = 0;
    while (str[i] != '\0')
        i++;
    return i;
}

int in_single_or_double(char *word)
{
    size_t len = my_strlen(word);
    int complete_quote = 1;
    int single_q = 0;
    int double_q = 0;
    for (size_t i = 0; i < len; i++)
    {
        if (word[i] == '\'' && !double_q)
        {
            single_q = !single_q;
            complete_quote = !complete_quote;
            rolChar(&word, i);
            i--;
            len--;
        }
        else if (word[i] == '"' && (i == 0 || word[i - 1] != '\\') && !single_q)
        {
            double_q = !double_q;
            complete_quote = !complete_quote;
            rolChar(&word, i);
            i--;
            len--;
        }
        else if (word[i] == '\\' && !single_q)
        {
            if (i < len - 1 && word[i + 1] != '\'')
            {
                rolChar(&word, i);
                len--;
            }
            else if (i < len - 1 && word[i + 1] == '\'')
            {
                rolChar(&word, i);
                i--;
                len--;
            }
        }
    }
    if (!complete_quote)
        return 1;
    return 0;
}

int expand_quote(char **words)
{
    int quote = 0;
    size_t i = 0;
    while (words[i])
    {
        quote = in_single_or_double(words[i]);
        if (quote == 1)
            return 1;
        i++;
    }
    return 0;
}

/*int main()
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

    for (int i = 0; i < 3; i++)
        printf("word %d: %s\n", i, ast_cmd.words[i]);

    int res = expand_quote((struct ast *)&ast_cmd);

    printf("\nAfter expansion\n\n");
    printf("res: %d\n", res);

    for (int i = 0; i < 3; i++)
        printf("word %d: %s\n", i, ast_cmd.words[i]);

    return 0;
}*/
