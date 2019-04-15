#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tree.h"
#include "differentiation.h"
#include "errors.h"


node_t* differ(node_t* node)
{
    ISNOTNULL(node);

    node->left  = differ(node->left);
    node->right = differ(node->right);

    if (node->val_type == OPER && node->val.s[0] == '\'')
    {
        char var = node->right->val.s[0];
        return derivative(node->left, var);

    }
    else
        return node;

}

node_t* derivative(node_t* node, char var)
{
    ISNOTNULL(node);

    switch (node->val_type)
    {
    case STR:

        if (!strcmp(node->val.s, "sin"))
            return deriv_sin(node, var);
        else
        if (!strcmp(node->val.s, "cos"))
            return deriv_cos(node, var);

        break;

    case OPER:
        switch (node->val.s[0])
        {
        case'+':
            return deriv_add(node, var);
            break;

        case'-':
            return deriv_sub(node, var);
            break;

        case'*':
            return deriv_mul(node, var);
            break;

        case'/':
            return deriv_div(node, var);
            break;

        }


        break;
    case NUMB:
        return new_dnode(0);

        break;
    case VAR:
        if (node->val.s[0] == var) return new_dnode(1);
        return new_dnode(0);

        break;

    }

}


node_t* optim(node_t* tree)
{
    ISNOTNULL(tree);

    int isChange = 0;
    while (true)
    {
        isChange = 0;
        tree = optim1(tree, &isChange);
        tree = optim2(tree, &isChange);
        if (!isChange) break;

    }

    return tree;
}

node_t* optim1(node_t* tree, int *isChange)
{
    ISNOTNULL(tree);
    if (tree->val_type == OPER &&
        tree->left-> val_type == NUMB &&
        tree->right->val_type == NUMB)
    {
            *isChange = 1;
            switch (tree->val.s[0])
            {
            case '+':
                return new_dnode(tree->left->val.d +
                                  tree->right->val.d );
                break;
            case '*':
                return new_dnode(tree->left->val.d *
                                 tree->right->val.d );
                break;
            case '-':
                return new_dnode(tree->left->val.d -
                                 tree->right->val.d );
                break;
            case '/':
                return new_dnode(tree->left->val.d /
                                 tree->right->val.d );
                break;
            }
    }

    else
    {
        tree->left  = optim1(tree->left,  isChange);
        tree->right = optim1(tree->right, isChange);
    }

    return tree;

}

node_t* optim2(node_t* tree, int *isChange)
{
    ISNOTNULL(tree);
    if (tree->val_type == OPER)
    {
        switch (tree->val.s[0])
        {
        case '+':
            {
                if (tree->left->val_type == NUMB &&
                    tree->left->val.d == 0)
                {
                    tree_dtor(tree->left);
                    node_dtor(tree);
                    *isChange = 1;
                    return tree->right;
                }


                if (tree->right->val_type == NUMB &&
                    tree->right->val.d == 0)
                {
                    tree_dtor(tree->right);
                    node_dtor(tree);
                    *isChange = 1;
                    return tree->left;
                }
            }
            break;
        case '*':
            {

                if (tree->left->val_type == NUMB &&
                    tree->left->val.d == 0)
                {
                    tree_dtor(tree);
                    *isChange = 1;
                    return new_dnode(0);
                }


                if (tree->right->val_type == NUMB &&
                    tree->right->val.d == 0)
                {
                    tree_dtor(tree);
                    *isChange = 1;
                    return new_dnode(0);
                }

                if (tree->left->val_type == NUMB &&
                    tree->left->val.d == 1)
                {
                    tree_dtor(tree->left);
                    node_dtor(tree);
                    *isChange = 1;
                    return tree->right;
                }

                if (tree->right->val_type == NUMB &&
                    tree->right->val.d == 1)
                {
                    tree_dtor(tree->right);
                    node_dtor(tree);
                    *isChange = 1;
                    return tree->left;
                }
            }
            break;
        case '-':
            {
                if (tree->right->val_type == NUMB &&
                    tree->right->val.d == 0)
                {
                    tree_dtor(tree->right);
                    node_dtor(tree);
                    *isChange = 1;
                    return tree->left;
                }
            }
            break;
        case '/':
            {
                if (tree->left->val_type == NUMB &&
                    tree->left->val.d == 0)
                {
                    tree_dtor(tree);
                    *isChange = 1;
                    return new_dnode(0);
                }

                if (tree->right->val_type == NUMB &&
                    tree->right->val.d == 1)
                {
                    tree_dtor(tree->right);
                    node_dtor(tree);
                    *isChange = 1;
                    return tree->left;
                }

            }
            break;
        }

        tree->left  = optim2(tree->left,  isChange);
        tree->right = optim2(tree->right, isChange);

    }


    return tree;

}


#define PLUS(left, right) \
    new_opnode("+", left, right)

#define MINUS(left, right) \
    new_opnode("-", left, right)

#define MUL(left, right) \
    new_opnode("*", left, right)

#define DIV(left, right) \
    new_opnode("*", left, right)

#define D(node) derivative(node, var)

#define C(node) tree_copy(node)

#define L node->left
#define R node->right

node_t* deriv_add(node_t* node, char var)
{
    ISNOTNULL(node);
    return PLUS(D(L), D(R));
}

node_t* deriv_sub(node_t* node, char var)
{
    ISNOTNULL(node);
    return MINUS(D(L), D(R));
}

node_t* deriv_mul(node_t* node, char var)
{
    ISNOTNULL(node);
    return PLUS(MUL(D(L), C(R)),
                MUL(C(L), D(R)));
}

node_t* deriv_div(node_t* node, char var)
{
    ISNOTNULL(node);
    node_t* numerator   = MINUS(MUL(D(L), C(R)),
                                MUL(C(L), D(R)));
    node_t* denominator = MUL(C(R), C(R));

    return DIV(numerator, denominator);

}

node_t* deriv_sin(node_t* node, char var)
{
    ISNOTNULL(node);
    node_t* func = C(node);
    func->val.s = "cos";

    return MUL(func, D(L));

}

node_t* deriv_cos(node_t* node, char var)
{
    ISNOTNULL(node);
    node_t* func = C(node);
    func->val.s = "sin";

    return MUL(new_dnode(-1), MUL(func, D(L)));

}
