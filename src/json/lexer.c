
#include "lexer.h"
#include "common/string.h"
#include "common/common.h"

#include <ctype.h>
#include <string.h>

static Vector tokens;
static const char* source;
static size_t pos;
static size_t line;

static Token make_token(TokenType type, String* value)
{
    Token tok = {
        .type = type,
        .value = value->data,
        .line = line
    };
    return tok;
}

static Token make_tokench(TokenType type, char c) {
    Token tok = {
        .type = type,
        .value = common_malloc(sizeof(char) * 2),
        .line = line
    };
    tok.value[0] = c;
    tok.value[1] = '\0';
    return tok;
}

static char at()
{
    return source[pos];
}

static void advance()
{
    if (at() == '\n')
        line += 1;
    pos += 1;
}

static bool not_end()
{
    return source[pos] != '\0';
}

static char eat()
{
    char c = at();
    advance();
    return c;
}

static Token make_errorch(const char* desc, char c, char* type)
{
    String err = str_new();
    str_append(&err, desc);
    str_append(&err, " '");
    str_write(&err, c);
    str_write(&err, '\'');
    Token tok = make_token(TK_ERROR, &err);
    tok.errtype = type;
    return tok;
}

static Token make_errorstr(const char* desc, String* str, char* type)
{
    String err = str_new();
    str_append(&err, desc);
    if (str != NULL) {
        str_append(&err, " '");
        str_append(&err, str->data);
        str_destroy(str);
        str_write(&err, '\'');
    }
    Token tok = make_token(TK_ERROR, &err);
    tok.errtype = type;  
    return tok;
}

static Token lex_string()
{
    String string = str_new();
    advance();

    while (at() != '\"' && not_end())
        str_write(&string, eat());

    if (!not_end()) {
        str_destroy(&string);
        return make_errorstr("missing \"", NULL, "Invalid String");
    }

    advance();
    return make_token(TK_STRING, &string);
}

static Token lex_keyword()
{
    String keyword = str_frombuf(&(char){eat()}, 1);
    TokenType type;

    while (isalpha(at()))
        str_write(&keyword, eat());

    if (!strcmp(keyword.data, "null")) 
        type = KW_NULL;
    else if (!strcmp(keyword.data, "true"))
        type = KW_TRUE;
    else if (!strcmp(keyword.data, "false"))
        type = KW_FALSE;
    else 
        return make_errorstr("unknown identifier", &keyword, "Invalid Keyword");

    return make_token(type, &keyword);
}

static Token lex_number()
{
    String number = str_new();

    if (at() == '-')
        str_write(&number, eat());

    if (at() == '0') {
        str_write(&number, eat());
    } else if (isdigit(at())) {
        while (isdigit(at()))
            str_write(&number, eat());
    } else {
        str_destroy(&number);
        return make_errorstr("expected digit after '-'", NULL, "Invalid Number");
    }

    if (at() == '.') {
        str_write(&number, eat());
        if (!isdigit(at())) {
            str_destroy(&number);
            return make_errorstr("digit expected after decimal point", NULL, "Invalid Number");
        }

        while (isdigit(at()))
            str_write(&number, eat());
    }

    if (at() == 'e' || at() == 'E') {
        str_write(&number, eat());

        if (at() == '-' || at() == '+')
            str_write(&number, eat());

        if (!isdigit(at())) {
            str_destroy(&number);
            return make_errorstr("exponent must have at least one digit", NULL, "Invalid Number");
        }

        while (isdigit(at()))
            str_write(&number, eat());
    }

    return make_token(TK_NUMBER, &number);
}

static Token read_token()
{
    #define symbol_type(type, c) \
        case c: \
            return make_tokench(type, eat());

    switch (at()) {
        case '"':
            return lex_string();
        #include "symbols.inc"
        
        default: {
            if (isalpha(at())) 
                return lex_keyword();
            
            if (isdigit(at()) || at() == '-')
                return lex_number();
        }
    }
    #undef symbol_type
    return make_errorch("stray character", eat(), "Invalid Character");
}

static bool at_whitespace()
{
    return at() == ' ' || at() == '\r' || at() == '\n';
}

static void write_token()
{
    while (at_whitespace())
        advance();

    Token tok = read_token();
    vec_push(&tokens, &tok);
}

Token* json_tokenize(const char* src)
{
    tokens = vec_new(sizeof(Token));
    source = src;
    line = 1;
    pos = 0;

    while (not_end())
        write_token();

    String eof = str_newstring("(eof)");
    Token eof_tok = make_token(TK_EOF, &eof);

    vec_push(&tokens, &eof_tok);
    return (Token*)tokens.data;
}