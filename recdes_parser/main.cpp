#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "tree.h"
#include "main.h"

int tree_to_asm(node_t* tree, char* fname)
{
    if (!tree_ok(tree)) return 0;

    FILE* out = fopen(fname, "w");

    node_to_asm(tree, out);

    fprintf(out, "out\nend\n");

    return 0;
}

int node_to_asm(node_t* node, FILE* out)
{
    if (!tree_ok(node)) return 0;

    if (node->val_type == OPERATOR)
    {
        node_to_asm(node->left,  out);
        node_to_asm(node->right, out);

        switch (node->value.sv[0])
        {
        case '+':
            fprintf(out, "add\n");
            break;
        case '-':
            fprintf(out, "sub\n");
            break;
        case '*':
            fprintf(out, "mul\n");
            break;
        case '/':
            fprintf(out, "div\n");
            break;

        }
    }
    else
        fprintf(out, "push %lg\n", node->value.dv);


    return 0;
}

int main()
{

    char* str = "10*4+10*8-(11+9)*10";

    node_t* tree = getG0(str);

    tree_to_asm(tree, "calc.txt");

    tree_print(tree);

    return 0;
}

