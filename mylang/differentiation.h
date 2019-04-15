#ifndef DIFFERENTIATION_H_INCLUDED
#define DIFFERENTIATION_H_INCLUDED

node_t* derivative(node_t* node, char var);

node_t* differ(node_t* node);

node_t* deriv_add(node_t* node, char var);

node_t* deriv_sub(node_t* node, char var);

node_t* deriv_mul(node_t* node, char var);

node_t* deriv_div(node_t* node, char var);

node_t* deriv_sin(node_t* node, char var);

node_t* deriv_cos(node_t* node, char var);

node_t* optim(node_t* tree);

node_t* optim1(node_t* tree, int *isChange);

node_t* optim2(node_t* tree, int *isChange);

#endif // DIFFERENTIATION_H_INCLUDED
