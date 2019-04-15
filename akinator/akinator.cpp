#include"tree.h"


int main()
{
    node_t* tree = new_node("BEK?");
    tree_insert(tree, new_node("BEK"), 1);
    tree_insert(tree, new_node("IVANOVA"), 0);

    tree_print(tree);
    return 0;
}


