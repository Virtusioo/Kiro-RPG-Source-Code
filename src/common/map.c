
#include "map.h"
#include "common.h"

#include <string.h>

static void map_resize(Map* map);

Map map_new(size_t item_size)
{
    Map map;
    map.slots = COMMON_INIT_SIZE;
    map.length = 0;
    map.item_size = item_size;
    map.entries = common_malloc(map.slots * sizeof(MapEntry));
    memset(map.entries, 0, map.slots * sizeof(MapEntry));
    return map;
}

void map_destroy(Map* map)
{
    for (size_t i = 0; i < map->slots; i++) {
        if (map->entries[i].occupied) {
            common_free(map->entries[i].value);
        }
    }
    common_free(map->entries);
}

void map_set(Map* map, const char* key, void* item)
{
    if ((map->length + 1) * 2 >= map->slots) 
        map_resize(map);

    size_t hash = common_hash(key) % map->slots;

    while (map->entries[hash].occupied && strcmp(map->entries[hash].key, key) != 0) {
        hash = (hash + 1) % map->slots;
    }

    if (!map->entries[hash].occupied) {
        map->length++;
        map->entries[hash].key = key;
        map->entries[hash].value = common_malloc(map->item_size);
    }

    memcpy(map->entries[hash].value, item, map->item_size);
    map->entries[hash].occupied = true;
}

void* map_get(Map* map, const char* key)
{
    size_t hash = common_hash(key) % map->slots;

    while (map->entries[hash].occupied) {
        if (strcmp(map->entries[hash].key, key) == 0) {
            return map->entries[hash].value;
        }
        hash = (hash + 1) % map->slots;
    }

    return NULL;
}

static void map_resize(Map* map)
{
    size_t old_slots = map->slots;
    MapEntry* old_entries = map->entries;

    map->slots *= 2;
    map->entries = common_malloc(map->slots * sizeof(MapEntry));
    memset(map->entries, 0, map->slots * sizeof(MapEntry));
    map->length = 0; // will be recalculated on reinsertion

    for (size_t i = 0; i < old_slots; i++) {
        if (old_entries[i].occupied) {
            map_set(map, old_entries[i].key, old_entries[i].value);
            common_free(old_entries[i].value); // free old value after re-insertion
        }
    }

    common_free(old_entries);
}
