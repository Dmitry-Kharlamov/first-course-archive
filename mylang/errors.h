#ifndef ERRORS_H_INCLUDED
#define ERRORS_H_INCLUDED

#define ISNOTNULL(ptr)          \
    if (!ptr)                   \
        return NULL;            \


enum ERRORNUMS
{
    ENPTR   = 1,
    ECALLOC = 2

};

#endif // ERRORS_H_INCLUDED
