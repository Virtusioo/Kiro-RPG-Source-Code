
#ifndef csstream_h
#define csstream_h

#include <stddef.h>

typedef struct 
{
    char* data;
    size_t length;
    size_t slots;
} StringStream;

StringStream* sstream_new();
void sstream_destroy(StringStream* stream);

void sstream_appendf(StringStream* stream, const char* fmt, ...);

#endif