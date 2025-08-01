
#include "rpg.h"
#include "tiled/tiled.h"
#include "common/common.h"
#include "common/vector.h"
#include "common/file.h"

#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL.h>

struct RPGMap
{
    SDL_Texture** tilesets;
    size_t tilesets_len;
    TiledData data;
};

static RPGMap* rpg_maps = NULL;
static size_t rpg_map_slots = 0;
static size_t rpg_map_length = 0;

extern SDL_Renderer* renderer;

void rpg_init()
{
    rpg_maps = common_malloc(sizeof(RPGMap) * COMMON_INIT_SIZE);
    rpg_map_slots = COMMON_INIT_SIZE;
    rpg_map_length = 0;
}

void rpg_quit()
{
    for (size_t i = 0; i < rpg_map_length; i++) {
        for (size_t j = 0; j < rpg_maps[i].tilesets_len; j++) {
            SDL_DestroyTexture(rpg_maps->tilesets[i]);
        }
        common_free(rpg_maps->tilesets);
        tiled_destroydata(&rpg_maps[i].data);
    }
    common_free(rpg_maps);
}

static void rpg_pushmap(RPGMap map)
{
    if (rpg_map_length >= rpg_map_slots) {
        rpg_map_slots *= 2;
        rpg_maps = common_realloc(rpg_maps, rpg_map_slots);
    }
    rpg_maps[rpg_map_length++] = map;
}

RPGMap* rpg_newmap(const char* tmj_path)
{
    TiledData data = tiled_parse(tmj_path);
    Vector texture_buf = vec_new(sizeof(SDL_Texture*));

    for (size_t i = 0; i < data.tilesets.length; i++) {
        TiledTileset tileset = data.tilesets.data[i];

        char* newpath = file_resolve(tileset.image);
        common_free(tileset.image);
        SDL_Texture* image = IMG_LoadTexture(renderer, newpath);
        vec_push(&texture_buf, &image);
    }

    rpg_pushmap((RPGMap) {
        .data = data,
        .tilesets = (SDL_Texture**)texture_buf.data,
        .tilesets_len = texture_buf.length
    });

    return &rpg_maps[rpg_map_length - 1];
}

void rpg_rendermap(RPGMap* map)
{
for (size_t i = 0; i < map->data.layers.length; i++) {
    TiledLayer layer = map->data.layers.data[i];

    if (layer.type != TILED_TILELAYER || !layer.tilelayer.visible)
        continue;

    for (size_t j = 0; j < layer.tilelayer.length; j++) {
        size_t tile_index = layer.tilelayer.data[j];
        if (tile_index == 0)
            continue;

        // Find correct tileset
        SDL_Texture* texture = NULL;
        TiledTileset* ts = NULL;

        for (size_t t = 0; t < map->data.tilesets.length; t++) {
            TiledTileset* tileset = &map->data.tilesets.data[t];
            if (tile_index >= tileset->firstgid &&
                tile_index < tileset->firstgid + tileset->tilecount) {
                texture = map->tilesets[t];
                ts = tileset;
                break;
            }
        }

        if (!texture || !ts) continue;

        int localid = tile_index - ts->firstgid;
        int columns = ts->columns;

        SDL_FRect to_cut = {
            .x = (localid % columns) * RPG_DIM,
            .y = (localid / columns) * RPG_DIM,
            .w = RPG_DIM,
            .h = RPG_DIM
        };

        size_t x = j % layer.tilelayer.width;
        size_t y = j / layer.tilelayer.width;

        SDL_FRect dst = {
            .x = x * RPG_DIM,
            .y = y * RPG_DIM,
            .w = RPG_DIM,
            .h = RPG_DIM
        };

        SDL_RenderTexture(renderer, texture, &to_cut, &dst);
    }
}

}