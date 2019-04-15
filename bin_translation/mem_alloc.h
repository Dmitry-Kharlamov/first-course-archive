#ifndef MEM_ALLOC_H_INCLUDED
#define MEM_ALLOC_H_INCLUDED

#include "resources.h"

translated_t* mem_allocation(char* source);

char* code_alloc();

char* data_alloc();

int* addrconv_alloc();

#endif // MEM_ALLOC_H_INCLUDED
