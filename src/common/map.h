
#ifndef cmap_h
#define cmap_h

#include <stdbool.h>
#include <stddef.h>

typedef struct 
{
    char* key;
    void* value;
    bool occupied;
} MapEntry;

typedef struct 
{
    MapEntry* entries;
    size_t length;
    size_t slots;
    size_t item_size;
} Map;

Map* map_new(size_t item_size);
void map_destroy(Map* map);

void map_set(Map* map, char* key, void* value);
void* map_get(Map* map, char* key);

#endif