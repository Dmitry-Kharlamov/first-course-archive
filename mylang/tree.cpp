#include"tree.h"
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "errors.h"


int priorcmp(char first, char second)
{
    int prior1 = 0, prior2 = 0;

    if (first == '+' || first == '-') prior1 = 0;
    else prior1 = 1;

    if (second == '+' || second == '-') prior2 = 0;
    else prior2 = 1;

    return (prior1 > prior2);
}

node_t* new_dnode(int value)
{
    node_t* node = (node_t*) calloc(1, sizeof(*node));

    node->val.d = value;
    node->val_type = NUMB;

    node->left  = NULL;
    node->right = NULL;

    return node;
}

node_t* new_cnode(char value)
{
    node_t* node = (node_t*) calloc(1, sizeof(*node));

    node->val.c    = value;
    node->val_type = CHAR;

    node->left  = NULL;
    node->right = NULL;

    return node;
}

node_t* new_snode(int type, char* value)
{
    ISNOTNULL(value);

    node_t* node = (node_t*) calloc(1, sizeof(*node));

    node->val.s    = value;
    node->val_type = type;

    node->left  = NULL;
    node->right = NULL;

    return node;
}

node_t* new_opnode(char* oper, node_t* left, node_t* right)
{
    ISNOTNULL(oper);

    node_t* op_node = new_snode(OPER, oper);

    tree_insert(op_node, left, LEFT);

    tree_insert(op_node, right, RIGHT);

    return op_node;
}

node_t* tree_insert(node_t* parent, node_t* child, int pos)
{
    ISNOTNULL(parent);

    node_t* old_child = NULL;
    if (pos)
    {
        old_child  = parent->right;
        parent->right = child;
    }
    else
    {
        old_child = parent->left;
        parent->left = child;
    }

    return old_child;
}


node_t* tree_copy(node_t* _this)
{
    ISNOTNULL(_this);

    switch (_this->val_type)
    {
    case OPER:
        return new_opnode(_this->val.s,
                          tree_copy(_this->left),
                          tree_copy(_this->right));

    case NUMB:
        return new_dnode(_this->val.d);

    case STR:
        {
            node_t* str = new_snode(STR, _this->val.s);

            if (!strcmp( _this->val.s, "sin") ||
                !strcmp( _this->val.s, "cos"))
                tree_insert(str, tree_copy(_this->left), LEFT);

            return str;
        }

    case VAR:
        return new_snode(VAR, _this->val.s);

    }
    return NULL;

}

int tree_print(node_t* _this)
{
    if (!_this) return 0;
    if (!tree_ok(_this)) return 0;

    switch (_this->val_type)
    {
    case OPER:
        {
            if (*_this->val.s == '`')
            {
                tree_print(_this->left);
                printf("\n");
                tree_print(_this->right);
                return 0;
            }
            node_t* nleft = _this->left;

            PRIORCHANG(nleft) printf("(");
            tree_print(nleft);
            PRIORCHANG(nleft) printf(")");

            printf(" %s ", _this->val.s);

            node_t* nright = _this->right;

            PRIORCHANG(nright) printf("(");
            tree_print(nright);
            PRIORCHANG(nright) printf(")");
        }
        break;
    case NUMB:
        printf("%d", _this->val.d);
        break;
    case VAR:
        printf("%s", _this->val.s);
        break;
    case STR:
        {
            printf("%s", _this->val.s);
            if (!strcmp( _this->val.s, "sin") ||
                !strcmp( _this->val.s, "cos") ||
                !strcmp( _this->val.s, "if")  ||
                !strcmp( _this->val.s, "while"))
            {

                printf("(");
                tree_print(_this->left);
                printf(")");
            }
            if (!strcmp(_this->val.s, "out"))
            {
                printf(" ");
                tree_print(_this->left);
            }

        }
        break;


    default:
        break;

    }


    return 0;
}

/*node_t* tree_read(char* tree_str)
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
        if (TTYPECMP(int))
            node = new_dnode(atof(value));

        if (TTYPECMP(char*))
            node = new_snode(value);

        tree_insert(node, tree_read(tree_str), 0);
        tree_insert(node, tree_read(tree_str), 1);

        return node;
    }



    return 0;
}
*/
int tree_ok(node_t* tree)
{
    if (!tree)
        return 0;

    return 1;
}

int tree_dtor(node_t* _this)
{
    if (_this)
        return 0;

    tree_dtor(_this->left );
    tree_dtor(_this->right);

    node_dtor(_this);

    return 0;
}

int node_dtor(node_t* _this)
{
    if (_this) return 0;

    if (_this->val_type == OPER || _this->val_type == VAR)
        free(_this->val.s);

    free(_this);

    return 0;
}
