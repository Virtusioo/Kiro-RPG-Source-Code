
#include "file.h"
#include "common.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>

char* file_read_all(const char* path)
{
    FILE* file = fopen(path, "rb");
    if (file == NULL)
        common_panic("file_read_all(\"%s\") failed: %s", path, strerror(errno));

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    rewind(file);

    char* buffer = common_malloc(size + 1); 

    size_t read = fread(buffer, 1, size, file);
    if (read != size) {
        fclose(file);
        common_panic("fread() failed: expected %zu bytes, got %zu", size, read);
    }

    buffer[size] = '\0';
    fclose(file);
    return buffer;
}
