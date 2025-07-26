
#include "json/json.h"
#include "common/file.h"

#include <stdio.h>

int main()
{
    char* path = "build/map.tmj";
    char* content = file_read_all(path);
    JsonResult result = json_parse(content);

    if (result.errors == NULL) {
        JsonValue* value = result.value;
        JsonValue* layers = json_objectget(value, "layers");

        for (size_t i = 0; i < json_arraylen(layers); i++) {

            JsonValue* layer = json_arrayget(layers, i);
            JsonValue* name = json_objectget(layer, "name");
            const char* layer_name = json_getstring(name);

            printf("%s\n", layer_name);
        }

    } else {
        printf("In .json file '%s':\n%s", path, result.errors);
    }

    json_destroyresult(&result);
}