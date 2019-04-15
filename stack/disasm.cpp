#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "commands.h"


int main()
{
    FILE* in = fopen("asmprog.txt", "r");

    void* buf = calloc(16384, 1);

    fread(buf, 1, 16384, in);

    fclose(in);

    char* ptr = (char*) buf;


    FILE* out = fopen("disasmprog.txt", "w");

    int i = *ptr;
    ptr++;
    while (i != END)
    {
        fprintf(out, "%s", S_CMD[i - BEGIN]);
        if (i == PUSH || i == POP)
        {
            if (*ptr == NUM)
            {
                ptr++;
                fprintf(out, " %lg", *(double*)ptr);
                ptr += sizeof(double);
            }
            else
            {
                ptr++;
                fprintf(out, " %s", S_REG[*ptr - AX]);
                ptr++;
            }
        }

        if (JA <= i && i <= JMP)
        {
               fprintf(out, " %d", *(int*)ptr);

               ptr += sizeof(int);
        }




        fprintf(out, "\n");

        i = *ptr;
        ptr++;

    }
    fprintf(out, "end\n");
    fclose(out);

    free(buf);
}

