
#include "common.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

void common_panic(const char* message, ...)
{
    va_list vargs;
    va_start(vargs, message);
    vfprintf(stderr, message, vargs);
    fprintf(stderr, "\n");
    va_end(vargs);
    exit(1);
}

void* common_malloc(size_t size)
{
    void* mem = malloc(size);
    if (mem == NULL)
        common_panic("common_malloc(%d) failed: %s", size, strerror(errno));
    return mem;
}

void* common_realloc(void* mem, size_t size)
{
    void* new_mem = realloc(mem, size);
    if (new_mem == NULL)
        common_panic("common_realloc(%d) failed: %s", size, strerror(errno));
    return new_mem;
}

void common_free(void* mem)
{
    free(mem);
}

size_t common_hash(const char* str)
{
    size_t hash = 5381;
    size_t c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + (unsigned char)c;
    return hash;
}

void common_log(const char* message, ...)
{
    va_list vargs;
    va_start(vargs, message);
    vfprintf(stdout, message, vargs);
    fputs("\n", stdout);
    va_end(vargs);
}