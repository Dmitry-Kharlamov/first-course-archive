#ifndef HASH_H_INCLUDED
#define HASH_H_INCLUDED

#include "list.h"


typedef int (*hash_func_t)(char*);

struct hash_t
{
    int sizeh;
    list_t** table;
    hash_func_t func;
};




int hash_func1(char* str);

int hash_func2(char* str);

int hash_func3(char* str);

int hash_func4(char* str);

int hash_func5(char* str);


hash_t* hash_ctor(int sizeh, hash_func_t func);

int hash_table_add(hash_t* _this, char* value);

int hash_table_find(hash_t* _this, char* value);

int hash_dump(hash_t* _this, char* name);

int hash_ok(hash_t* _this);

hash_t* hash_dtor(hash_t* _this);


#endif // HASH_H_INCLUDED
