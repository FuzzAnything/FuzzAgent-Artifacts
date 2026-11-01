// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_ParseWithLength at cJSON.c:1232:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
// cJSON_ParseWithOpts at cJSON.c:1131:23 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
// cJSON_ParseWithOpts at cJSON.c:1131:23 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
// cJSON_ParseWithLength at cJSON.c:1232:23 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
// cJSON_ParseWithLengthOpts at cJSON.c:1147:23 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
// cJSON_ParseWithLengthOpts at cJSON.c:1147:23 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Fuzz driver for cjson library */

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Ensure null termination for string-based parsers */
    char *input = (char *)malloc(Size + 1);
    if (!input) {
        return 0;
    }
    memcpy(input, Data, Size);
    input[Size] = '\0';

    const char *parse_end = NULL;
    cJSON *root = NULL;
    char *output = NULL;

    /* 1. Test cJSON_Parse */
    root = cJSON_Parse(input);
    if (root) {
        /* Test serialization */
        output = cJSON_PrintBuffered(root, Size * 2, 0);
        if (output) {
            free(output);
        }
        output = cJSON_PrintBuffered(root, Size * 2, 1);
        if (output) {
            free(output);
        }
        cJSON_Delete(root);
    } else {
        /* Check error pointer */
        (void)cJSON_GetErrorPtr();
    }

    /* 2. Test cJSON_ParseWithOpts */
    parse_end = NULL;
    root = cJSON_ParseWithOpts(input, &parse_end, 0);
    if (root) {
        output = cJSON_PrintBuffered(root, Size * 2, 0);
        if (output) free(output);
        cJSON_Delete(root);
    } else {
        (void)cJSON_GetErrorPtr();
    }

    parse_end = NULL;
    root = cJSON_ParseWithOpts(input, &parse_end, 1);
    if (root) {
        output = cJSON_PrintBuffered(root, Size * 2, 1);
        if (output) free(output);
        cJSON_Delete(root);
    } else {
        (void)cJSON_GetErrorPtr();
    }

    /* 3. Test cJSON_ParseWithLength */
    root = cJSON_ParseWithLength(input, Size);
    if (root) {
        output = cJSON_PrintBuffered(root, Size * 2, 0);
        if (output) free(output);
        cJSON_Delete(root);
    } else {
        (void)cJSON_GetErrorPtr();
    }

    /* 4. Test cJSON_ParseWithLengthOpts */
    parse_end = NULL;
    root = cJSON_ParseWithLengthOpts(input, Size, &parse_end, 0);
    if (root) {
        output = cJSON_PrintBuffered(root, Size * 2, 1);
        if (output) free(output);
        cJSON_Delete(root);
    } else {
        (void)cJSON_GetErrorPtr();
    }

    parse_end = NULL;
    root = cJSON_ParseWithLengthOpts(input, Size, &parse_end, 1);
    if (root) {
        output = cJSON_PrintBuffered(root, Size * 2, 0);
        if (output) free(output);
        cJSON_Delete(root);
    } else {
        (void)cJSON_GetErrorPtr();
    }

    /* 5. Test edge cases with truncated input */
    if (Size > 0) {
        size_t len = Size;
        while (len > 0) {
            root = cJSON_ParseWithLength(input, len);
            if (root) {
                cJSON_Delete(root);
            }
            len--;
        }
    }

    free(input);
    return 0;
}