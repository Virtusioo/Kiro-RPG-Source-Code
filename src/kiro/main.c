
#include "json/json.h"

#include <stdio.h>

int main()
{
    JsonResult result = json_parse("{\"key\": [1, 2, 3, 4]}");

    if (result.errors == NULL) {
        JsonValue* value = result.value;
        JsonValue* array = json_objectget(value, "key");

        for (size_t i = 0; i < json_arraylen(array); i++) {
            JsonValue* json_num = json_arrayget(array, i);
            float number = json_getnumber(json_num);
            printf("%f\n", number);
        }
    } else {
        printf("%s", result.errors);
    }

    json_destroyresult(&result);
}