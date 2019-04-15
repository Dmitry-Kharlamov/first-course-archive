/**
    @file
    This program is my realization stack CPU.
    It gets byte code files and execute it.
    Output - console. Logs - logs.txt

    @author Dmitry Kharlamov, student of 1st course DREC MIPT.
    @version 1.1
*/

#include "stack\stack.h"
#include "resources.h"
#include "CPU.h"
#include "commands.h"


#define ERRHANDLER if (__errnum) { errlog(__errnum, __FILE__, __LINE__); cpu_dump(_this); return __errnum; }
#define $ __errnum =
#define CPUERR(errfunc)                             \
{                                                   \
    printf("CPU CRASHED! %s HAVEN'T FULFILLED",     \
           errfunc);                                \
    cpu_dump(cpu);                                  \
}

const int              FOREVER  = 1;
const int        CPU_STACK_MAX  = 10000;
const int   MAX_CALL_STACK_LEN  = 100;

struct cpu_t_
{
    stack_t     *stk;
    double      ax;
    double      bx;
    double      cx;
    double      dx;
    stack_t     *call;
    char*       addr;
    double*     rom;
    bool        vars[MAX_VAR];

};

/**
    This function inputs byte code of instructions

    @param      code   pointer on array of byte code
    @param file_name   name of file to input

    @return number of error(0 if no error)
*/
int cpu_input_code(char** code, char* file_name)
{

    FILE* in_file = fopen(file_name, "rb");
    assert(in_file);

    void* buf = calloc(CODE_SIZE, 1);
    assert(buf);

    fread(buf, 1, CODE_SIZE, in_file);
    assert(buf);

    fclose(in_file);

    *code = (char*) buf;

    return 0;
}

/**
    This function exec byte code of instructions

    @param      code  array of byte code

    @return number of error(0 if no error)
*/
int cpu_run(char* addr_code)
{
    assert(addr_code);

    cpu_t* cpu = NULL;

    cpu_ctor(&cpu, CPU_STACK_MAX, addr_code);

    char* current_byte = addr_code;

    start_logging("logs.txt");


    while(FOREVER)
    {
        int cmd = *current_byte;
        current_byte++;

        switch(cmd)
        {
        case PUSH:
            if(cpu_push(cpu, &current_byte))    CPUERR("PUSH");
            break;

        case POP:
            if(cpu_pop(cpu, &current_byte))     CPUERR("POP");
            break;

        case OK:
            if(cpu_ok(cpu)) printf("CPU CRASHED! CPU ISN'T OK");
            break;

        case DUMP:
            if(cpu_dump(cpu))                   CPUERR("DUMP");
            break;

        case ADD:
            if(cpu_add(cpu))                    CPUERR("ADD");
            break;

        case SUB:
            if(cpu_sub(cpu))                    CPUERR("SUB");
            break;

        case MUL:
            if(cpu_mul(cpu))                    CPUERR("PUSH");
            break;

        case DIR:
            if(cpu_dir(cpu))                    CPUERR("DIR");
            break;

        case SIN:
            if(cpu_sin(cpu))                    CPUERR("SIN");
            break;

        case COS:
            if(cpu_cos(cpu))                    CPUERR("COS");
            break;

        case SQRT:
            if(cpu_sqrt(cpu))                   CPUERR("SQRT");
            break;

        case JA:
            if(cpu_ja(cpu, &current_byte))      CPUERR("JA");
            break;

        case JAE:
            if(cpu_jae(cpu, &current_byte))     CPUERR("JAE");
            break;

        case JB:
            if(cpu_jb(cpu, &current_byte))      CPUERR("JB");
            break;

        case JBE:
            if(cpu_jbe(cpu, &current_byte))     CPUERR("JBE");
            break;

        case JE:
            if(cpu_je(cpu, &current_byte))      CPUERR("JE");
            break;

        case JNE:
            if(cpu_jne(cpu, &current_byte))     CPUERR("JNE");
            break;

        case JMP:
            if(cpu_jmp(cpu, &current_byte))     CPUERR("JMP");
            break;

        case OUT:
            if(cpu_out(cpu))                    CPUERR("OUT");
            break;

        case MOV:
            if(cpu_mov(cpu, &current_byte))     CPUERR("MOV");
            break;

        case INC:
            if(cpu_inc(cpu, &current_byte))     CPUERR("INC");

        case DEC:
            if(cpu_dec(cpu, &current_byte))     CPUERR("DEC");
            break;

        case CALL:
            if(cpu_call(cpu, &current_byte))    CPUERR("CALL");
            break;

        case RET:
            if(cpu_ret(cpu, &current_byte))     CPUERR("RET");
            break;

        case END:
            return 0;
            break;
        default:
        {
            fprintf(stderr, "ERROR! Unknown command at address %o. \n", (int)current_byte);
            return 1;
        }

        }

    }
    return 0;
}

int cpu_ctor(cpu_t** _this, int length, char* addr)
{
    int $ 0;
    cpu_t* cpu = (cpu_t*) calloc(sizeof(cpu_t), 1);

    ISNOTNULL(cpu);
    assert(length > 0);

    $ stack_Ctor(&(cpu->stk), length);

    $ stack_Ctor(&(cpu->call), MAX_CALL_STACK_LEN);


    if (__errnum)
        {
            errlog(__errnum, __FILE__, __LINE__);
            cpu_dump(*_this);
            return __errnum;
        }

    cpu->ax = 0.0;
    cpu->bx = 0.0;
    cpu->cx = 0.0;
    cpu->dx = 0.0;
    cpu->addr = addr;

    double* rom = (double*) calloc(sizeof(*rom), MAX_VAR);

    ISNOTNULL(rom);
    cpu->rom = rom;

    for (int i = 0; i< MAX_VAR; i++) cpu->vars[i] = false;

    *_this = cpu;

    return 0;
}

int cpu_push(cpu_t* _this, char** ptr_current_byte)
{
    ISNOTNULL(_this);
    int $ 0;

    char* current_byte = *ptr_current_byte;

    switch (*current_byte)
    {
    case NUM:
    {
        current_byte++;

        double a = *(double*)current_byte;

        $ stack_Push(_this->stk, a);

        current_byte += sizeof(double);
    }
    break;

    case REG:
    {
        current_byte++;

        switch (*current_byte)
        {
        case AX:
            $ stack_Push(_this->stk, _this->ax);
            break;
        case BX:
            $ stack_Push(_this->stk, _this->bx);
            break;
        case CX:
            $ stack_Push(_this->stk, _this->cx);
            break;
        case DX:
            $ stack_Push(_this->stk, _this->dx);
            break;
        }
        current_byte++;
    }
    break;

    case VAR:
    {
        current_byte++;
        char var_num = *current_byte;
        current_byte++;

        if (_this->vars[var_num])
            $ stack_Push(_this->stk, _this->rom[var_num]);
        else
            return ENODEFVAR;

    }
    break;

    default:
       return ECPUPUSH;
    }
    ERRHANDLER;

    *ptr_current_byte = current_byte;
    return 0;
}

int cpu_pop(cpu_t* _this, char** ptr_current_byte)
{
    ISNOTNULL(_this);
    ISNOTNULL(ptr_current_byte);
    ISNOTNULL(*ptr_current_byte);
    int $ 0;

    char* current_byte = *ptr_current_byte;

    switch (*current_byte)
    {
    case AX:
        $ stack_Pop(_this->stk, &(_this->ax));
        break;
    case BX:
        $ stack_Pop(_this->stk, &(_this->bx));
        break;
    case CX:
        $ stack_Pop(_this->stk, &(_this->cx));
        break;
    case DX:
        $ stack_Pop(_this->stk, &(_this->dx));
        break;
    }

    ERRHANDLER;

    (*ptr_current_byte)++;

    return 0;
}

int cpu_ok(cpu_t* _this)
{
    ISNOTNULL(_this);
    ISNOTNULL(_this->addr);
    return stack_Ok(_this->stk);


    return 0;
}

int cpu_dump(cpu_t* _this)
{
    ISNOTNULL(_this);

    stack_Dump(_this->stk);

    fprintf(stderr, "\nax %lg\nbx %lg\ncx %lg\ndx %lg\n\n",
            _this->ax, _this->bx, _this->cx, _this->dx);


    if (!_this->addr) fprintf(stderr, "Invalid address to byte code\n");
    else for(char* ptr = _this->addr; *ptr != END; ptr++)
    {
        if ((ptr - _this->addr) > CODE_SIZE) break;
        int byte = *ptr;
        fprintf(stderr, "%02x ", byte);
    }
    fprintf(stderr, "\n\n");


}

int cpu_add(cpu_t* _this)
{
    ISNOTNULL(_this);
    int $ 0;

    double a = 0.0;

    $ stack_Pop(_this->stk, &a);

    ERRHANDLER;

    double b = 0.0;

    $ stack_Pop(_this->stk, &b);

    ERRHANDLER;

    $ stack_Push(_this->stk, (a + b));

    ERRHANDLER;

    return 0;
}

int cpu_sub(cpu_t* _this)
{
    ISNOTNULL(_this);
    int $ 0;

    double a = 0.0;

    $ stack_Pop(_this->stk, &a);

    ERRHANDLER;

    double b = 0.0;

    $ stack_Pop(_this->stk, &b);

    ERRHANDLER;

    $ stack_Push(_this->stk, (b - a));

    ERRHANDLER;

    return 0;

}

int cpu_mul(cpu_t* _this)
{
    ISNOTNULL(_this);
    int $ 0;

    double a = 0.0;

    $ stack_Pop(_this->stk, &a);

    ERRHANDLER;

    double b = 0.0;

    $ stack_Pop(_this->stk, &b);

    ERRHANDLER;

    $ stack_Push(_this->stk, (a * b));

    ERRHANDLER;

    return 0;

}

int cpu_dir(cpu_t* _this)
{
    ISNOTNULL(_this);
    int $ 0;

    double a = 0.0;

    $ stack_Pop(_this->stk, &a);

    ERRHANDLER;

    double b = 0.0;

    $ stack_Pop(_this->stk, &b);

    ERRHANDLER;

    $ stack_Push(_this->stk, (b / a));

    ERRHANDLER;

    return 0;

}

int cpu_sin(cpu_t* _this)
{
    ISNOTNULL(_this);
    int $ 0;

    double a = 0.0;

    $ stack_Pop(_this->stk, &a);

    ERRHANDLER;

    $ stack_Push(_this->stk, sin(a));

    ERRHANDLER;

    return 0;

}

int cpu_cos(cpu_t* _this)
{
    ISNOTNULL(_this);
    int $ 0;

    double a = 0.0;

    $ stack_Pop(_this->stk, &a);

    ERRHANDLER;

    $ stack_Push(_this->stk, cos(a));

    ERRHANDLER;

    return 0;

}

int cpu_sqrt(cpu_t* _this)
{
    ISNOTNULL(_this);
    int $ 0;

    double a = 0.0;

    $ stack_Pop(_this->stk, &a);

    ERRHANDLER;

    $ stack_Push(_this->stk, sqrt(a));

    ERRHANDLER;

    return 0;

}

#define JMP_IF(cond)                    \
    ISNOTNULL(current_byte);            \
    ISNOTNULL(*current_byte);           \
    ISNOTNULL(_this);                   \
    int $ 0;                            \
                                        \
    double a = 0.0;                     \
                                        \
    $ stack_Pop(_this->stk, &a);        \
                                        \
    ERRHANDLER;                         \
                                        \
    double b = 0.0;                     \
                                        \
    $ stack_Pop(_this->stk, &b);        \
                                        \
    ERRHANDLER;                         \
                                        \
    int label = *(int*)(*current_byte); \
                                        \
    if (label < 0) $ EJMPBADLABEL;      \
                                        \
    ERRHANDLER;                         \
                                        \
    if (#cond)                          \
        *current_byte = _this->addr     \
                            + label;    \
    else *current_byte += sizeof(int);  \
                                        \
    return 0;                           \


int cpu_ja(cpu_t* _this, char** current_byte)
{
    JMP_IF("a > b");
}

int cpu_jae(cpu_t* _this, char** current_byte)
{
    JMP_IF("a >= b");
}

int cpu_jb(cpu_t* _this, char** current_byte)
{
    JMP_IF("a < b");
}

int cpu_jbe(cpu_t* _this, char** current_byte)
{
    JMP_IF("a >= b");
}

int cpu_je(cpu_t* _this, char** current_byte)
{
    ISNOTNULL(current_byte);
    ISNOTNULL(*current_byte);
    ISNOTNULL(_this);
    int $ 0;

    double a = 0.0;

    $ stack_Pop(_this->stk, &a);

    ERRHANDLER;

    int label = *(int*)(*current_byte);

    if (label < 0) $ EJMPBADLABEL;

    ERRHANDLER;

    if (a < 1E-8)
        *current_byte = _this->addr
                            + label;
    else *current_byte += sizeof(int);

    return 0;

}

int cpu_jne(cpu_t* _this, char** current_byte)
{
    ISNOTNULL(current_byte);
    ISNOTNULL(*current_byte);
    ISNOTNULL(_this);
    int $ 0;

    double a = 0.0;

    $ stack_Pop(_this->stk, &a);

    ERRHANDLER;

    int label = *(int*)(*current_byte);

    if (label < 0) $ EJMPBADLABEL;

    ERRHANDLER;

    if (a > 0)
        *current_byte = _this->addr
                            + label;
    else *current_byte += sizeof(int);

    return 0;

}

int cpu_jmp(cpu_t* _this, char** current_byte)
{
    ISNOTNULL(current_byte);
    ISNOTNULL(*current_byte);
    ISNOTNULL(_this);
    int $ 0;

    int label = *(int*)(*current_byte);

    if (label < 0) $ EJMPBADLABEL;

    ERRHANDLER;

    *current_byte = _this->addr + label;

    return 0;

}

int cpu_out(cpu_t* _this)
{
    ISNOTNULL(_this);
    int $ 0;

    double a = 0.0;

    $ stack_Pop(_this->stk, &a);

    ERRHANDLER;

    $ stack_Push(_this->stk, a);

    ERRHANDLER;

    printf("%lg\n", a);

    return 0;
}

int cpu_mov(cpu_t* _this, char** ptr_current_byte)
{
    ISNOTNULL(ptr_current_byte);
    ISNOTNULL(*ptr_current_byte);
    ISNOTNULL(_this);

    char* current_byte = *ptr_current_byte;
    int $ 0;
    switch (*current_byte)
    {
    case REG:
    {
        current_byte++;
        int reg = (int) *current_byte;
        current_byte++;

        int val = 0;

        switch (*current_byte)
        {

        case NUM:
        {
            current_byte++;
            val = *(double*)current_byte;
            current_byte += sizeof(double);
        }
        break;

        case REG:
        {
            current_byte++;
            int reg2 = (int) *current_byte;
            current_byte++;


            switch (reg2)
            {
            case AX:
                val = _this->ax;
                break;
            case BX:
                val = _this->bx;
                break;
            case CX:
                val = _this->cx;
                break;
            case DX:
                val = _this->dx;
                break;
            }

        }
        break;

        case VAR:
        {
            current_byte++;
            int var_num2 = (int) *current_byte;
            current_byte++;

            if (_this->vars[var_num2])
                val = _this->rom[var_num2];
            else return ENODEFVAR;
        }
        break;

        }

        switch (reg)
        {
        case AX:
            _this->ax = val;
            break;
        case BX:
            _this->bx = val;
            break;
        case CX:
            _this->cx = val;
            break;
        case DX:
            _this->dx = val;
            break;
        }
    }
    break;

    case VAR:
    {
        current_byte++;
        char var_num = *current_byte;
        current_byte++;

        int val = 0;

        switch (*current_byte)
        {

        case NUM:
        {
            current_byte++;
            val = *(double*)current_byte;
            current_byte += sizeof(double);
        }
        break;

        case REG:
        {
            current_byte++;
            int reg2 = (int) *current_byte;
            current_byte++;


            switch (reg2)
            {
            case AX:
                val = _this->ax;
                break;
            case BX:
                val = _this->bx;
                break;
            case CX:
                val = _this->cx;
                break;
            case DX:
                val = _this->dx;
                break;
            }

        }
        break;

        case VAR:
        {
            current_byte++;
            int var_num2 = (int) *current_byte;
            current_byte++;

            if (_this->vars[var_num2])
                val = _this->rom[var_num2];
            else return ENODEFVAR;
        }
        break;

        }
        _this->vars[var_num] = true;

        _this->rom[var_num]  = val;


    }
    break;

    }

    *ptr_current_byte = current_byte;

    return 0;
}

int cpu_inc(cpu_t* _this, char** ptr_current_byte)
{
    ISNOTNULL(ptr_current_byte);
    ISNOTNULL(*ptr_current_byte);
    ISNOTNULL(_this);

    char* current_byte = *ptr_current_byte;
    int $ 0;


    if (*current_byte == NOARG)
    {
        current_byte++;
        double a = 0.0;

        $ stack_Pop(_this->stk, &a);

        ERRHANDLER;

        $ stack_Push(_this->stk, (a + 1));

        ERRHANDLER;
    }
    else if (*current_byte == REG)
    {
        current_byte++;

        switch (*current_byte)
        {
        case AX:
            _this->ax++;
            break;
        case BX:
            _this->bx++;
            break;
        case CX:
            _this->cx++;
            break;
        case DX:
            _this->dx++;
            break;

        }
        current_byte++;

    }



    *ptr_current_byte = current_byte;

    return 0;
}

int cpu_dec(cpu_t* _this, char** ptr_current_byte)
{
    ISNOTNULL(ptr_current_byte);
    ISNOTNULL(*ptr_current_byte);
    ISNOTNULL(_this);

    char* current_byte = *ptr_current_byte;
    int $ 0;


    if (*current_byte == NOARG)
    {
        current_byte++;
        double a = 0.0;

        $ stack_Pop(_this->stk, &a);

        ERRHANDLER;

        $ stack_Push(_this->stk, (a - 1));

        ERRHANDLER;
    }
    else if (*current_byte == REG)
    {
        current_byte++;
        switch (*current_byte)
        {
        case AX:
            _this->ax--;
            break;
        case BX:
            _this->bx--;
            break;
        case CX:
            _this->cx--;
            break;
        case DX:
            _this->dx--;
            break;
        }

        current_byte++;

    }



    *ptr_current_byte = current_byte;

    return 0;
}


int cpu_call(cpu_t* _this, char** ptr_current_byte)
{
    ISNOTNULL(ptr_current_byte);
    ISNOTNULL(*ptr_current_byte);
    ISNOTNULL(_this);

    char* current_byte = *ptr_current_byte;
    int $ 0;

    int addr_func = *(int*)current_byte;
    current_byte += sizeof(int);


    if (addr_func < 0) $ EJMPBADLABEL;

    ERRHANDLER;

    $ stack_Push(_this->call, (double)(int)current_byte);

    ERRHANDLER;

    current_byte = _this->addr + addr_func;

    *ptr_current_byte = current_byte;

    return 0;
}

int cpu_ret(cpu_t* _this, char** ptr_current_byte)
{
    ISNOTNULL(ptr_current_byte);
    ISNOTNULL(*ptr_current_byte);
    ISNOTNULL(_this);

    char* current_byte = *ptr_current_byte;
    int $ 0;

    double addr_d = 0;

    $ stack_Pop(_this->call, &addr_d);

    ERRHANDLER;

    int addr_func = (int) addr_d;

    if (addr_func < 0) $ EJMPBADLABEL;

    ERRHANDLER;


    current_byte = (char*)addr_func;


    *ptr_current_byte = current_byte;


    return 0;

}

int cpu_end(cpu_t* _this)
{
    ISNOTNULL(_this);
    end_logging();
    return 0;
}
