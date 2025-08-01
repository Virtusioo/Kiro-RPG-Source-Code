
#include "json.h"
#include "lexer.h"
#include "common/common.h"
#include "common/string.h"
#include "core/debug.h"

#include <string.h>
#include <stdlib.h>

static Token* tokens;
static size_t pos;
static String errors;

static Token* at()
{
    return tokens + pos;
}

static bool not_eof()
{
    return at()->type != TK_EOF;
}

static void advance()
{
    if (!not_eof())
        return;
    pos += 1;
}

static Token* eat()
{
    Token* tok = at();
    advance();
    return tok;
}

static char* expect(TokenType type, const char* message)
{
    Token* tok = eat();
    if (tok->type != type) {
        if (tok->type == TK_ERROR) 
            str_appendf(&errors, "line %zu: Syntax Error: %s, got invalid token\n", tok->line, message);
        else
            str_appendf(&errors, "line %zu: Syntax Error: %s, got '%s'\n", tok->line, message, tok->value);
        return NULL;
    }
    return strdup(tok->value);
}

static JsonValue* new_json(JsonType type, JsonUnion value)
{
    JsonValue* json = common_malloc(sizeof(JsonValue));
    json->type = type;
    json->value = value;
    return json;
}

static JsonValue* parse_expr();

static JsonValue* parse_keyword()
{
    TokenType type = eat()->type;
    if (type == KW_NULL)
        return new_json(JSON_NULL, (JsonUnion){0});

    if (type == KW_TRUE)
        return new_json(JSON_BOOL, (JsonUnion){.boolean = true});

    return new_json(JSON_BOOL, (JsonUnion){.boolean = false});
}

static JsonValue* parse_string()
{
    /* transfer ownership */
    char* str = strdup(eat()->value);
    return new_json(JSON_STRING, (JsonUnion){.string = str});
}

static JsonValue* parse_number()
{
    char* num_str = eat()->value;
    return new_json(JSON_NUMBER, (JsonUnion){.number = atof(num_str)});
}

static JsonValue* parse_primary()
{
    Token* tok = at();
    switch (tok->type) {
        case TK_ERROR: {
            str_appendf(&errors, "line %zu: %s: %s\n", tok->line, tok->errtype, tok->value);
            return NULL;
        }
        case KW_FALSE:
        case KW_NULL:
        case KW_TRUE:
            return parse_keyword();
        case TK_STRING:
            return parse_string();
        case TK_NUMBER:
            return parse_number();
    }
    /* lazy error handling :) */
    expect(TK_ERROR, "unexpected symbol");
    return NULL;
}

static void parse_arrayvalues(Vector* array)
{
    /* This is Safe: 
        vec_push() pushes the pointer into the vector 
        (which needs a starting address to copy the bytes of the pointer) 
    */
    vec_push(array, &(JsonValue*){parse_expr()});
    while (at()->type == SYM_COMMA && not_eof()) {
        Token* comma = eat();
        if (at()->type == SYM_CLOSEBRACKET) {
            str_appendf(&errors, "line %zu: Syntax Error: no trailing commas allowed\n", comma->line);
        }
        vec_push(array, &(JsonValue*){parse_expr()});
    }
    expect(SYM_CLOSEBRACKET, "expected ']' when closing array");
}

static JsonValue* parse_array()
{
    if (at()->type == SYM_OPENBRACKET) {
        advance();
        Vector array = vec_new(sizeof(JsonValue*));
        if (at()->type == SYM_CLOSEBRACKET) {
            advance();
            return new_json(JSON_ARRAY, (JsonUnion) {
                .array.length = 0,
                .array.data = (void*)array.data
            });
        }

        parse_arrayvalues(&array);

        return new_json(JSON_ARRAY, (JsonUnion) {
            .array.length = array.length,
            .array.data = (void*)array.data
        });
    }
    return parse_primary();
}

static void parse_kvpair(Map* object)
{
    char* key = expect(TK_STRING, "expected string as key");
    char* colon = expect(SYM_COLON, "expected ':' when seperating pair");
    if (colon == NULL || key == NULL) {
        common_free(colon);
        common_free(key);
        return;
    } 
    common_free(colon);
    map_set(object, key, &(JsonValue*){parse_expr()});
}

static void parse_kvpairs(Map* object)
{
    parse_kvpair(object);
    while (at()->type == SYM_COMMA && not_eof()) {
        Token* comma = eat();
        if (at()->type == SYM_CLOSEBRACE) {
            str_appendf(&errors, "line %zu: Syntax Error: no trailing commas allowed\n", comma->line);
        }
        parse_kvpair(object);
    }
    expect(SYM_CLOSEBRACE, "expected '}' when closing object");
}

static JsonValue* parse_object()
{
    if (at()->type == SYM_OPENBRACE) {
        advance();
        Map object = map_new(sizeof(JsonValue*));

        if (at()->type == SYM_CLOSEBRACE) {
            advance();
            return new_json(JSON_OBJECT, (JsonUnion){.object = object});
        }

        parse_kvpairs(&object);
        return new_json(JSON_OBJECT, (JsonUnion){.object = object});;
    }
    return parse_array();
}

static JsonValue* parse_expr()
{
    return parse_object();
}

JsonResult json_parse(const char* source)
{
    pos = 0;

    tokens = json_tokenize(source);
    errors = str_new();

    JsonResult result;
    result.value = parse_expr();

    if (errors.length != 0)
        result.errors = errors.data;
    else 
        result.errors = NULL;

    size_t i = 0;
    while (tokens[i].type != TK_EOF) 
        common_free(tokens[i++].value);
    common_free(tokens[i].value);
    common_free(tokens);
    
    return result;
}

void json_destroy(JsonValue* root)
{
    if (!root)
        return;
    if (root->type == JSON_ARRAY) {
        for (size_t i = 0; i < root->value.array.length; i++)
            json_destroy(root->value.array.data[i]);
    }
    if (root->type == JSON_STRING) {
        common_free(root->value.string);
    }
    if (root->type == JSON_OBJECT) {
        for (size_t i = 0; i < root->value.object.slots; i++) {
            MapEntry entry = root->value.object.entries[i];
            if (entry.occupied) {
                common_free(entry.key);
                json_destroy(*(JsonValue**)entry.value);
            }
        }
        map_destroy(&root->value.object);
    }
    common_free(root);
}

void json_destroyresult(JsonResult* result)
{
    json_destroy(result->value);
    common_free(result->errors);
}

JsonValue* json_objectget(JsonValue* object, const char* key) 
{
    if (object->type != JSON_OBJECT)
        return NULL;
    void* e = map_get(&object->value.object, key);
    if (e == NULL)
        return NULL;
    return *(JsonValue**)e;
}

JsonValue* json_arrayget(JsonValue* array, size_t index)
{
    if (array->type != JSON_ARRAY) 
        return NULL;
    return array->value.array.data[index];
}

size_t json_arraylen(JsonValue* array)
{
    if (array->type != JSON_ARRAY)
        return 0;
    return array->value.array.length;
}

const char* json_typename(JsonType type)
{
    switch (type) {
        case JSON_ARRAY: return "array";
        case JSON_BOOL: return "bool";
        case JSON_NULL: return "null";
        case JSON_NUMBER: return "number";
        case JSON_STRING: return "string";
    }
    return "unknown";
}

bool json_isnull(JsonValue* value)
{
    return value->type == JSON_NULL;
}