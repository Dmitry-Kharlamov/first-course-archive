#include "list.h"
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>


list_t* list_ctor()
{
    list_t* mylist = (list_t*) calloc(sizeof(list_t), 1);

    assert(mylist);

    mylist->head  = NULL;
    mylist->tail  = NULL;
    mylist->nElem = 0;

    return mylist;
}

list_elem_t* list_elem_ctor(char* value)
{
    assert(value);
    list_elem_t* list_elem = (list_elem_t*) calloc(sizeof(list_elem_t), 1);

    assert(list_elem);

    list_elem->value    = value;
    list_elem->next     = NULL;
    list_elem->prev     = NULL;


    return list_elem;
}


int list_add(list_t* _this, list_elem_t* prev_elem, char* value)
{
    assert(!list_ok(_this));

    int len = strlen(value);
    char* str = (char*) calloc(len, sizeof(*str));
    strcpy(str, value);

    list_elem_t* add_elem = list_elem_ctor(str);

    list_elem_t* next_elem = NULL;

    if (!_this->head || !prev_elem) { next_elem = _this->head; _this->head = add_elem; }
    else { next_elem = prev_elem->next; prev_elem->next = add_elem; }

    add_elem->next = next_elem;

    if (!(_this->tail) || (prev_elem == _this->tail))
    {
        if ((_this->tail) && (prev_elem == _this->tail)) {add_elem->prev = prev_elem;}
        _this->tail = add_elem;
    }
    else
    {
        add_elem->prev = prev_elem;
        next_elem->prev = add_elem;
    }


    (_this->nElem)++;
    assert(!list_ok(_this));

    return 0;
}

int list_del(list_t* _this, list_elem_t* del_elem)
{
    assert(!list_ok(_this));
    assert(del_elem);

    if (_this->head == del_elem)
    {
        _this->head = del_elem->next;
        del_elem->next->prev = NULL;
    }
    else if (_this->tail == del_elem)
    {
        _this->tail = del_elem->prev;
        del_elem->prev->next = NULL;
    }

    else
    {
        del_elem->prev->next = del_elem->next;
        del_elem->next->prev = del_elem->prev;
    }


    (_this->nElem)--;
    list_elem_dtor(del_elem);

    assert(!list_ok(_this));

    return 0;
}

int list_dump(list_t* _this)
{
    if (!_this) {printf("NO LIST\n"); return 0;}
    if (list_ok(_this)) {printf("bad list!!!"); return 0;}

    list_elem_t* elem = _this->head;

    for (int i = 1; i <= _this->nElem; i++)
    {
        printf("------------\n"
               "%12s\n"
               "%p ==>\n"
               "<== %p\n",
               elem->value,
               elem->next,
               elem->prev );

        elem = elem->next;

    }


    return 0;
}

int list_ok(list_t* _this)
{
    if (_this) return ENULLPTR;

    if (_this->nElem)
    {
        if (!_this->head) return ENOHEAD;
        if (!_this->tail) return ENOTAIL;

        for (list_elem_t* ptr = _this->head;
            ptr != _this->tail;
            ptr = ptr->next)
            {
                if (!ptr->next) {return EBADNEXT; break;}
                if ( ptr != _this->head && !ptr->prev)
                {
                    return EBADPREV;
                    break;
                }

            }
    }
    return 0;
}


int list_find(list_t* _this, char* value)
{
    assert(!list_ok(_this));
    assert(value);

    if (!_this->head) return 0;
    list_elem_t* elem = _this->head;

    for (int i = 0; i < _this->nElem-1; i++)
    {
        if (!strcmp(elem->value, value)) return 1;
        elem = elem->next;
    }
    if (!strcmp(elem->value, value)) return 1;

    return 0;
}

int list_elem_dtor(list_elem_t* destr_elem)
{
    if(!destr_elem) return 0;

    destr_elem->next  = NULL;
    destr_elem->prev  = NULL;
    destr_elem->value = NULL;

    free(destr_elem);
    return 0;
}

list_t* list_dtor(list_t* _this)
{
    assert(_this);

    if (_this->head)
    {
        list_elem_t* elem = _this->head;

        for (int i = 1; i < _this->nElem; i++)
        {
            elem = elem->next;
            list_elem_dtor(elem->prev);
        }
        list_elem_dtor(elem);

        _this->head  = NULL;
        _this->tail  = NULL;
        _this->nElem = 0;
    }

    free(_this);
    return NULL;
}


