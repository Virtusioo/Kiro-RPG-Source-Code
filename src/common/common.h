
#ifndef ccommon_h
#define ccommon_h

#include <stddef.h>
#include <stdbool.h>

#define COMMON_INIT_SIZE 8

void common_panic(const char* message, ...);
void* common_malloc(size_t size);
void* common_realloc(void* mem, size_t size);
void common_free(void* mem);
size_t common_hash(const char* str);
void common_log(const char* messsage, ...);

#endif