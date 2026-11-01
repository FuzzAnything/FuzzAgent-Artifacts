// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Version at cJSON.c:124:27 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
// cJSON_ParseWithLength at cJSON.c:1232:23 in cJSON.h
// cJSON_ParseWithOpts at cJSON.c:1131:23 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
// cJSON_ParseWithLengthOpts at cJSON.c:1147:23 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
// cJSON_ParseWithLengthOpts at cJSON.c:1147:23 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
// cJSON_ParseWithOpts at cJSON.c:1131:23 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <cJSON.h>

static void cleanup_json(cJSON *item) {
    if (item != NULL) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Data == NULL || Size == 0) {
        return 0;
    }

    // Test cJSON_Version
    const char *version = cJSON_Version();
    (void)version; // Use version to avoid unused variable warning

    // Create a null-terminated copy for functions that expect C-strings
    // This prevents heap-buffer-overflow when cJSON_Parse calls strlen on non-null-terminated data
    char *null_terminated_data = (char *)malloc(Size + 1);
    if (null_terminated_data == NULL) {
        return 0;
    }
    memcpy(null_terminated_data, Data, Size);
    null_terminated_data[Size] = '\0';

    // Test cJSON_Parse (expects null-terminated string)
    cJSON *item1 = cJSON_Parse(null_terminated_data);
    cleanup_json(item1);
    if (item1 == NULL) {
        cJSON_GetErrorPtr();
    }

    // Test cJSON_ParseWithLength (handles non-null-terminated data safely)
    cJSON *item2 = cJSON_ParseWithLength((const char *)Data, Size);
    cleanup_json(item2);

    // Test cJSON_ParseWithOpts (expects null-terminated string)
    const char *parse_end = NULL;
    cJSON_bool require_null_terminated = 0;
    cJSON *item3 = cJSON_ParseWithOpts(null_terminated_data, &parse_end, require_null_terminated);
    cleanup_json(item3);
    if (item3 == NULL) {
        cJSON_GetErrorPtr();
    }

    // Test cJSON_ParseWithLengthOpts with require_null_terminated = 0
    const char *parse_end2 = NULL;
    cJSON_bool require_null_terminated2 = 0;
    cJSON *item4 = cJSON_ParseWithLengthOpts((const char *)Data, Size, &parse_end2, require_null_terminated2);
    cleanup_json(item4);
    if (item4 == NULL) {
        cJSON_GetErrorPtr();
    }

    // Test cJSON_ParseWithLengthOpts with require_null_terminated = 1
    const char *parse_end3 = NULL;
    cJSON_bool require_null_terminated3 = 1;
    cJSON *item5 = cJSON_ParseWithLengthOpts((const char *)Data, Size, &parse_end3, require_null_terminated3);
    cleanup_json(item5);
    if (item5 == NULL) {
        cJSON_GetErrorPtr();
    }

    // Test cJSON_ParseWithOpts with require_null_terminated = 1
    const char *parse_end4 = NULL;
    cJSON_bool require_null_terminated4 = 1;
    cJSON *item6 = cJSON_ParseWithOpts(null_terminated_data, &parse_end4, require_null_terminated4);
    cleanup_json(item6);
    if (item6 == NULL) {
        cJSON_GetErrorPtr();
    }

    free(null_terminated_data);

    return 0;
}