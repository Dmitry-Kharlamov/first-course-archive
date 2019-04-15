#ifndef LEXER_H_INCLUDED
#define LEXER_H_INCLUDED

#define SKIPSPACES while(*str && isspace(*str)) str++;

const int MAX_TOKEN = 5000;

struct token
{
    info    val;
    int     val_type;

};

token* new_stoken(int type, char* str);

token* new_dtoken(int type, int num);

token** lexer(char* str);

void token_dump(token** tks);

#endif // LEXER_H_INCLUDED
