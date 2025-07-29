
#ifndef jlexer_h
#define jlexer_h

#include "common/vector.h"

typedef enum
{
    /* keywords */
    KW_TRUE,
    KW_FALSE,
    KW_NULL,

    /* symbols */
    SYM_COMMA,
    SYM_OPENBRACE,
    SYM_CLOSEBRACE,
    SYM_OPENBRACKET,
    SYM_CLOSEBRACKET,
    SYM_COLON,

    /* tokens */
    TK_STRING,
    TK_NUMBER,
    TK_EOF,
    TK_ERROR,
} TokenType;

typedef struct 
{
    TokenType type;
    char* value;
    char* errtype;
    size_t line;
} Token;

Token* json_tokenize(const char* source);

#endif