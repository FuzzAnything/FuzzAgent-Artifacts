// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ParseWithOpts at cJSON.c:1131:23 in cJSON.h
// cJSON_PrintUnformatted at cJSON.c:1312:22 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void safe_cleanup(char *ptr, cJSON *item) {
    if (ptr) {
        free(ptr);
    }
    if (item) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure null termination for parsing
    char *json_str = (char *)malloc(Size + 1);
    if (!json_str) {
        return 0;
    }
    memcpy(json_str, Data, Size);
    json_str[Size] = '\0';

    const char *return_parse_end = NULL;
    cJSON_bool require_null_terminated = 1;

    // Step 1: Parse JSON
    cJSON *item = cJSON_ParseWithOpts(json_str, &return_parse_end, require_null_terminated);
    free(json_str);

    if (!item) {
        return 0;
    }

    char *out_unformatted = NULL;
    char *out_buffered = NULL;
    char *out_formatted = NULL;
    char *out_minified = NULL;

    // Step 2: Print Unformatted
    out_unformatted = cJSON_PrintUnformatted(item);

    // Step 3: Print Buffered (guess size 1024, formatted=1)
    out_buffered = cJSON_PrintBuffered(item, 1024, 1);

    // Step 4: Print Formatted
    out_formatted = cJSON_Print(item);

    // Step 5: Minify (only if we have a formatted string to minify)
    if (out_formatted) {
        out_minified = strdup(out_formatted);
        if (out_minified) {
            cJSON_Minify(out_minified);
        }
    }

    // Cleanup all allocated resources
    safe_cleanup(out_unformatted, NULL);
    safe_cleanup(out_buffered, NULL);
    safe_cleanup(out_formatted, NULL);
    safe_cleanup(out_minified, NULL);
    safe_cleanup(NULL, item);

    return 0;
}