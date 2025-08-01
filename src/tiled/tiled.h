
#ifndef rtiled_h
#define rtiled_h

#include "common/map.h"

typedef enum
{
    TILED_OBJECTGROUP,
    TILED_TILELAYER
} TiledLayerType;

typedef struct
{
    bool visible;
    float x;
    float y;
    float width;
    float height;
} TiledObject;

typedef struct 
{
    TiledObject* data;
    size_t length;
} TiledObjectGroup;

typedef struct 
{
    bool visible;
    size_t* data;
    size_t length;
    int width;
} TiledTilelayer;

typedef struct 
{
    TiledLayerType type;
    union 
    {
        TiledObjectGroup objectgroup;
        TiledTilelayer tilelayer;
    };
} TiledLayer;

typedef struct 
{
    char* image;
    int columns;
    int firstgid;
    int tilecount;
} TiledTileset;

typedef struct 
{
    TiledLayer* data;
    size_t length;
} TiledLayers;

typedef struct 
{
    TiledTileset* data;
    size_t length;
} TiledTilesets;

typedef struct
{
    TiledLayers layers;
    TiledTilesets tilesets;
} TiledData;

TiledData tiled_parse(const char* tmj_path);
void tiled_destroylayers(TiledLayers* layers);
void tiled_destroytilesets(TiledTilesets* tilesets);
void tiled_destroydata(TiledData* data);

#endif