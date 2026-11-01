// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_AddTrueToObject at cJSON.c:2162:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    cJSON *obj = cJSON_CreateObject();
    if (obj == NULL) {
        return 0;
    }

    if (Size > 0) {
        // Ensure the input is treated as a null-terminated string by checking for a null terminator
        // within the buffer bounds. If no null terminator is found, truncate or skip.
        size_t str_len = 0;
        while (str_len < Size && Data[str_len] != '\0') {
            str_len++;
        }

        // Only proceed if a valid null-terminated string exists within the buffer
        if (str_len < Size) {
            cJSON_AddTrueToObject(obj, (const char *)Data);
        }
    }

    cJSON_Delete(obj);
    return 0;
}