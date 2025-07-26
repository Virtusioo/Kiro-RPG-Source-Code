
#include "sstream.h"
#include "common/common.h"

#include <stdio.h>
#include <stdarg.h>

StringStream* sstream_new()
{
    StringStream* stream = common_malloc(sizeof(StringStream));
    stream->data = common_malloc(sizeof(char) * COMMON_INIT_SIZE);
    stream->data[0] = '\0';
    stream->length = 0;
    stream->slots = COMMON_INIT_SIZE;
    return stream;
}

void sstream_destroy(StringStream* stream)
{
    common_free(stream->data);
    common_free(stream);
}

void sstream_appendf(StringStream* stream, const char* fmt, ...) {
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

    while (stream->length + required + 1 > stream->slots) {
        stream->slots = (stream->length + required + 1) * 2;
        stream->data = common_realloc(stream->data, stream->slots);
    }

    vsnprintf(stream->data + stream->length, stream->slots - stream->length, fmt, args_copy);
    va_end(args_copy);

    stream->length += required;
    stream->data[stream->length] = '\0'; // Ensure null-termination
}
