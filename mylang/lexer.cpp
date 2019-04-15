#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "tree.h"
#include "errors.h"
#include "lexer.h"

token** lexer(char* str)
{
    ISNOTNULL(str);

    token** tokens = (token**) calloc(sizeof(*tokens), MAX_TOKEN);
    ISNOTNULL(tokens);

    token** tks = tokens;

    while (*str)
    {
        SKIPSPACES;

        if (isalpha(*str))
        {
            char* comm = (char*) calloc(sizeof(char), MAX_STR);
            ISNOTNULL(comm);

            char* ptr = comm;

            while (isalpha(*str))
            {
                *ptr = *str;
                str++;
                ptr++;
            }

            if ((ptr - comm) == 1)
                *tks = new_stoken(VAR, comm);
            else
                *tks = new_stoken(STR, comm);
            tks++;

        }
        else if (isdigit(*str))
        {
            int num = 0;

            sscanf(str, "%d", &num);

            *tks = new_dtoken(NUMB, num);
            tks++;

            while (*str && isdigit(*str)) str++;
            if (*str == '.') str++;
            while (*str && isdigit(*str)) str++;
        }
        else switch (*str)
        {
            case '+':
            case '-':
            case '/':
            case '*':
            case '(':
            case ')':
            case '=':
            case '{':
            case '}':
            case'\'':
                {
                    char* oper = (char*) calloc(1, 2);
                    oper[0] = *str;

                    *tks = new_stoken(OPER, oper);

                    str++;
                    tks++;
                }
                break;
            default:
                break;
        }

        SKIPSPACES
    }

    *tks = new_stoken(NO_TYPE, "");

    return tokens;

}

token* new_stoken(int type, char* str)
{
    ISNOTNULL(str);

    token* tkn = (token*) calloc(sizeof(*tkn), 1);
    ISNOTNULL(tkn);

    tkn->val.s = str;
    tkn->val_type = type;

    return tkn;
}


token* new_dtoken(int type, int num)
{
    token* tkn = (token*) calloc(sizeof(*tkn), 1);
    ISNOTNULL(tkn);

    tkn->val.d = num;
    tkn->val_type = type;

    return tkn;
}

void token_dump(token** tks)
{

    int i = 0;
    printf("=========TOKENSDUMP===========\n");

    while (tks[i]->val_type != NO_TYPE)
    {
        if (tks[i]->val_type == NUMB) printf("%d\n", tks[i]->val.d);
        else printf("%s\n", tks[i]->val.s);
        i++;
    }
}
