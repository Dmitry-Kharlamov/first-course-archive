/* BINARY TRANSLATER FROM DSKh asm to x86
** input *.code file, translating and
** execute it.
** email: harldmitr@gmail.com
** 05.2015 (@)Dmitry Kharlamov
*/

#include "input.h"
#include "mem_alloc.h"
#include "resources.h"
#include "bin_translation.h"


int main(int argc, char** argv)
{
    char* source = NULL;

    if (argc == 2) source = input(argv[1]);
    else
    {
        source = input("program.code");
    }

    translated_t* translated = mem_allocation(source);

    bin_translation(translated);    //translate to x86 code

    dump_tr_code(translated->code); //save x86 code

    exec(translated->code);         //execute x86 code

    return 0;
}
