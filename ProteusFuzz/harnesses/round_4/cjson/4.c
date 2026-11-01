// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_ParseWithOpts at cJSON.c:1131:23 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_PrintUnformatted at cJSON.c:1312:22 in cJSON.h
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure null-termination for parsing
    char *json_str = (char *)malloc(Size + 1);
    if (!json_str) {
        return 0;
    }
    memcpy(json_str, Data, Size);
    json_str[Size] = '\0';

    const char *return_parse_end = NULL;
    cJSON_bool require_null_terminated = (Size > 0 && Data[Size - 1] == '\0') ? 1 : 0;

    // 1. cJSON_ParseWithOpts
    cJSON *item = cJSON_ParseWithOpts(json_str, &return_parse_end, require_null_terminated);
    
    // Cleanup the input string as it is no longer needed for parsing
    free(json_str);

    if (item == NULL) {
        return 0;
    }

    // 2. cJSON_PrintBuffered
    char *buffered_str = cJSON_PrintBuffered(item, 256, 1);
    if (buffered_str) {
        free(buffered_str);
    }

    // 3. cJSON_Print
    char *print_str = cJSON_Print(item);
    if (print_str) {
        free(print_str);
    }

    // 4. cJSON_PrintUnformatted
    char *unformatted_str = cJSON_PrintUnformatted(item);
    if (unformatted_str) {
        // 5. cJSON_Minify
        // Minify requires a writable buffer. unformatted_str is allocated on heap, so it is writable.
        cJSON_Minify(unformatted_str);
        free(unformatted_str);
    }

    // 6. cJSON_Delete
    cJSON_Delete(item);

    return 0;
}