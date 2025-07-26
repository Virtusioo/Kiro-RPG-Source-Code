
#ifndef json_h
#define json_h

#include "common/map.h"

typedef enum 
{
    JSON_NULL,
    JSON_BOOL,
    JSON_NUMBER,
    JSON_STRING,
    JSON_ARRAY,
    JSON_OBJECT
} JsonType;

typedef struct JsonValue JsonValue;

typedef union 
{
    bool boolean;
    float number;
    char* string;
    Map* object;
    struct 
    {
        JsonValue** data;
        int length;
    } array;
} JsonUnion;

struct JsonValue 
{
    JsonType type;
    JsonUnion value;
};

typedef struct 
{
    char* errors;
    JsonValue* value;
} JsonResult;

/* do not use JsonValue* value, if errors != NULL*/
JsonResult json_parse(const char* source);
void json_destroy(JsonValue* root);
void json_destroyresult(JsonResult* result);

JsonValue* json_objectget(JsonValue* object, char* key);
JsonValue* json_arrayget(JsonValue* array, size_t index);
size_t json_arraylen(JsonValue* array);
const char* json_getstring(JsonValue* value);
float json_getnumber(JsonValue* value);
bool json_getboolean(JsonValue* value);
bool json_isnull(JsonValue* value);

#endif
