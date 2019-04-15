#include"tree.h"
#include<stdlib.h>
#include<stdio.h>


node_t* new_node(char* value)
{
    node_t* node = (node_t*) calloc(1, sizeof(*node));

    node->value = value;

    node->left  = NULL;
    node->right = NULL;

    return node;
}


node_t* tree_insert(node_t* parent, node_t* children, int pos)
{
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

    printf("(%s ", _this->value);
    tree_print(_this->left );
    tree_print(_this->right);

    printf(") ", _this->value);

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




        return new_node(value);
    }



    return 0;
}


int node_dtor(node_t* _this)
{
    if (_this) return 0;

    free(_this->value);
    _this->value = NULL;

    node_dtor(_this->left );
    node_dtor(_this->right);
    free(_this);

    return 0;
}
