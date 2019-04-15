#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "commands.h"


int main()
{
    FILE* in = fopen("program.txt", "r");
    char cmd[15] = "";


    void* cpu_code = calloc(16384, 1);

    char* ptr = (char*) cpu_code;


    fscanf(in, "%s", cmd);
    int i = BEGIN;
    while (i <= END)
    {
        if (!strcmp(cmd, S_CMD[i - BEGIN]))
        {

            *ptr = i;
            ptr++;

            if (i == END) break;
            else if (i > END) assert(!"undefined command");

            if (i == PUSH || i == POP)
            {
                double val = 0;
                if (fscanf(in, "%lg", &val))
                {
                    *ptr = NUM;
                    ptr++;
                    *(double*)ptr = val;
                    ptr += sizeof(double);


                }

                else
                {
                    *ptr = REG;
                    ptr++;
                    char reg[5] = "";
                    fscanf(in, "%s", reg);
                    if (!strcmp(reg, S_REG[0])) {*ptr = AX; ptr++;}
                    if (!strcmp(reg, S_REG[1])) {*ptr = BX; ptr++;}
                    if (!strcmp(reg, S_REG[2])) {*ptr = CX; ptr++;}
                    if (!strcmp(reg, S_REG[3])) {*ptr = DX; ptr++;}

                }

            }

            if (JA <= i && i <= JMP)
            {
               int num = 0;

               fscanf(in, "%d", &num);

               if(0 > num) assert(!"bad pointer");

               *(int*)ptr = num;
               ptr += sizeof(int);
            }

            fscanf(in, "%s", cmd);
            i = BEGIN;
        }
        else i++;

    }
    fclose(in);

    FILE* out = fopen("asmprog.txt", "w");
    fwrite(cpu_code, sizeof(char), (int)ptr - (int)cpu_code, out);
    free(cpu_code);

    fclose(out);
}
