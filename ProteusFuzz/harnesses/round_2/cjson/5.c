// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_ParseWithOpts at cJSON.c:1131:23 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_PrintUnformatted at cJSON.c:1312:22 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
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

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure the input is null-terminated for parsing
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    const char *parse_end = NULL;
    cJSON_bool require_null_terminated = 0;

    // 1. Parse the input JSON string
    cJSON *item = cJSON_ParseWithOpts(input_str, &parse_end, require_null_terminated);
    
    // Clean up the input string buffer
    free(input_str);

    if (!item) {
        // Parsing failed, nothing to print or delete
        return 0;
    }

    // 2. Serialize using cJSON_PrintBuffered
    char *buffered_str = cJSON_PrintBuffered(item, 256, 1);
    if (buffered_str) {
        free(buffered_str);
    }

    // 3. Serialize using cJSON_Print
    char *formatted_str = cJSON_Print(item);
    if (formatted_str) {
        free(formatted_str);
    }

    // 4. Serialize using cJSON_PrintUnformatted
    char *unformatted_str = cJSON_PrintUnformatted(item);
    if (unformatted_str) {
        free(unformatted_str);
    }

    // 5. Minify a copy of the formatted string (if available)
    // We need a fresh copy because cJSON_Minify modifies in-place
    // and we want to test it on a valid JSON string derived from our item.
    // Re-generate formatted string to minify it.
    char *minify_input = cJSON_Print(item);
    if (minify_input) {
        cJSON_Minify(minify_input);
        free(minify_input);
    }

    // 6. Delete the cJSON object to free memory
    cJSON_Delete(item);

    return 0;
}