// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_malloc at cJSON.c:3197:22 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
// cJSON_ParseWithLength at cJSON.c:1232:23 in cJSON.h
// cJSON_ParseWithOpts at cJSON.c:1131:23 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
// cJSON_ParseWithLengthOpts at cJSON.c:1147:23 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
// cJSON_malloc at cJSON.c:3197:22 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
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

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure we have a null-terminated string for functions that require it
    // We allocate a buffer of Size + 1 to safely append a null terminator
    char *buffer = (char *)cJSON_malloc(Size + 1);
    if (!buffer) {
        return 0;
    }
    memcpy(buffer, Data, Size);
    buffer[Size] = '\0';

    // Test 1: cJSON_Parse (requires null-terminated string)
    cJSON *item1 = cJSON_Parse(buffer);
    cleanup_cjson(item1);
    if (!item1) {
        // Only call GetErrorPtr if parse failed
        const char *error_ptr = cJSON_GetErrorPtr();
        (void)error_ptr; // Use the pointer to avoid unused variable warnings
    }

    // Test 2: cJSON_ParseWithLength (does not require null-termination, but we have it)
    cJSON *item2 = cJSON_ParseWithLength(buffer, Size);
    cleanup_cjson(item2);

    // Test 3: cJSON_ParseWithOpts (requires null-terminated string)
    const char *return_parse_end = NULL;
    cJSON_bool require_null_terminated = (Size > 0 && Data[Size - 1] == 0) ? 1 : 0;
    // We force require_null_terminated to 0 sometimes to test non-strict mode, 
    // but since we manually added '\0', it should be safe.
    // To diversify, let's toggle the flag based on input data
    if (Size > 0) {
        require_null_terminated = (Data[0] % 2);
    }
    
    cJSON *item3 = cJSON_ParseWithOpts(buffer, &return_parse_end, require_null_terminated);
    cleanup_cjson(item3);
    if (!item3) {
        const char *error_ptr = cJSON_GetErrorPtr();
        (void)error_ptr;
    }

    // Test 4: cJSON_ParseWithLengthOpts
    const char *return_parse_end_len = NULL;
    cJSON_bool require_null_terminated_len = (Data[1 % Size] % 2);
    cJSON *item4 = cJSON_ParseWithLengthOpts(buffer, Size, &return_parse_end_len, require_null_terminated_len);
    cleanup_cjson(item4);
    if (!item4) {
        const char *error_ptr = cJSON_GetErrorPtr();
        (void)error_ptr;
    }

    // Test 5: cJSON_malloc (basic allocation test)
    void *mem = cJSON_malloc(Size);
    if (mem) {
        // Do something with memory to ensure it's allocated
        memset(mem, 0, Size);
        cJSON_free(mem);
    }

    // Cleanup buffer
    cJSON_free(buffer);

    return 0;
}