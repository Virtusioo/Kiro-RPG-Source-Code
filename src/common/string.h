
#define cstring_h
#define cstring_h

#include <stddef.h>

typedef struct
{
    char* data;
    size_t length;
    size_t slots;
} String;

String str_new();
void str_destroy(String* str);

void str_write(String* str, char c);
void str_append(String* str, const char* string);
String str_newstring(const char* string);
String str_frombuf(const char* buf, size_t len);
void str_appendf(String* str, const char* fmt, ...);