#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include "tree.h"

node_t* getG0(char* str);

node_t* getE();

node_t* getT();

node_t* getP();

node_t* getN();


const int LEFT  = 0;
const int RIGHT = 1;

#endif // PARSER_H_INCLUDED
