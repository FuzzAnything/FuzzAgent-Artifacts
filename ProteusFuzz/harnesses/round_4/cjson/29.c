// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_ParseWithOpts at cJSON.c:1131:23 in cJSON.h
// cJSON_ParseWithLength at cJSON.c:1232:23 in cJSON.h
// cJSON_ParseWithLengthOpts at cJSON.c:1147:23 in cJSON.h
// cJSON_Version at cJSON.c:124:27 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "cJSON.h"

static void cleanup_json(cJSON *item) {
    if (item) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure the input is treated as a valid C-string for functions requiring it
    // by allocating a buffer with space for a null terminator if needed.
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    // 1. Fuzz cJSON_Parse (Standard null-terminated parse)
    cJSON *item1 = cJSON_Parse(input_str);
    cleanup_json(item1);

    // 2. Fuzz cJSON_ParseWithOpts (With options)
    const char *parse_end1 = NULL;
    // Fixed: Check Size > 0 before accessing Data[0]
    cJSON_bool require_null = (Size > 0 && Data[0] % 2 == 0) ? cJSON_True : cJSON_False;
    cJSON *item2 = cJSON_ParseWithOpts(input_str, &parse_end1, require_null);
    cleanup_json(item2);

    // 3. Fuzz cJSON_ParseWithLength (Known length, no null terminator requirement)
    cJSON *item3 = cJSON_ParseWithLength(input_str, Size);
    cleanup_json(item3);

    // 4. Fuzz cJSON_ParseWithLengthOpts (Known length with options)
    const char *parse_end2 = NULL;
    // Fixed: Check Size > 1 before accessing Data[1]
    cJSON_bool require_null2 = (Size > 1 && Data[1] % 2 == 0) ? cJSON_True : cJSON_False;
    cJSON *item4 = cJSON_ParseWithLengthOpts(input_str, Size, &parse_end2, require_null2);
    cleanup_json(item4);

    // 5. Fuzz cJSON_Version (Side effect: retrieves version string)
    const char *version = cJSON_Version();
    (void)version; // Suppress unused variable warning

    // 6. Fuzz cJSON_GetErrorPtr (Only meaningful after a parse failure)
    const char *error_ptr = cJSON_GetErrorPtr();
    (void)error_ptr;

    free(input_str);
    return 0;
}