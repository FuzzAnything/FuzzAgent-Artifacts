// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
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
// cJSON_ParseWithLength at cJSON.c:1232:23 in cJSON.h
// cJSON_ParseWithLengthOpts at cJSON.c:1147:23 in cJSON.h
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

static void cleanup_string(char *str) {
    if (str) {
        free(str);
    }
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure null termination for string-based parsers
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    cJSON *root = NULL;
    char *output = NULL;
    const char *parse_end = NULL;

    // 1. Test cJSON_Parse
    root = cJSON_Parse(input_str);
    if (root) {
        output = cJSON_PrintBuffered(root, Size + 100, 0);
        cleanup_string(output);
        output = cJSON_PrintBuffered(root, Size + 100, 1);
        cleanup_string(output);
        cleanup_cjson(root);
    } else {
        cJSON_GetErrorPtr();
    }

    // 2. Test cJSON_ParseWithOpts
    root = cJSON_ParseWithOpts(input_str, &parse_end, 0);
    if (root) {
        cleanup_cjson(root);
    } else {
        cJSON_GetErrorPtr();
    }

    root = cJSON_ParseWithOpts(input_str, &parse_end, 1);
    if (root) {
        cleanup_cjson(root);
    } else {
        cJSON_GetErrorPtr();
    }

    // 3. Test cJSON_ParseWithLength
    root = cJSON_ParseWithLength(input_str, Size);
    if (root) {
        cleanup_cjson(root);
    } else {
        cJSON_GetErrorPtr();
    }

    // 4. Test cJSON_ParseWithLengthOpts
    root = cJSON_ParseWithLengthOpts(input_str, Size, &parse_end, 0);
    if (root) {
        cleanup_cjson(root);
    } else {
        cJSON_GetErrorPtr();
    }

    root = cJSON_ParseWithLengthOpts(input_str, Size, &parse_end, 1);
    if (root) {
        cleanup_cjson(root);
    } else {
        cJSON_GetErrorPtr();
    }

    // 5. Test edge cases with truncated input for length-based parsers
    if (Size > 0) {
        size_t small_len = Size / 2;
        if (small_len > 0) {
            root = cJSON_ParseWithLength(input_str, small_len);
            if (root) {
                cleanup_cjson(root);
            }
            root = cJSON_ParseWithLengthOpts(input_str, small_len, &parse_end, 0);
            if (root) {
                cleanup_cjson(root);
            }
        }
    }

    free(input_str);
    return 0;
}