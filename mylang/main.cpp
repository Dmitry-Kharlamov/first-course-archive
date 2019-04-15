#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "tree.h"
#include "lexer.h"
#include "main.h"
#include "compiler.h"
#include "differentiation.h"
#include "errors.h"
#include "output_TeX.h"

enum MODE
{
    LANG     = 10,
    DIFtoTeX = 20
};

int main()
{

    char* str = fileread("prog.txt");

    run(str, LANG);

    return 0;
}


int run(char* str, int mode)
{
    switch(mode)
    {
    case LANG:
    {
        token** tks = lexer(str);

        node_t* tree = getG0(tks);

        tree = differ(tree);

        tree = optim(tree);

        tree_to_asm(tree, "program.txt");

    }
    break;
    case DIFtoTeX:
    {
        token** tks = lexer(str);

        node_t* tree = getG1(tks);

        output_TeX(tree, NULL);

    }
    break;

    }


    return 0;
}

char* fileread(char* filename)
{
    char* str = (char*) calloc(sizeof(*str), MAX_PROG);

    FILE* in = fopen(filename, "r");

    fread(str, 1, MAX_PROG, in);

    return str;
}
