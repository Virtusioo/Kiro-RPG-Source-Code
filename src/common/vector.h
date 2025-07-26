
#ifndef cvector_h
#define cvector_h

#include <stddef.h>

typedef struct 
{
    char* data;
    size_t length;
    size_t slots;
    size_t item_size;
} Vector;

Vector* vec_new(size_t item_size);
void vec_destroy(Vector* vec);

void vec_push(Vector* vec, const void* item);
void vec_pop(Vector* vec);
void* vec_get(Vector* vec, size_t index);
void vec_set(Vector* vec, size_t index, const void* item);

#endif