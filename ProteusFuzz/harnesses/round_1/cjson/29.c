// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_ParseWithLengthOpts at cJSON.c:1147:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ParseWithLengthOpts at cJSON.c:1147:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_malloc at cJSON.c:3197:22 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
// cJSON_ParseWithLength at cJSON.c:1232:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ParseWithOpts at cJSON.c:1131:23 in cJSON.h
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
// cJSON_ParseWithLengthOpts at cJSON.c:1147:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ParseWithLengthOpts at cJSON.c:1147:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

/* Fuzz driver for cJSON library */

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Ensure the input is treated as a C string for functions requiring it */
    char *input_str = NULL;
    const char *error_ptr = NULL;
    cJSON *json = NULL;
    const char *parse_end = NULL;

    /* Allocate buffer with extra space for null termination if needed */
    input_str = (char *)cJSON_malloc(Size + 1);
    if (!input_str) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    /* Test 1: cJSON_Parse */
    json = cJSON_Parse(input_str);
    if (json) {
        cJSON_Delete(json);
        json = NULL;
    } else {
        /* Check error pointer after failed parse */
        error_ptr = cJSON_GetErrorPtr();
        (void)error_ptr; /* Suppress unused warning */
    }

    /* Test 2: cJSON_ParseWithLength */
    json = cJSON_ParseWithLength(input_str, Size);
    if (json) {
        cJSON_Delete(json);
        json = NULL;
    }

    /* Test 3: cJSON_ParseWithOpts */
    json = cJSON_ParseWithOpts(input_str, &parse_end, 0);
    if (json) {
        cJSON_Delete(json);
        json = NULL;
    } else {
        error_ptr = cJSON_GetErrorPtr();
        (void)error_ptr;
    }

    /* Test 4: cJSON_ParseWithOpts with require_null_terminated = 1 */
    json = cJSON_ParseWithOpts(input_str, &parse_end, 1);
    if (json) {
        cJSON_Delete(json);
        json = NULL;
    } else {
        error_ptr = cJSON_GetErrorPtr();
        (void)error_ptr;
    }

    /* Test 5: cJSON_ParseWithLengthOpts with require_null_terminated = 0 */
    json = cJSON_ParseWithLengthOpts(input_str, Size, &parse_end, 0);
    if (json) {
        cJSON_Delete(json);
        json = NULL;
    } else {
        error_ptr = cJSON_GetErrorPtr();
        (void)error_ptr;
    }

    /* Test 6: cJSON_ParseWithLengthOpts with require_null_terminated = 1 */
    json = cJSON_ParseWithLengthOpts(input_str, Size, &parse_end, 1);
    if (json) {
        cJSON_Delete(json);
        json = NULL;
    } else {
        error_ptr = cJSON_GetErrorPtr();
        (void)error_ptr;
    }

    /* Test 7: cJSON_ParseWithLengthOpts with zero length */
    json = cJSON_ParseWithLengthOpts(input_str, 0, &parse_end, 0);
    if (json) {
        cJSON_Delete(json);
        json = NULL;
    }

    /* Test 8: cJSON_ParseWithLengthOpts with NULL value */
    json = cJSON_ParseWithLengthOpts(NULL, Size, &parse_end, 0);
    if (json) {
        cJSON_Delete(json);
        json = NULL;
    }

    /* Test 9: cJSON_ParseWithLengthOpts with NULL return_parse_end */
    json = cJSON_ParseWithLengthOpts(input_str, Size, NULL, 0);
    if (json) {
        cJSON_Delete(json);
        json = NULL;
    }

    /* Test 10: cJSON_ParseWithLengthOpts with NULL return_parse_end and require_null_terminated = 1 */
    json = cJSON_ParseWithLengthOpts(input_str, Size, NULL, 1);
    if (json) {
        cJSON_Delete(json);
        json = NULL;
    }

    /* Cleanup */
    cJSON_free(input_str);

    return 0;
}