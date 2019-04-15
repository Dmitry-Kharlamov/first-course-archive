#include <stdlib.h>
#include <assert.h>
#include "windows.h"
#include "mem_alloc.h"

translated_t* mem_allocation(char* source)
{
    assert(source);

    translated_t* tr = (translated_t*)calloc(sizeof(*tr), 1);

    tr->code        = code_alloc();
    tr->data        = data_alloc();
    tr->addrconv    = addrconv_alloc();

    tr->source      = source;
    tr->counter     = 0;

    return tr;
}


char* code_alloc()
{
    char* code = (char*) VirtualAlloc(NULL, MAX_CODE_SEG,
                      MEM_COMMIT, PAGE_EXECUTE_READWRITE);

    assert(code);

    return code;
}

char* data_alloc()
{

    char* data = (char*) calloc(MAX_DATA_SEG,
                                sizeof(*data));

    assert(data);

    return data;
}

int* addrconv_alloc()
{
    int* addr = (int*) calloc(MAX_CODE_SEG,
                                sizeof(*addr));

    assert(addr);

    return addr;
}
