
#include "json/json.h"
#include "common/file.h"

#include <stdio.h>
#include <string.h>

int main()
{
    const char* path = "build/map.tmj";
    char* content = file_read(path);
    JsonResult result = json_parse(content);

    if (result.errors == NULL) {
        JsonValue* layers = json_objectget(result.value, "layers");

        for (size_t i = 0; i < json_arraylen(layers); i++) {
            
            JsonValue* layer = json_arrayget(layers, i);
            JsonValue* type = json_objectget(layer, "type");
            const char* layer_type = json_getstring(type);

            if (!strcmp(layer_type, "tilelayer"))  {
                JsonValue* data = json_objectget(layer, "data");
                printf("%zu\n", json_arraylen(data));
            }
        }
    } else {
        printf("In .json file '%s'\n%s", path, content);
    }
}