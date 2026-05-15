// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_malloc at cJSON.c:3197:22 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_ParseWithLength at cJSON.c:1232:23 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateRaw at cJSON.c:2581:23 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test cJSON_malloc and cJSON_free
    size_t alloc_size = Size % 1024; // Limit allocation size to avoid OOM
    void *mem = cJSON_malloc(alloc_size);
    if (mem) {
        cJSON_free(mem);
    }

    // Test cJSON_ParseWithLength
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    cJSON *parsed = cJSON_ParseWithLength(input_str, Size);
    if (parsed) {
        // Test cJSON_Print
        char *printed = cJSON_Print(parsed);
        if (printed) {
            cJSON_free(printed);
        }
        cJSON_Delete(parsed);
    }

    // Test cJSON_Parse
    cJSON *parsed2 = cJSON_Parse(input_str);
    if (parsed2) {
        char *printed2 = cJSON_Print(parsed2);
        if (printed2) {
            cJSON_free(printed2);
        }
        cJSON_Delete(parsed2);
    }

    // Test cJSON_CreateRaw
    cJSON *raw_json = cJSON_CreateRaw(input_str);
    if (raw_json) {
        char *printed_raw = cJSON_Print(raw_json);
        if (printed_raw) {
            cJSON_free(printed_raw);
        }
        cJSON_Delete(raw_json);
    }

    free(input_str);
    return 0;
}