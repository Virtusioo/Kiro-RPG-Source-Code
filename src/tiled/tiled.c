
#include "tiled.h"
#include "json/json.h"
#include "common/file.h"

TiledData tiled_parse(const char* tmj_path)
{
    char* map_data = file_read(tmj_path);
}