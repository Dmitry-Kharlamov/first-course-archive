#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "parser.h"

char* curr_symb = NULL;

node_t* getG0(char* str)
{

    curr_symb = str;

    return getE();
}


node_t* getE()
{
    node_t* lnode = getT();

    node_t* node_op = NULL;


    if(*curr_symb == '+') {curr_symb++; node_op = new_snode("+");}
    if(*curr_symb == '-') {curr_symb++; node_op = new_snode("-");}

    if (!node_op) return lnode;

    tree_insert(node_op, lnode, LEFT);

    tree_insert(node_op, getE(), RIGHT);

    return node_op;
}


node_t* getT()
{
    node_t* lnode = getP();

    node_t* node_op = NULL;


    if(*curr_symb == '*') {curr_symb++; node_op = new_snode("*");}
    if(*curr_symb == '/') {curr_symb++; node_op = new_snode("/");}

    if (!node_op) return lnode;

    tree_insert(node_op, lnode, LEFT);

    tree_insert(node_op, getT(), RIGHT);


    return node_op;
}


node_t* getP()
{
    if(*curr_symb == '(')
    {
        curr_symb++;
        node_t* node = getE();

        if (*curr_symb != ')') return NULL;

        curr_symb++;
        return node;
    }
    else return getN();

}

node_t* getN()
{
    double value = 0;
    while ('0' <= *curr_symb && *curr_symb <= '9')
    {
        value = value * 10 + *curr_symb - '0';
        curr_symb++;
    }

    return new_dnode(value);
}


