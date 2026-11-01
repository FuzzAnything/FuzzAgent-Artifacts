// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_malloc at cJSON.c:3197:22 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
// cJSON_ParseWithOpts at cJSON.c:1131:23 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
// cJSON_ParseWithOpts at cJSON.c:1131:23 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
// cJSON_ParseWithLength at cJSON.c:1232:23 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
// cJSON_ParseWithLengthOpts at cJSON.c:1147:23 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
// cJSON_ParseWithLengthOpts at cJSON.c:1147:23 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
// cJSON_malloc at cJSON.c:3197:22 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_malloc at cJSON.c:3197:22 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <cJSON.h>

static void cleanup_cjson(cJSON *item) {
    if (item != NULL) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure the input is null-terminated for string-based functions
    char *input_str = (char *)cJSON_malloc(Size + 1);
    if (input_str == NULL) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    const char *parse_end = NULL;
    cJSON *json_obj = NULL;

    // Test 1: cJSON_Parse
    json_obj = cJSON_Parse(input_str);
    if (json_obj == NULL) {
        cJSON_GetErrorPtr(); // Trigger error pointer retrieval on failure
    }
    cleanup_cjson(json_obj);
    json_obj = NULL;

    // Test 2: cJSON_ParseWithOpts (require_null_terminated = 0)
    json_obj = cJSON_ParseWithOpts(input_str, &parse_end, 0);
    if (json_obj == NULL) {
        cJSON_GetErrorPtr();
    }
    cleanup_cjson(json_obj);
    json_obj = NULL;

    // Test 3: cJSON_ParseWithOpts (require_null_terminated = 1)
    json_obj = cJSON_ParseWithOpts(input_str, &parse_end, 1);
    if (json_obj == NULL) {
        cJSON_GetErrorPtr();
    }
    cleanup_cjson(json_obj);
    json_obj = NULL;

    // Test 4: cJSON_ParseWithLength
    json_obj = cJSON_ParseWithLength(input_str, Size);
    if (json_obj == NULL) {
        cJSON_GetErrorPtr();
    }
    cleanup_cjson(json_obj);
    json_obj = NULL;

    // Test 5: cJSON_ParseWithLengthOpts (require_null_terminated = 0)
    json_obj = cJSON_ParseWithLengthOpts(input_str, Size, &parse_end, 0);
    if (json_obj == NULL) {
        cJSON_GetErrorPtr();
    }
    cleanup_cjson(json_obj);
    json_obj = NULL;

    // Test 6: cJSON_ParseWithLengthOpts (require_null_terminated = 1)
    json_obj = cJSON_ParseWithLengthOpts(input_str, Size, &parse_end, 1);
    if (json_obj == NULL) {
        cJSON_GetErrorPtr();
    }
    cleanup_cjson(json_obj);
    json_obj = NULL;

    // Test 7: cJSON_malloc edge case (size 0)
    void *ptr = cJSON_malloc(0);
    if (ptr != NULL) {
        cJSON_free(ptr);
    }

    // Test 8: cJSON_malloc edge case (large size, may fail)
    ptr = cJSON_malloc(Size + 1024);
    if (ptr != NULL) {
        cJSON_free(ptr);
    }

    // Cleanup allocated input string
    cJSON_free(input_str);

    return 0;
}