
#include "vector.h"
#include "common.h"

#include <string.h>

Vector vec_new(size_t item_size)
{
    return (Vector) {
        .item_size = item_size,
        .data = common_malloc(item_size * COMMON_INIT_SIZE),
        .length = 0,
        .slots = COMMON_INIT_SIZE
    };
}

void vec_destroy(Vector* vec)
{
    common_free(vec->data);
}

void vec_push(Vector* vec, const void* item)
{
    if (vec->length >= vec->slots) {
        vec->slots *= 2;
        vec->data = common_realloc(vec->data, vec->slots * vec->item_size);
    }
    memcpy(vec->data + vec->item_size * vec->length++, item, vec->item_size);
}

void vec_pop(Vector* vec)
{
    vec->length -= 1;
}

void* vec_get(Vector* vec, size_t index)
{
    return vec->data + vec->item_size * index;
}

void vec_set(Vector* vec, size_t index, const void* item)
{
    memcpy(vec->data + vec->item_size * index, item, vec->item_size);
}