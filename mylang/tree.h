#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

#define TTYPECMP(type) sizeof(type) == sizeof(tvalue_t)

#define PRIORCHANG(node)                    \
            if (node->val_type == OPER &&   \
                priorcmp(_this->val.s[0],   \
                          node->val.s[0]))

typedef int tvalue_t;

enum NODE_TYPE
{   NO_TYPE = 0,
    OPER    = 1,
    NUMB    = 2,
    VAR     = 3,
    STR     = 4,
    CHAR    = 5
};

const int MAX_STR = 512;

const int LEFT  = 0;
const int RIGHT = 1;

union  info
{
    int d;
    char   c;
    char*  s;
};

struct node_t
{
    info          val;
    int      val_type;
    node_t*      left;
    node_t*     right;
};

node_t* new_dnode(int value);

node_t* new_snode(int type, char* value);

node_t* new_cnode(int type, char value);

node_t* new_opnode(char* oper, node_t* left, node_t* right);

node_t* tree_copy(node_t* _this);

int tree_ok(node_t* tree);

node_t* tree_insert(node_t* parent, node_t* child, int pos);

int node_dtor(node_t* _this);

int tree_dtor(node_t* _this);

int tree_print(node_t* _this);


#endif // TREE_H_INCLUDED
