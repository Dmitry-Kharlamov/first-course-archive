#include "resources.h"
#include "stdio.h"
#include "string.h"
#include "bin_translation.h"

char* opcode = NULL;            //global var for current command in source opcode

char printf_str[] = ">\t%d\n";  //format of printf in OUT_ ("%d" expected)

jmp_t jmp[MAX_CODE_SEG];        // store jmp param for patching
int n_jmp = 0;

int bin_translation(translated_t* tr)
{
    opcode = tr->source;

    while (*(opcode) != 0)
    {
        switch(*opcode++)
        {

#define mac(name)                       \
    case name:                          \
        translation_##name(tr);         \
        break;                          \


#include "commands.h"
#undef mac(name)

        }

        tr->addrconv[opcode - tr->source] = tr->counter;
    }

    patch_jmp(tr);

}

void exec(char* code)
{
    int (*f)(void) = (int (*)(void)) code;
    f();
}

//=================================================================================================
//===============================TRANSLATION HELPER FUNC===========================================
//=================================================================================================

int get_lit_end(int big_end)
{
    int lit_end = 0;

    lit_end += (big_end & 0x000000FF) << 24;
    lit_end += (big_end & 0x0000FF00) << 8;
    lit_end += (big_end & 0x00FF0000) >> 8;
    lit_end += (big_end & 0xFF000000) >> 24;

    return lit_end;
}

int get_varaddr(char* opcode, translated_t* tr)
{
    return (int)tr->data + *opcode * sizeof(int);
}

int get_modrr(char* opcode)
{
    return  192 + get_transreg(opcode) * 8 + get_transreg(opcode+2);
}

int get_transreg(char* opcode)
{
    return reg_conv[*opcode - AX];
}

int patch_jmp(translated_t* tr)
{
    for (int i = 0; i < n_jmp; i++)
    {
        *(int *)((int)tr->code + jmp[i].offset - sizeof(int)) =
        tr->addrconv[*(int *)jmp[i].opcode] - (int)jmp[i].offset;
    }
}

//=================================================================================================
//===============================TRANSLATION FUNC==================================================
//=================================================================================================

int translation_MOV (translated_t* tr)
{
    switch(*opcode++)
    {
    case REG:
        opcode++;
        switch(*opcode)
        {
        case REG:
            memcpy(tr->code + tr->counter, TR_MOV_RR, sizeof(TR_MOV_RR));
            tr->counter += sizeof(TR_MOV_RR);

            tr->code[tr->counter - 1] = get_modrr(opcode++ - 1);
            opcode++;
            break;

        case VAR:
            memcpy(tr->code + tr->counter, TR_MOV_RV, sizeof(TR_MOV_RV));
            tr->counter += sizeof(TR_MOV_RV);

            tr->code[tr->counter - 5] += get_transreg(opcode++ - 1) * 8;

            *(int *)(tr->code + tr->counter - sizeof(int)) = get_varaddr(opcode++, tr);

            break;

        case NUM:
            memcpy(tr->code + tr->counter, TR_MOV_RC, sizeof(TR_MOV_RC));
            tr->counter += sizeof(TR_MOV_RC);

            tr->code[tr->counter - 5] += get_transreg(opcode++ - 1);

            *(int *)(tr->code + tr->counter - sizeof(int)) = *(int *)opcode;
             opcode += sizeof(int);

            break;
        }

        break;

    case VAR:
        opcode++;
        switch(*opcode)
        {
        case REG:
            memcpy(tr->code + tr->counter, TR_MOV_VR, sizeof(TR_MOV_VR));
            tr->counter += sizeof(TR_MOV_VR);

            tr->code[tr->counter - 5] += get_transreg(opcode + 1) * 8;

            *(int *)(tr->code + tr->counter - sizeof(int)) = get_varaddr(opcode++ - 1, tr);

            break;

        case VAR:

            opcode++;

            break;

        case NUM:
            memcpy(tr->code + tr->counter, TR_MOV_VC, sizeof(TR_MOV_VR));
            tr->counter += sizeof(TR_MOV_VR);


            *(int *)(tr->code + tr->counter - sizeof(int)) = get_varaddr(opcode++ - 1, tr);

            *(int *)(tr->code + tr->counter - sizeof(int)) = *(int *)opcode;
            opcode += sizeof(int);

            break;

        }

    }

}

int translation_PUSH(translated_t* tr) //char* opcode)
{
    switch(*opcode++)
    {
    case REG:

        tr->code[tr->counter++] = TR_PUSH_REG + get_transreg(opcode++);

        break;

    case NUM:

        memcpy(tr->code + tr->counter, TR_PUSH_CON, sizeof(TR_PUSH_CON));
        tr->counter += sizeof(TR_PUSH_CON);

        *(int *)(tr->code + tr->counter - sizeof(int)) = *(int *)opcode;
        opcode += sizeof(int);

        break;

    case VAR:

        memcpy(tr->code + tr->counter, TR_PUSH_VAR, sizeof(TR_PUSH_VAR));
        tr->counter += sizeof(TR_PUSH_VAR);

        *(int *)(tr->code + tr->counter - sizeof(int)) = get_varaddr(opcode++, tr);

        break;

    }

}

int translation_POP (translated_t* tr)
{

    tr->code[tr->counter++] = TR_POP + get_transreg(opcode++);

}

int translation_INC (translated_t* tr)
{
    if (*opcode++ == NOARG)
    {
        memcpy(tr->code + tr->counter, TR_INC_STK, sizeof(TR_INC_STK));
        tr->counter += sizeof(TR_INC_STK);
    }
    else
        tr->code[tr->counter++] = TR_INC_REG + get_transreg(opcode++);
}

int translation_DEC (translated_t* tr)
{
    if (*opcode++ == NOARG)
    {
        memcpy(tr->code + tr->counter, TR_DEC_STK, sizeof(TR_DEC_STK));
        tr->counter += sizeof(TR_DEC_STK);
    }
    else
        tr->code[tr->counter++] = TR_DEC_REG + get_transreg(opcode++);

}

int translation_RET (translated_t* tr)
{
    tr->code[tr->counter++] = TR_RETN;

}

int translation_BEGIN(translated_t* tr)
{
    tr->code[tr->counter++] = TR_BEGIN;

    opcode;
}


int translation_OUT_ (translated_t* tr)
{
    memcpy(tr->code + tr->counter, TR_OUT_, sizeof(TR_OUT_));


    *(int *)((int)tr->code + tr->counter + 1) =
    (int)printf_str;

    *(int *)((int)tr->code + tr->counter + 7) =
    (int)printf;
    tr->counter += sizeof(TR_OUT_);
}

int translation_END (translated_t* tr)
{
    tr->code[tr->counter++] = TR_RETN;
}


//==========================ARITHMETIC FUNC========================================

#define arith_func(name)                                                    \
int translation_##name(translated_t* tr)                                    \
{                                                                           \
    memcpy(tr->code + tr->counter, TR_##name, sizeof(TR_##name));           \
    tr->counter += sizeof(TR_##name);                                       \
                                                                            \
}


arith_func(ADD);

arith_func(SUB);

arith_func(MUL);

arith_func(DIV);


//==========================JMP FUNC==============================================

#define jmp_func(name)                                                          \
int translation_##name(translated_t* tr)                                        \
{                                                                               \
    memcpy(tr->code + tr->counter, TR_##name, sizeof(TR_##name));                                  \
    tr->counter += sizeof(TR_##name);                                           \
                                                                                \
    jmp[n_jmp].opcode = opcode;                                                 \
    jmp[n_jmp].offset = tr->counter;                                            \
    n_jmp++;                                                                    \
                                                                                \
    opcode += sizeof(int);                                                      \
                                                                                \
}



jmp_func(CALL);

jmp_func(JMP);

jmp_func(JE);

jmp_func(JNE);

jmp_func(JA);

jmp_func(JAE);

jmp_func(JB);

jmp_func(JBE);
