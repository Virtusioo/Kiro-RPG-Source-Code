
#include "file.h"
#include "common.h"

#include <stdio.h>
#include <string.h>

char* file_read_all(const char* path)
{
    FILE* file = fopen(path, "r");
    if (file == NULL)
        common_panic("file_read_all(\"%s\") failed: %s", strerror(errno));

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    rewind(file);

    char* buffer = common_malloc(size + 1); 

    size_t read = fread(buffer, 1, size, file);
    buffer[read] = '\0';

    fclose(file);
    return buffer;
}