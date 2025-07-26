
#include "json/json.h"

#include <stdio.h>

int main()
{
    JsonResult result = json_parse("{\"key\": [1, 2, 3, 4]}");

    if (result.errors == NULL) {
        JsonValue* value = result.value;
        JsonValue* test = *(JsonValue**)map_get(value->value.object, "key");

        printf("%f\n", test->value.array.data[1]->value.number);
    } else {
        printf("%s", result.errors);
    }

    json_destroyresult(&result);
}