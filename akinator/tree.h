#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

struct node_t
{
    char *value;
    node_t*  left;
    node_t* right;
};

node_t* new_node(char* value);

node_t* tree_insert(node_t* parent, node_t* children, int pos);

int node_dtor(node_t* _this);

int tree_print(node_t* _this);


#endif // TREE_H_INCLUDED
