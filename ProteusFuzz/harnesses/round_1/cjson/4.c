// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_ParseWithOpts at cJSON.c:1131:23 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
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

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure the input is null-terminated for parsing
    char *json_str = (char *)malloc(Size + 1);
    if (!json_str) {
        return 0;
    }
    memcpy(json_str, Data, Size);
    json_str[Size] = '\0';

    const char *parse_end = NULL;
    cJSON_bool require_null_terminated = 1;

    // 1. Parse the JSON string
    cJSON *item = cJSON_ParseWithOpts(json_str, &parse_end, require_null_terminated);
    
    // Free the raw input string as it's no longer needed for parsing
    free(json_str);

    if (item == NULL) {
        // Parsing failed, nothing to print or delete
        return 0;
    }

    // 2. Print with buffer (formatted)
    char *buffered_str = cJSON_PrintBuffered(item, 256, 1);
    if (buffered_str) {
        free(buffered_str);
    }

    // 3. Print formatted
    char *print_str = cJSON_Print(item);
    if (print_str) {
        // 4. Minify the printed string (must be writable, not a literal)
        cJSON_Minify(print_str);
        free(print_str);
    }

    // 5. Print unformatted
    char *unformatted_str = cJSON_PrintUnformatted(item);
    if (unformatted_str) {
        // Minify the unformatted string as well to exercise that code path
        cJSON_Minify(unformatted_str);
        free(unformatted_str);
    }

    // 6. Delete the JSON tree
    cJSON_Delete(item);

    return 0;
}