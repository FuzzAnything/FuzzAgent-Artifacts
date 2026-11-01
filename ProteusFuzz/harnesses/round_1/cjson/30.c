// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
// cJSON_ParseWithLength at cJSON.c:1232:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
// cJSON_ParseWithOpts at cJSON.c:1131:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
// cJSON_ParseWithLengthOpts at cJSON.c:1147:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
// cJSON_ParseWithLengthOpts at cJSON.c:1147:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*
 * Fuzz driver for cjson library.
 * Targets: cJSON_Parse, cJSON_ParseWithLength, cJSON_ParseWithOpts,
 *          cJSON_ParseWithLengthOpts, cJSON_PrintBuffered, cJSON_GetErrorPtr
 */

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Ensure null-termination for functions expecting C-strings */
    char *null_terminated = (char *)malloc(Size + 1);
    if (!null_terminated) {
        return 0;
    }
    memcpy(null_terminated, Data, Size);
    null_terminated[Size] = '\0';

    cJSON *json = NULL;
    const char *parse_end = NULL;
    char *printed = NULL;

    /* 1. Test cJSON_Parse */
    json = cJSON_Parse(null_terminated);
    if (json) {
        cJSON_Delete(json);
        json = NULL;
    } else {
        /* Check error pointer after failure */
        (void)cJSON_GetErrorPtr();
    }

    /* 2. Test cJSON_ParseWithLength */
    json = cJSON_ParseWithLength((const char *)Data, Size);
    if (json) {
        cJSON_Delete(json);
        json = NULL;
    } else {
        (void)cJSON_GetErrorPtr();
    }

    /* 3. Test cJSON_ParseWithOpts */
    parse_end = NULL;
    json = cJSON_ParseWithOpts(null_terminated, &parse_end, 0);
    if (json) {
        cJSON_Delete(json);
        json = NULL;
    } else {
        (void)cJSON_GetErrorPtr();
    }

    /* 4. Test cJSON_ParseWithLengthOpts with require_null_terminated = 0 */
    parse_end = NULL;
    json = cJSON_ParseWithLengthOpts((const char *)Data, Size, &parse_end, 0);
    if (json) {
        cJSON_Delete(json);
        json = NULL;
    } else {
        (void)cJSON_GetErrorPtr();
    }

    /* 5. Test cJSON_ParseWithLengthOpts with require_null_terminated = 1 */
    parse_end = NULL;
    json = cJSON_ParseWithLengthOpts((const char *)Data, Size, &parse_end, 1);
    if (json) {
        cJSON_Delete(json);
        json = NULL;
    } else {
        (void)cJSON_GetErrorPtr();
    }

    /* 6. Test cJSON_PrintBuffered with a valid JSON object if we have one */
    /* Re-parse to get a valid object for printing */
    json = cJSON_Parse(null_terminated);
    if (json) {
        /* Try with different prebuffer sizes */
        int prebuffer = (int)Size;
        if (prebuffer < 0) prebuffer = 0;
        
        /* Unformatted */
        printed = cJSON_PrintBuffered(json, prebuffer, 0);
        if (printed) {
            free(printed);
            printed = NULL;
        }

        /* Formatted */
        printed = cJSON_PrintBuffered(json, prebuffer, 1);
        if (printed) {
            free(printed);
            printed = NULL;
        }

        cJSON_Delete(json);
        json = NULL;
    }

    /* Cleanup */
    free(null_terminated);

    return 0;
}