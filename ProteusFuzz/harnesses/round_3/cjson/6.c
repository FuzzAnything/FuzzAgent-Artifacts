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
#include <cJSON.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
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
    cJSON_bool require_null_terminated = 1; // Try strict mode

    // 1. Parse JSON
    cJSON *item = cJSON_ParseWithOpts(json_str, &return_parse_end, require_null_terminated);
    
    // Cleanup input string
    free(json_str);

    if (!item) {
        return 0;
    }

    // 2. Print with buffered strategy (formatted)
    char *buf_formatted = cJSON_PrintBuffered(item, 256, 1);
    if (buf_formatted) {
        free(buf_formatted);
    }

    // 3. Print formatted
    char *str_formatted = cJSON_Print(item);
    if (str_formatted) {
        free(str_formatted);
    }

    // 4. Print unformatted
    char *str_unformatted = cJSON_PrintUnformatted(item);
    if (str_unformatted) {
        // 5. Minify the unformatted string (requires writable buffer)
        cJSON_Minify(str_unformatted);
        free(str_unformatted);
    }

    // 6. Delete the JSON tree
    cJSON_Delete(item);

    return 0;
}