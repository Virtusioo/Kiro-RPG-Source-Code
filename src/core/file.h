
#ifndef file_h
#define file_h

#include <stdbool.h>

void file_mount(const char* folder);
char* file_read(const char* path);

#endif