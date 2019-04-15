#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

struct list_elem_t
{
    char* value;
    list_elem_t* next;
    list_elem_t* prev;
};

struct list_t
{
    list_elem_t* head;
    list_elem_t* tail;
    int nElem;
};

enum ERRORSCODES
{
    ENULLPTR = 0,
    EBADLIST,
    EBADNEXT,
    EBADPREV,
    ENOHEAD,
    ENOTAIL,
    ENOFUNC,
    ENOTABLE,
    EBADSIZE

};

list_t* list_ctor();

list_elem_t* list_elem_ctor(char* value);

int list_add(list_t* _this, list_elem_t* prev_elem, char* value);

int list_del(list_t* _this, list_elem_t* del_elem);

int list_find(list_t* _this, char* value);

int list_dump(list_t* _this);

int list_ok(list_t* _this);

list_t* list_dtor(list_t* _this);

int list_elem_dtor(list_elem_t* destr_elem);


#endif // LIST_H_INCLUDED
