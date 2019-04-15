#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "parser.h"
#include "errors.h"


token** tks     = NULL;
int     n_tkn   = 0;

node_t* getG0(token** tokens)
{
    tks = tokens;
    return getB();
}

node_t* getG1(token** tokens)
{
    tks = tokens;
    return getME();
}

node_t* getB()
{
    if(*tks[n_tkn]->val.s == '{')
     {
        n_tkn++;

        node_t* startnode = new_snode(OPER, "`");

        tree_insert(startnode, getE(), LEFT);

        node_t* parent = startnode;
        while (*tks[n_tkn]->val.s != '}')
        {
            node_t* child = new_snode(OPER, "`");
            tree_insert(child, getE(), LEFT );

            if (!child->left)
                assert(!"ERROR! BAD SYNTAX! expected '}'");


            tree_insert(parent, child, RIGHT);

            parent = child;
        }

        n_tkn++;
        return startnode;
    }
    else
        assert(!"ERROR! BAD SYNTAX! expected '{'");

    return getE();
}



node_t* getE()
{
    if (tks[n_tkn]->val_type == STR)
    {
        if (!strcmp(tks[n_tkn]->val.s, "if"))
        {
            n_tkn++;
            node_t* expr = new_snode(STR, "if");

            tree_insert(expr, getP(), LEFT);

            tree_insert(expr, getB(), RIGHT);

            return expr;
        }
        else
        if (!strcmp(tks[n_tkn]->val.s, "while"))
        {
            n_tkn++;
            node_t* expr = new_snode(STR, "while");
            tree_insert(expr, getP(), LEFT);

            tree_insert(expr, getB(), RIGHT);

            return expr;
        }
        else
        if (!strcmp(tks[n_tkn]->val.s, "out"))
        {
            n_tkn++;
            node_t* expr = new_snode(STR, "out");
            tree_insert(expr, getN(), LEFT);

            return expr;
        }

    }


    return getA();
}

node_t* getA()
{
    node_t* var = getN();

    if (*tks[n_tkn]->val.s == '=')
    {
        n_tkn++;
        return new_opnode("=", var, getME());
    }

    return var;

}

node_t* getME()
{
    node_t* lnode = getT();

    node_t* node_op = NULL;

    if(*tks[n_tkn]->val.s == '+')
    {
        n_tkn++;
        node_op = new_snode(OPER, "+");
    }
    else if(*tks[n_tkn]->val.s == '-')
    {
        n_tkn++;
        node_op = new_snode(OPER, "-");
    }


    if (!node_op) return lnode;

    tree_insert(node_op, lnode, LEFT);

    tree_insert(node_op, getME(), RIGHT);

    return node_op;
}


node_t* getT()
{
    node_t* lnode = getD();

    node_t* node_op = NULL;

    if(tks[n_tkn]->val.s[0] == '*')
    {
        n_tkn++;
        node_op = new_snode(OPER, "*");
    }
    else if(tks[n_tkn]->val.s[0] == '/')
    {
        n_tkn++;
        node_op = new_snode(OPER, "/");
    }

    if (!node_op) return lnode;

    tree_insert(node_op, lnode, LEFT);

    tree_insert(node_op, getT(), RIGHT);


    return node_op;
}

node_t* getD()
{
    node_t* func_node = getP();

    if (tks[n_tkn]->val_type == OPER &&
        tks[n_tkn]->val.s[0] == '\'')
    {
        n_tkn++;

        return new_opnode("\'", func_node, getN());
    }
    else return func_node;

}

node_t* getP()
{
    if (tks[n_tkn]->val_type == OPER)
    {
        if (tks[n_tkn]->val.s[0] == '(')
        {
            n_tkn++;
            node_t* node = getME();

            if (*tks[n_tkn]->val.s != ')')
                assert(!"ERROR! BAD SYNTAX! expected ')'");

            n_tkn++;
            return node;
        }
        else
            assert(!"ERROR! BAD SYNTAX! expected '('");
    }
    else return getF();

}

node_t* getF()
{
    if (tks[n_tkn]->val_type == STR)
    {


        char* mfunc = (char*) calloc(sizeof(char), MAX_STR);
        if (!mfunc) return NULL;

        strcpy(mfunc, tks[n_tkn]->val.s);

        node_t* node_mfunc = new_snode(STR, mfunc);

        n_tkn++;

        tree_insert(node_mfunc, getP(), LEFT);

        return node_mfunc;

    }

    return getN();

}


node_t* getN()
{
    switch (tks[n_tkn]->val_type)
    {
    case NUMB:
        {
            int num = tks[n_tkn]->val.d;
            n_tkn++;
            return new_dnode(num);
        }
        break;

    case VAR:
        {
            char* var = (char*) calloc(sizeof(char), 2);
            var[0] = tks[n_tkn]->val.s[0];
            n_tkn++;
            return new_snode(VAR, var);
        }
        break;

    case OPER:
        assert(!"ERROR! BAD SYNTAX! expected '('");

    case NO_TYPE:
        return NULL;

    default:
        assert(!"ERROR! Tokens array is wrong!");
    }

}

