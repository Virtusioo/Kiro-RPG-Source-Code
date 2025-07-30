
#include "tiled.h"
#include "json/json.h"
#include "common/file.h"
#include "common/vector.h"
#include "common/common.h"
#include "core/debug.h"

#include <string.h>

static const char* curr_tmj;
static TiledData* curr_data;
static JsonValue* root_value;
static Vector layers_buf;

static JsonValue* check_objectget(JsonValue* value, const char* key, JsonType required)
{
    if (value->type != JSON_OBJECT)
        debug_error("Failed to parse '%s':\nValue is not of type object", curr_tmj);

    JsonValue* data = json_objectget(value, key);

    if (data == NULL) 
        debug_error("Failed to parse '%s':\nMissing required field 'layers'", curr_tmj);

    if (data->type != required)
        debug_error("Failed to parse '%s':\nValue of field 'layers' is not of type %s", curr_tmj, json_typename(required));

    return data;
}

static void push_tilelayer(JsonValue* layer)
{
    Vector tileidx_buf = vec_new(sizeof(int));
    JsonValue* tile_data = check_objectget(layer, "data", JSON_ARRAY);
    JsonValue* tile_visible = check_objectget(layer, "visible", JSON_BOOL);
    
    for (size_t i = 0; i < json_arraylen(tile_data); i++) {
        JsonValue* tiled_idx = json_arrayget(tile_data, i);

        if (tiled_idx->type != JSON_NUMBER)
            debug_error("Failed to parse '%s':\nValue of data item #%zu is not a number", curr_tmj, i);

        vec_push(&tileidx_buf, &(int){tiled_idx->value.number});
    }

    vec_push(&layers_buf, &(TiledLayer) {
        .type = TILED_TILELAYER,
        .tilelayer = {
            .data = (int*)tileidx_buf.data,
            .length = tileidx_buf.length,
            .visible = tile_visible->value.boolean
        }
    });
}

static void push_objectgroup(JsonValue* layer)
{
    Vector obj_buf = vec_new(sizeof(TiledObject));
    JsonValue* obj_data = check_objectget(layer, "objects", JSON_ARRAY);

    for (size_t i = 0; i < json_arraylen(obj_data); i++) {
        JsonValue* obj_info = json_arrayget(obj_data, i);

        vec_push(&obj_buf, &(TiledObject) {
            .width = check_objectget(obj_info, "width", JSON_NUMBER)->value.number,
            .height = check_objectget(obj_info, "height", JSON_NUMBER)->value.number,
            .x = check_objectget(obj_info, "x", JSON_NUMBER)->value.number,
            .y = check_objectget(obj_info, "y", JSON_NUMBER)->value.number,
            .visible = check_objectget(obj_info, "visible", JSON_BOOL)->value.boolean
        });
    }

    vec_push(&layers_buf, &(TiledLayer) {
        .type = TILED_OBJECTGROUP,
        .objectgroup = {
            .data = (TiledObject*)obj_buf.data,
            .length = obj_buf.length,
        }
    });
}

static void push_layer(JsonValue* layer)
{
    JsonValue* layer_type = check_objectget(layer, "type", JSON_STRING);
    const char* type = layer_type->value.string;

    if (!strcmp(type, "tiledlayer"))
        push_tilelayer(layer);
    else if (!strcmp(type, "objectgroup"))
        push_objectgroup(layer);
    else
        debug_error("Failed to parse '%s':\nUnsupported layer type '%s'", curr_tmj, type);
}

static void get_layers()
{
    JsonValue* layers = check_objectget(root_value, "layers", JSON_ARRAY);

    for (size_t i = 0; i < json_arraylen(layers); i++) {
        JsonValue* layer = json_arrayget(layers, i);

        if (layer->type != JSON_OBJECT) 
            debug_error("Failed to parse '%s':\nLayer #%zu is not of type array", curr_tmj);

        push_layer(layer);
    }

    curr_data->layers.data = (TiledLayer*)layers_buf.data;
    curr_data->layers.length = layers_buf.length;
}

static void get_tilesets()
{
    JsonValue* tilesets = check_objectget(root_value, "tilesets", JSON_ARRAY);
    Vector tilesets_buf = vec_new(sizeof(TiledTileset));

    for (size_t i = 0; i < json_arraylen(tilesets); i++) {
        JsonValue* tileset = json_arrayget(tilesets, i);

        if (tileset->type != JSON_OBJECT) 
            debug_error("Failed to parse '%s':\nTileset #%zu is not of type array", curr_tmj);

        JsonValue* image_info = check_objectget(tileset, "image", JSON_STRING);

        vec_push(&tilesets_buf, &(TiledTileset) {
            .image = strdup(image_info->value.string),
            .tilewidth = check_objectget(tileset, "tilewidth", JSON_NUMBER)->value.number,
            .tileheight = check_objectget(tileset, "tileheight", JSON_NUMBER)->value.number
        });
    }

    curr_data->tilesets.data = (TiledTileset*)tilesets_buf.data;
    curr_data->tilesets.length = tilesets_buf.length;
}

TiledData tiled_parse(const char* tmj_path)
{
    curr_tmj = tmj_path;
    layers_buf = vec_new(sizeof(TiledLayer));

    char* map_data = file_read(tmj_path);
    JsonResult result = json_parse(map_data);
    TiledData data;

    /* reduce parameter slop */
    curr_data = &data;
    root_value = result.value;

    if (result.errors != NULL) 
        debug_error("Failed to parse '%s':\n%s", tmj_path, result.errors);
    else 
        get_layers();

    get_tilesets();

    json_destroyresult(&result);
    return data;
}

void tiled_destroydata(TiledData* data)
{
    for (size_t i = 0; i < data->layers.length; i++) {
        TiledLayer layer = data->layers.data[i];
        switch (layer.type) {
            case TILED_TILELAYER:
                common_free(layer.tilelayer.data);
                break;
            case TILED_OBJECTGROUP:
                common_free(layer.objectgroup.data);
                break;
        }
    }

    for (size_t i = 0; i < data->tilesets.length; i++) {
        TiledTileset tileset = data->tilesets.data[i];
        common_free(tileset.image);
    }
    common_free(data->tilesets.data);
}