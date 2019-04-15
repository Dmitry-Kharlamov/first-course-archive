#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

#define TTYPECMP(type) sizeof(type) == sizeof(tvalue_t)

#define PRIORCHANG(node)                        \
            if (node->val_type == OPERATOR &&   \
                priorcmp(_this->value.sv[0],    \
                          node->value.sv[0]))

typedef double tvalue_t;

enum NODE_TYPE
{
    OPERATOR = 1,
    NUMBER   = 2,
    VARIABLE = 3
};


union  info
{
    double dv;
    char*  sv;
};

struct node_t
{
    info        value;
    int      val_type;
    node_t*      left;
    node_t*     right;
};

node_t* new_dnode(double value);

node_t* new_snode(char* value);

int tree_ok(node_t* tree);

node_t* tree_insert(node_t* parent, node_t* children, int pos);

int node_dtor(node_t* _this);

int tree_print(node_t* _this);


#endif // TREE_H_INCLUDED
