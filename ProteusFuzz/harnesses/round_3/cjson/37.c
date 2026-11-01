// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_ParseWithOpts at cJSON.c:1131:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_malloc at cJSON.c:3197:22 in cJSON.h
// cJSON_free at cJSON.c:3202:20 in cJSON.h
// cJSON_malloc at cJSON.c:3197:22 in cJSON.h
// cJSON_malloc at cJSON.c:3197:22 in cJSON.h
// cJSON_ParseWithLength at cJSON.c:1232:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
// cJSON_ParseWithLengthOpts at cJSON.c:1147:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
// cJSON_ParseWithLengthOpts at cJSON.c:1147:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
// cJSON_ParseWithOpts at cJSON.c:1131:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_GetErrorPtr at cJSON.c:94:28 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"

/* Ensure the buffer is null-terminated for functions that require it */
static char *ensure_null_terminated(const uint8_t *data, size_t size, size_t *out_size) {
    char *buf;
    size_t copy_size = size;

    if (size == 0) {
        return NULL;
    }

    /* Check if the last byte is already null */
    if (data[size - 1] == '\0') {
        buf = (char *)cJSON_malloc(size);
        if (!buf) {
            return NULL;
        }
        memcpy(buf, data, size);
        *out_size = size;
    } else {
        /* Allocate one extra byte for the null terminator */
        buf = (char *)cJSON_malloc(size + 1);
        if (!buf) {
            return NULL;
        }
        memcpy(buf, data, size);
        buf[size] = '\0';
        *out_size = size + 1;
    }

    return buf;
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    cJSON *item = NULL;
    const char *parse_end = NULL;
    char *null_terminated_buf = NULL;
    size_t null_terminated_size = 0;
    cJSON_bool require_null_terminated = 0;

    if (Size == 0) {
        return 0;
    }

    /* Test 1: cJSON_ParseWithLength (No null terminator required) */
    item = cJSON_ParseWithLength((const char *)Data, Size);
    if (item) {
        cJSON_Delete(item);
        item = NULL;
    } else {
        /* If parse failed, we can check the error pointer */
        cJSON_GetErrorPtr();
    }

    /* Test 2: cJSON_ParseWithLengthOpts */
    item = cJSON_ParseWithLengthOpts((const char *)Data, Size, &parse_end, 0);
    if (item) {
        cJSON_Delete(item);
        item = NULL;
    } else {
        cJSON_GetErrorPtr();
    }

    /* Test 3: cJSON_ParseWithLengthOpts with require_null_terminated = 1 */
    /* Note: This might fail if the buffer isn't null-terminated, which is expected behavior */
    item = cJSON_ParseWithLengthOpts((const char *)Data, Size, &parse_end, 1);
    if (item) {
        cJSON_Delete(item);
        item = NULL;
    } else {
        cJSON_GetErrorPtr();
    }

    /* Prepare null-terminated buffer for functions that require it */
    null_terminated_buf = ensure_null_terminated(Data, Size, &null_terminated_size);
    if (null_terminated_buf) {
        /* Test 4: cJSON_Parse (Requires null-terminated string) */
        item = cJSON_Parse(null_terminated_buf);
        if (item) {
            cJSON_Delete(item);
            item = NULL;
        } else {
            cJSON_GetErrorPtr();
        }

        /* Test 5: cJSON_ParseWithOpts (Requires null-terminated string) */
        item = cJSON_ParseWithOpts(null_terminated_buf, &parse_end, 0);
        if (item) {
            cJSON_Delete(item);
            item = NULL;
        } else {
            cJSON_GetErrorPtr();
        }

        /* Test 6: cJSON_ParseWithOpts with require_null_terminated = 1 */
        item = cJSON_ParseWithOpts(null_terminated_buf, &parse_end, 1);
        if (item) {
            cJSON_Delete(item);
            item = NULL;
        } else {
            cJSON_GetErrorPtr();
        }

        cJSON_free(null_terminated_buf);
    }

    /* Test 7: cJSON_malloc (Test memory allocation directly) */
    void *mem = cJSON_malloc(Size > 0 ? Size : 1);
    if (mem) {
        /* Write some data to ensure the memory is usable */
        memset(mem, 0, Size > 0 ? Size : 1);
        cJSON_free(mem);
    }

    return 0;
}