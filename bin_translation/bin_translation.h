#ifndef BIN_TRANSLATION_H_INCLUDED
#define BIN_TRANSLATION_H_INCLUDED


int bin_translation(translated_t* tr);

void exec(char* code);

#define mac(name) \
    int translation_##name(translated_t* tr);

#include "commands.h"

#undef mac(name)

int get_reladdr (char* opcode, translated_t* tr);

int get_varaddr (char* opcode, translated_t* tr);

int get_modrr   (char* opcode);

int get_transreg(char* opcode);

int patch_jmp(translated_t* tr);

void emulated_OUT_(int x);


#endif // BIN_TRANSLATION_H_INCLUDED
