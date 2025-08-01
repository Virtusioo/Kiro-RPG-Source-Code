
#include "file.h"
#include "core/debug.h"
#include "common/common.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>

const char* res_folder = "";

void file_mount(const char* _folder)
{
    res_folder = _folder;
}

char* file_resolve(const char* path)
{
    char* newpath = common_malloc(FILE_MAX_PATH);
    snprintf(newpath, FILE_MAX_PATH, "%s/%s", res_folder, path);
    return newpath;
}

char* file_read(const char* path)
{
    char fullpath[FILE_MAX_PATH];
    snprintf(fullpath, FILE_MAX_PATH, "%s/%s", res_folder, path);

    FILE* file = fopen(fullpath, "rb");
    if (file == NULL)
        debug_error("file_read(\"%s\") failed:\n%s", fullpath, strerror(errno));

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    rewind(file);

    char* buffer = common_malloc(size + 1); 

    size_t read = fread(buffer, 1, size, file);
    if (read != size) {
        fclose(file);
        debug_error("fread() failed:\nexpected %zu bytes, got %zu", size, read);
    }

    buffer[size] = '\0';
    fclose(file);
    return buffer;
}

