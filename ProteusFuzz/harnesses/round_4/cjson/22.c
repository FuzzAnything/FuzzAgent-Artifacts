// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ParseWithLength at cJSON.c:1232:23 in cJSON.h
// cJSON_ParseWithLengthOpts at cJSON.c:1147:23 in cJSON.h
// cJSON_malloc at cJSON.c:3197:22 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
// cJSON_ParseWithOpts at cJSON.c:1131:23 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_malloc at cJSON.c:3197:22 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void cleanup_cjson(cJSON *item) {
    if (item) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Cast input data to char pointer
    const char *json_str = (const char *)Data;

    // Test 1: cJSON_ParseWithLength
    cJSON *item1 = cJSON_ParseWithLength(json_str, Size);
    cleanup_cjson(item1);

    // Test 2: cJSON_ParseWithLengthOpts
    const char *parse_end1 = NULL;
    cJSON_bool require_null_terminated = (Size > 0 && (Data[Size - 1] == '\0')) ? 1 : 0;
    cJSON *item2 = cJSON_ParseWithLengthOpts(json_str, Size, &parse_end1, require_null_terminated);
    cleanup_cjson(item2);

    // Test 3: cJSON_Parse (requires null-terminated string)
    // Create a null-terminated buffer if the input is not already null-terminated
    char *null_terminated_str = NULL;
    if (Size > 0 && Data[Size - 1] != '\0') {
        null_terminated_str = (char *)cJSON_malloc(Size + 1);
        if (null_terminated_str) {
            memcpy(null_terminated_str, Data, Size);
            null_terminated_str[Size] = '\0';
        }
    } else {
        null_terminated_str = (char *)Data;
    }

    if (null_terminated_str) {
        cJSON *item3 = cJSON_Parse(null_terminated_str);
        cleanup_cjson(item3);

        // If parsing failed, check error pointer
        if (item3 == NULL) {
            const char *error_ptr = cJSON_GetErrorPtr();
            (void)error_ptr; // Suppress unused variable warning
        }

        // Test 4: cJSON_ParseWithOpts
        const char *parse_end2 = NULL;
        cJSON_bool require_null_terminated_opts = (Size > 0 && Data[Size - 1] == '\0') ? 1 : 0;
        cJSON *item4 = cJSON_ParseWithOpts(null_terminated_str, &parse_end2, require_null_terminated_opts);
        cleanup_cjson(item4);

        if (item4 == NULL) {
            const char *error_ptr2 = cJSON_GetErrorPtr();
            (void)error_ptr2; // Suppress unused variable warning
        }

        // Free allocated buffer if we created one
        if (Size > 0 && Data[Size - 1] != '\0') {
            cJSON_free(null_terminated_str);
        }
    }

    // Test 5: cJSON_malloc (basic allocation test)
    void *malloc_result = cJSON_malloc(Size > 0 ? Size : 1);
    if (malloc_result) {
        // Write some data to ensure memory is valid
        memset(malloc_result, 0, Size > 0 ? Size : 1);
        cJSON_free(malloc_result);
    }

    return 0;
}