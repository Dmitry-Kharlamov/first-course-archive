#include "hash.h"
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include<assert.h>


#define KU printf("I'm here!\n");
#define OK assert(!hash_ok(_this));

const char* DUMP_FILE_EXTENSION = ".cvs";
const int MAX_LEN_WORD = 100;

//!======================================
//! HASH NUMBER FUNCTIONS
//!======================================
int hash_func1(char* str)
{
    assert(str);
    return 1;
}

int hash_func2(char* str)
{
    assert(str);
    return *str;
}

int hash_func3(char* str)
{
    assert(str);
    return strlen(str);
}

int hash_func4(char* str)
{
    assert(str);

    int sum = 0;
    for(; *str; str++) sum += *str;
    return sum;
}

int hash_func5(char* str)
{
    assert(str);

    if (!*str)  return 0;

    int hash_num = *str;
    str++;

    for(; *str; str++)
        hash_num = (hash_num << 1) ^ *str;

    return hash_num;
}


//!======================================
//! HASH TABLE FUNCTIONS
//!======================================
hash_t* hash_ctor(int sizeh, hash_func_t func)
{
    assert(sizeh > 0);
    assert(func);

    hash_t* hash_table = (hash_t*) calloc(1, sizeof(hash_t));
    assert(hash_table);

    hash_table->sizeh = sizeh;

    hash_table->table = (list_t**) calloc(sizeh, sizeof(list_t**));

    for (int i = 0; i < sizeh; i++)
        hash_table->table[i] = list_ctor();

    hash_table->func = func;

    return hash_table;
}

int hash_table_add(hash_t* _this, char* value)
{
    OK
    assert(value);

    if (hash_table_find(_this, value)) return 0;

    int hash_num = _this->func(value);
    hash_num %= _this->sizeh;

    list_t* hash_list = _this->table[hash_num];

    list_add(hash_list, hash_list->tail, value);

    return 0;
}

int hash_table_find(hash_t* _this, char* value)
{
    OK
    assert(value);

    int hash_num = _this->func(value);
    hash_num %= _this->sizeh;

    return list_find( _this->table[hash_num], value);

}

int hash_dump(hash_t* _this, char* name)
{
    assert(_this);
    assert(name);

    char dump_name[40] = "";
    int len = strlen(name);

    memcpy(dump_name, name, len);

    memcpy((dump_name+len), DUMP_FILE_EXTENSION, strlen(DUMP_FILE_EXTENSION));

    FILE* out = fopen(dump_name, "wb");
    assert(out);

    for (int i = 0; i < _this->sizeh; i++)
    {
        fprintf(out, "%d, %d\n", i, _this->table[i]->nElem);
    }

    return 0;
}

int hash_ok(hash_t* _this)
{
    if (!_this) return ENULLPTR;

    if (_this->sizeh < 0) return EBADSIZE;

    if (!_this->table) return ENOTABLE;

    if (!_this->func) return ENOFUNC;

    for (int i = 0; i < _this->sizeh; i++)
            if(list_ok(_this->table[i])) return EBADLIST;


    return 0;
}


hash_t* hash_dtor(hash_t* _this)
{
    assert(_this);

    _this->func = NULL;

    for (int i = 0; i < _this->sizeh; i++)
        list_dtor(_this->table[i]);

    free(_this->table);

    _this->sizeh = 0;

    free(_this);
    return NULL;
}


