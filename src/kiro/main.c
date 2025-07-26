
#include "parsers/json/json.h"
#include "common/common.h"

int main()
{
    JsonResult json = json_parse("{\"key\":{\"haha\": 69e}}");

    if (json.errors != NULL) {
        common_log(json.errors);
    }

    json_destroyresult(&json);
}