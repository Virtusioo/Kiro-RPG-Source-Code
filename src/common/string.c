
#include "string.h"
#include "common.h"

#include <stdio.h>
#include <stdarg.h>

String* str_new()
{
    String* str = common_malloc(sizeof(String) * COMMON_INIT_SIZE);
    str->data = common_malloc(sizeof(char) * COMMON_INIT_SIZE);
    str->data[0] = '\0';
    str->length = 0;
    str->slots = COMMON_INIT_SIZE;
    return str;
}

void str_destroy(String* str)
{
    common_free(str->data);
    common_free(str);
}

void str_write(String* str, char c)
{
    if (str->length + 1 >= str->slots) {
        str->slots *= 2;
        str->data = common_realloc(str->data, str->slots);
    }
    str->data[str->length++] = c;
    str->data[str->length] = '\0';
}

void str_append(String* str, const char* string)
{
    while (*string) {
        str_write(str, *(string++));
    }
}

String* str_newstring(const char* string)
{
    String* str = str_new();
    str_append(str, string);
    return str;
}

String* str_frombuf(const char* buf, size_t len)
{
    String* str = str_new();
    while (len--) {
        str_write(str, *(buf++));
    }
    return str;
}

void str_appendf(String* str, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    va_list args_copy;
    va_copy(args_copy, args);

    int required = vsnprintf(NULL, 0, fmt, args);
    va_end(args);

    if (required < 0) {
        va_end(args_copy);
        return;
    }

    while (str->length + required + 1 > str->slots) {
        str->slots = (str->length + required + 1) * 2;
        str->data = common_realloc(str->data, str->slots);
    }

    vsnprintf(str->data + str->length, str->slots - str->length, fmt, args_copy);
    va_end(args_copy);

    str->length += required;
    str->data[str->length] = '\0';
}
