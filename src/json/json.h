
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
    char* errors; /* NULL if no errors */
    JsonValue* value; /* Unsafe if errors != NULL */
} JsonResult;

JsonResult json_parse(const char* source);
void json_destroy(JsonValue* root);
void json_destroy_result(JsonResult* result);

JsonValue* json_object_get(JsonValue* object, char* key);
JsonValue* json_array_get(JsonValue* array, size_t index);
size_t json_array_len(JsonValue* array);
const char* json_get_string(JsonValue* value);
float json_get_number(JsonValue* value);
bool json_get_boolean(JsonValue* value);
bool json_is_null(JsonValue* value);

#endif
