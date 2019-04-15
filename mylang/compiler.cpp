#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "tree.h"
#include "lexer.h"
#include "compiler.h"
#include "errors.h"

#include "differentiation.h"

int num_if = 0;

int tree_to_asm(node_t* tree, char* fname)
{
    if (!tree_ok(tree)) return 0;
    if (!fname) return 0;

    FILE* out = fopen(fname, "w");

    node_to_asm(tree, out);

    fprintf(out, "end\n");

    return 0;
}

int node_to_asm(node_t* node, FILE* out)
{
    if (!tree_ok(node)) return 0;

    switch (node->val_type)
    {
    case OPER:
    {
        if(*node->val.s == '`')
        {
            node_to_asm(node->left , out);
            node_to_asm(node->right, out);
            return 0;
        }

        if(*node->val.s == '=')
        {
            node_to_asm(node->right, out);
            fprintf(out, "pop dx\n");

            fprintf(out, "mov %s, dx\n", node->left->val.s);
            return 0;
        }

        node_to_asm(node->left,  out);
        node_to_asm(node->right, out);

        switch (*node->val.s)
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

    break;

    case NUMB:
        fprintf(out, "push %d\n", node->val.d);

    break;
    case VAR:
        fprintf(out, "push %s\n", node->val.s);

    break;

    case STR:
    {
        if (!strcmp("if", node->val.s))
        {

            node_to_asm(node->left, out);
            fprintf(out, "je if%d\n", num_if);

            node_to_asm(node->right, out);

            fprintf(out, "if%d:\n", num_if);
            num_if++;
        }
        else
        if (!strcmp("while", node->val.s))
        {
            fprintf(out, "if%d:\n", num_if);

            node_to_asm(node->left, out);
            fprintf(out, "push 0\nje if%d\n", num_if+1);

            node_to_asm(node->right, out);

            fprintf(out, "jmp if%d\n", num_if);
            fprintf(out, "if%d:\n", num_if+1);
            num_if += 2;
        }
        else
        if (!strcmp("cos", node->val.s))
        {
            node_to_asm(node->left, out);
            fprintf(out, "cos\n");
        }
        else
        if (!strcmp("sin", node->val.s))
        {
            node_to_asm(node->left, out);
            fprintf(out, "sin\n");
        }
        else
        if (!strcmp("out", node->val.s))
        {
            fprintf(out, "push %s\n", node->left->val.s);
            fprintf(out, "out\npop dx\n");
        }

    }
    break;
    }
    return 0;
}

