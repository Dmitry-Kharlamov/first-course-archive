#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

char* input(char* filename)
{
    FILE* file = fopen(filename, "rb");
    assert(file);

    fseek(file, 0, SEEK_END);
    int len = ftell(file) * 2;
    fseek(file, 0, SEEK_SET);

    char* data = (char*) calloc(len+1, sizeof(*data));
    assert(data);

    fread(data, len, 1, file);
    fclose(file);


    return data;
}



int dump_tr_code(char* code)
{
    FILE* file = fopen("last.code", "wb");
    assert(file);

    fwrite(code, 4096, 1, file);
    fclose(file);

}
