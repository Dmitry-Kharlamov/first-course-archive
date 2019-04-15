#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include "tree.h"
#include "lexer.h"

node_t* getG0(token** tokens);

node_t* getG1(token** tokens);

node_t* getB();  //BLOCK

node_t* getE();  //EXPRESSION

node_t* getA();  //ASSIGNMENT

node_t* getME(); //MATH EXPRESSION

node_t* getT();

node_t* getD();

node_t* getP();

node_t* getF();

node_t* getN();



#endif // PARSER_H_INCLUDED
