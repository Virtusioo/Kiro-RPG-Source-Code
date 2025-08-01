
#ifndef file_h
#define file_h

#include <stdbool.h>

#define FILE_MAX_PATH 512

void file_mount(const char* folder);
char* file_read(const char* path);
char* file_resolve(const char* path);

#endif