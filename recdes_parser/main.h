#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

int tree_to_asm(node_t* tree, char* fname);

int node_to_asm(node_t* node, FILE* out);

#endif // MAIN_H_INCLUDED
