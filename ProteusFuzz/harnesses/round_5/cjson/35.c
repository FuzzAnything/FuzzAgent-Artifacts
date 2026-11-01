// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_malloc at cJSON.c:3197:22 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
// cJSON_ParseWithOpts at cJSON.c:1131:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ParseWithOpts at cJSON.c:1131:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
// cJSON_ParseWithLength at cJSON.c:1232:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ParseWithLengthOpts at cJSON.c:1147:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ParseWithLengthOpts at cJSON.c:1147:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
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

/* Ensure cJSON.h is included before this file is compiled */
/* The fuzzer input is treated as a raw byte buffer. We attempt to parse it
 * as JSON using various APIs to trigger different code paths and error states. */

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Create a null-terminated string for APIs that expect it */
    char *str = (char *)cJSON_malloc(Size + 1);
    if (!str) {
        return 0;
    }
    memcpy(str, Data, Size);
    str[Size] = '\0';

    cJSON *json = NULL;
    const char *parse_end = NULL;

    /* 1. Test cJSON_Parse */
    /* This is the simplest parsing function. It expects a null-terminated string. */
    json = cJSON_Parse(str);
    if (json) {
        cJSON_Delete(json);
    } else {
        /* On failure, we can inspect the error pointer */
        const char *error_ptr = cJSON_GetErrorPtr();
        (void)error_ptr; /* Suppress unused variable warning */
    }

    /* 2. Test cJSON_ParseWithOpts */
    /* Test with require_null_terminated = false */
    json = cJSON_ParseWithOpts(str, &parse_end, 0);
    if (json) {
        cJSON_Delete(json);
    }

    /* Test with require_null_terminated = true */
    json = cJSON_ParseWithOpts(str, &parse_end, 1);
    if (json) {
        cJSON_Delete(json);
    } else {
        (void)cJSON_GetErrorPtr();
    }

    /* 3. Test cJSON_ParseWithLength */
    /* This function does not require a null terminator, but we pass the string we created. */
    json = cJSON_ParseWithLength(str, Size);
    if (json) {
        cJSON_Delete(json);
    }

    /* 4. Test cJSON_ParseWithLengthOpts */
    /* Test with require_null_terminated = false */
    json = cJSON_ParseWithLengthOpts(str, Size, &parse_end, 0);
    if (json) {
        cJSON_Delete(json);
    }

    /* Test with require_null_terminated = true */
    json = cJSON_ParseWithLengthOpts(str, Size, &parse_end, 1);
    if (json) {
        cJSON_Delete(json);
    } else {
        (void)cJSON_GetErrorPtr();
    }

    /* Cleanup */
    cJSON_free(str);

    return 0;
}