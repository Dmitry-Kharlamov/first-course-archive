#include"tree.h"
#include<stdlib.h>
#include<stdio.h>

const int MAX_STR = 512;


int priorcmp(char first, char second)
{
    int prior1 = 0, prior2 = 0;

    if (first == '+' || first == '-') prior1 = 0;
    else prior1 = 1;

    if (second == '+' || second == '-') prior2 = 0;
    else prior2 = 1;

    return (prior1 > prior2);
}

node_t* new_dnode(double value)
{
    node_t* node = (node_t*) calloc(1, sizeof(*node));

    node->value.dv = value;
    node->val_type = NUMBER;

    node->left  = NULL;
    node->right = NULL;

    return node;
}

node_t* new_snode(char* value)
{
    node_t* node = (node_t*) calloc(1, sizeof(*node));

    node->value.sv = value;
    node->val_type = OPERATOR;

    node->left  = NULL;
    node->right = NULL;

    return node;
}

node_t* tree_insert(node_t* parent, node_t* children, int pos)
{
    if (!parent) return NULL;

    node_t* old_children = NULL;
    if (pos)
    {
        old_children  = parent->right;
        parent->right = children;
    }
    else
    {
        old_children = parent->left;
        parent->left = children;
    }

    return old_children;
}


int tree_print(node_t* _this)
{
    if (!_this) return 0;
    if (!tree_ok(_this)) return 0;

    if (_this->val_type == OPERATOR)
    {
        node_t* nleft = _this->left;

        PRIORCHANG(nleft) printf("(");
        tree_print(nleft);
        PRIORCHANG(nleft) printf(")");

        printf(" %s ", _this->value.sv);

        node_t* nright = _this->right;

        PRIORCHANG(nright) printf("(");
        tree_print(nright);
        PRIORCHANG(nright) printf(")");
    }
    else
        printf("%lg", _this->value.dv);

    return 0;
}

node_t* tree_read(char* tree_str)
{
    if (*tree_str == '(')
    {
        tree_str++;
        char* value = (char*) calloc(MAX_STR, sizeof(*value));

        char* ptr = value;
        while (*tree_str != ' ')
        {
            *ptr = *tree_str;
            ptr++;
            tree_str++;
        }
        tree_str++;

        node_t* node = NULL;
        if (TTYPECMP(double))
            node = new_dnode(atof(value));

        if (TTYPECMP(char*))
            node = new_snode(value);

        tree_insert(node, tree_read(tree_str), 0);
        tree_insert(node, tree_read(tree_str), 1);

        return node;
    }



    return 0;
}

int tree_ok(node_t* tree)
{
    if (!tree) return 0;
    return 1;
}

int node_dtor(node_t* _this)
{
    if (_this) return 0;

    node_dtor(_this->left );
    node_dtor(_this->right);
    free(_this);

    return 0;
}
