#include "hash.h"

#include<stdlib.h>
#include<stdio.h>
#include<assert.h>
#include<ctype.h>

const int MAX_LEN_WORD = 100;

//! return 0 when read, 1 when didn't read
int read_word(char** text_ptr, char* word)
{
    assert(text_ptr);
    assert(*text_ptr);
    assert(word);

    char* text = *text_ptr;

    while(!isalpha(*text)) {text++; if(!*text) break;}
    if(!*text) return 1;

    char* ptr = word;
    while(isalpha(*text))
    {
        *ptr = *text;
        text++;
        ptr++;
        if ((ptr - word + 1) > MAX_LEN_WORD) assert(!"too long str!");
    }

    *ptr = '\0';

    if(!*text) return 1;
    *text_ptr = text;

    return 0;
}



int main()
{
    hash_t* hash_tab1 = hash_ctor(3,   *hash_func1);
    hash_t* hash_tab2 = hash_ctor(31,  *hash_func2);
    hash_t* hash_tab3 = hash_ctor(31,  *hash_func3);
    hash_t* hash_tab4 = hash_ctor(911, *hash_func4);
    hash_t* hash_tab5 = hash_ctor(911, *hash_func5);

    FILE* in = fopen("crime&punishment_Ch1.txt", "r");
    assert(in);

    char str[MAX_LEN_WORD] = "";
    fseek(in, 0, SEEK_END);
    int len = ftell(in);

    fseek(in, 0,SEEK_SET);

    char* text = (char*) calloc(len+1, 1);
    assert(text);

    fread(text, len, 1, in);

    while(*text)
    {
        if (read_word(&text, str)) break;

        hash_table_add(hash_tab1, str);
        hash_table_add(hash_tab2, str);
        hash_table_add(hash_tab3, str);
        hash_table_add(hash_tab4, str);
        hash_table_add(hash_tab5, str);

    }


    hash_dump(hash_tab1, "hash_func1");
    hash_dump(hash_tab2, "hash_func2");
    hash_dump(hash_tab3, "hash_func3");
    hash_dump(hash_tab4, "hash_func4");
    hash_dump(hash_tab5, "hash_func5");

    hash_dtor(hash_tab1);
    hash_dtor(hash_tab2);
    hash_dtor(hash_tab3);
    hash_dtor(hash_tab4);
    hash_dtor(hash_tab5);

    return 0;

}


