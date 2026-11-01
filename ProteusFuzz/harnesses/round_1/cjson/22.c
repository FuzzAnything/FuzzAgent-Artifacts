// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_PrintUnformatted at cJSON.c:1312:22 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
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

/* Helper to create a cJSON object from raw input data */
static cJSON *create_cjson_from_input(const uint8_t *data, size_t size) {
    if (size == 0) {
        return NULL;
    }
    /* Ensure null termination */
    char *json_str = (char *)malloc(size + 1);
    if (!json_str) {
        return NULL;
    }
    memcpy(json_str, data, size);
    json_str[size] = '\0';

    cJSON *item = cJSON_Parse(json_str);
    free(json_str);
    return item;
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Test 1: cJSON_Minify */
    /* Create a mutable copy of the input to test minification */
    char *minify_buf = (char *)malloc(Size + 1);
    if (minify_buf) {
        memcpy(minify_buf, Data, Size);
        minify_buf[Size] = '\0';
        cJSON_Minify(minify_buf);
        free(minify_buf);
    }

    /* Test 2: cJSON_CreateString */
    /* Create a null-terminated string from input */
    char *str_buf = (char *)malloc(Size + 1);
    if (str_buf) {
        memcpy(str_buf, Data, Size);
        str_buf[Size] = '\0';
        cJSON *str_item = cJSON_CreateString(str_buf);
        if (str_item) {
            cJSON_Delete(str_item);
        }
        free(str_buf);
    }

    /* Test 3: Parse input to create cJSON item for other tests */
    cJSON *item = create_cjson_from_input(Data, Size);
    if (!item) {
        /* If parsing fails, try to create a simple object manually for further tests */
        item = cJSON_CreateObject();
        if (!item) {
            return 0;
        }
    }

    /* Test 4: cJSON_Print */
    char *print_buf = cJSON_Print(item);
    if (print_buf) {
        free(print_buf);
    }

    /* Test 5: cJSON_PrintUnformatted */
    char *print_unfmt_buf = cJSON_PrintUnformatted(item);
    if (print_unfmt_buf) {
        free(print_unfmt_buf);
    }

    /* Test 6: cJSON_PrintBuffered */
    /* Use Size as a hint for prebuffer, ensure it's positive */
    int prebuffer = (Size > 0) ? (int)Size : 1;
    char *print_bufed = cJSON_PrintBuffered(item, prebuffer, 0);
    if (print_bufed) {
        free(print_bufed);
    }
    /* Test with format flag 1 */
    char *print_bufed_fmt = cJSON_PrintBuffered(item, prebuffer, 1);
    if (print_bufed_fmt) {
        free(print_bufed_fmt);
    }

    /* Test 7: cJSON_PrintPreallocated */
    char *prealloc_buf = (char *)malloc(Size + 10); /* Allocate extra space */
    if (prealloc_buf) {
        int len = (int)(Size + 10);
        if (len > 0) {
            cJSON_PrintPreallocated(item, prealloc_buf, len, 0);
            cJSON_PrintPreallocated(item, prealloc_buf, len, 1);
        }
        free(prealloc_buf);
    }

    /* Cleanup */
    cJSON_Delete(item);

    return 0;
}