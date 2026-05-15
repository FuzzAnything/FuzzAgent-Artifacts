// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_ParseWithLength at cJSON.c:1232:23 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateRaw at cJSON.c:2581:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_malloc at cJSON.c:3197:22 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_malloc at cJSON.c:3197:22 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_malloc at cJSON.c:3197:22 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test cJSON_ParseWithLength
    cJSON *parsed_with_length = cJSON_ParseWithLength((const char *)Data, Size);
    if (parsed_with_length != NULL) {
        // Test cJSON_Print on the parsed object
        char *printed = cJSON_Print(parsed_with_length);
        if (printed != NULL) {
            cJSON_free(printed);
        }
        cJSON_Delete(parsed_with_length);
    }

    // Test cJSON_Parse (requires null-terminated string)
    char *null_terminated = (char *)malloc(Size + 1);
    if (null_terminated != NULL) {
        memcpy(null_terminated, Data, Size);
        null_terminated[Size] = '\0';
        
        cJSON *parsed = cJSON_Parse(null_terminated);
        if (parsed != NULL) {
            cJSON_Delete(parsed);
        }
        
        // Test cJSON_CreateRaw
        cJSON *raw = cJSON_CreateRaw(null_terminated);
        if (raw != NULL) {
            cJSON_Delete(raw);
        }
        
        free(null_terminated);
    }

    // Test cJSON_malloc with various sizes
    void *allocated_mem = cJSON_malloc(Size);
    if (allocated_mem != NULL) {
        // Test cJSON_free
        cJSON_free(allocated_mem);
    }

    // Test cJSON_malloc with small size
    void *small_alloc = cJSON_malloc(1);
    if (small_alloc != NULL) {
        cJSON_free(small_alloc);
    }

    // Test cJSON_malloc with zero size
    void *zero_alloc = cJSON_malloc(0);
    if (zero_alloc != NULL) {
        cJSON_free(zero_alloc);
    }

    return 0;
}